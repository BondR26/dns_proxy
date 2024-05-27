#include "query_manage_api.h"
#include <errno.h>


int receiveDnsQuery(UdpServer* server_, string* buf)
{
    socklen_t addrlen = sizeof(server_->client_addr);
    int len = recvfrom(server_->sockfd
        , buf->data
        , DEFAULT_CAPACITY
        , 0
        , (struct sockaddr*)&server_->client_addr
        , &addrlen);

    if (len < 0)
    {
        perror("recvfrom failed");
        printf("errno: %d\n", errno); // Print the value of errno
        return -1;
    }

    // the retunr value of recvfrom is the length of msg
    buf->size = len;
    buf->data[len] = '\0';

    return 1;
}

void parseDnsResponse(const string* const response_)
{
    // Check if response contains answer records
    if (response_->size < 12)
    {
        fprintf(stderr, "Invalid DNS response\n");
        return;
    }

    // The answer count (ANCOUNT) field is a 16-bit unsigned integer located at bytes 6 and 7 of the response
    // We need to use ntohs to convert the network byte order (big-endian) to host byte order
    uint16_t answer_count = ntohs(*(uint16_t*)(response_->data + 6));

    //find the start of the answer section of dns response
    size_t question_section_length = 0;
    const char* qname_ptr = response_->data + 12; // Start of the question section
    while (*qname_ptr != 0) 
    {
        question_section_length += *qname_ptr + 1; // Include length byte and label
        qname_ptr += *qname_ptr + 1; // Move to the next label
    }
    question_section_length += 5; // Add the QTYPE and QCLASS fields (2 bytes each) and the null terminator

    // Calculate the offset of the start of the answer section
    size_t answer_section_offset = 12 + question_section_length;

    size_t offset = answer_section_offset;
    // walk through each answer 
    int i;
    printf("Answers to the domain name : \n");
    for (i = 0; i < answer_count; i++)
    {
        // Read type, class, TTL, data length
        uint16_t type = ntohs(*(uint16_t*)(response_->data + offset));
        uint16_t dns_class = ntohs(*(uint16_t*)(response_->data + offset + 2));
        uint32_t ttl = ntohl(*(uint32_t*)(response_->data + offset + 4));
        uint16_t data_length = ntohs(*(uint16_t*)(response_->data + offset + 10));

        // Move offset to the start of the data field
        offset += 12;

        // for the purposes of simplicict i skip the certificate part
        // and would just print the domain to header
        // Process data based on record type
        if (/*type == 1 &&*/ dns_class == 1 && data_length == 4) { // Type A record (IPv4 address)
            // Read IPv4 address
            struct in_addr ipv4;
            memcpy(&ipv4, response_->data + offset, sizeof(ipv4));
            char ip_buffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &ipv4, ip_buffer, sizeof(ip_buffer));
            printf("IPv4 Address: %s\n", ip_buffer);
        }
        // Move offset to the start of the next answer record
        offset += data_length;
    } 
}


void handleDnsRequest(const UdpServer* server_
    , string* str_buf
    , Config* config_
    , DnsServer* dns_server_)
{
    printf("Received DNS query from %s:%d\n", inet_ntoa(server_->client_addr.sin_addr), ntohs(&server_->client_addr.sin_port));

    string response;
    // Forward the DNS query to the actual DNS server
    forwardDnsRequest(dns_server_, str_buf, &response);

    // Send the response back to the client
    sendto(server_->sockfd
        , response.data
        , response.size
        , 0
        , (struct sockaddr*)&server_->client_addr
        , server_->addr_len);
}

void forwardDnsRequest(const DnsServer* dns_
    , string* buf
    , string* response_buf)
{

    // Send the DNS query to the real DNS server
    int res = sendto(dns_->sockfd, &buf->data[0], buf->size, 0, (const struct sockaddr*)&dns_->dns_server_addr, dns_->addr_len);
    if (res < 0)
    {
        perror("sendto failed");
        printf("errno: %d\n", errno);
        close(dns_->sockfd);
        exit(EXIT_FAILURE);
    }

    // Receive the response from the DNS server
    response_buf->size = recvfrom(dns_->sockfd, response_buf->data, DEFAULT_CAPACITY, 0, NULL, NULL);
    if (response_buf->size < 0)
    {
        perror("recvfrom failed");
    }

    parseDnsResponse(response_buf);
}