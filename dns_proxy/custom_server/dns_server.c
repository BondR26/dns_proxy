#include "dns_server.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


void setUpDnsServer(Config* config, DnsServer* server_)
{
    if ((server_->sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Optionally set a receive timeout
    // struct timeval timeout;
    // timeout.tv_sec = 5;  // 5 seconds timeout
    // timeout.tv_usec = 0;
    // setsockopt(server_->sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    // Configure the DNS server address
    //memset(server_->dns_server_addr, 0, sizeof(server_->dns_server_addr));
    server_->dns_server_addr.sin_family = AF_INET;
    server_->dns_server_addr.sin_port = htons(53);  // Standard DNS port
    printf("Dns port %d\n", ntohs(server_->dns_server_addr.sin_port));
    //if (inet_pton(AF_INET, config->_dns_ip.data, &server_->dns_server_addr.sin_addr) <= 0)
    if (inet_pton(AF_INET, "192.168.0.1", &server_->dns_server_addr.sin_addr) <= 0)
    {
        perror("Invalid DNS server address");
        return -1;
    }
    server_->addr_len = sizeof(server_->dns_server_addr);

    printf("Dns Ip adress %s\n", inet_ntoa(server_->dns_server_addr.sin_addr));
    return 1;
}

void freeDnsServer(DnsServer* dns_server_)
{
    close(dns_server_->sockfd);
}