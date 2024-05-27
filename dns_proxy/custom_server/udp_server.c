#include "udp_server.h"
#include <errno.h>


#define DNS_PORT 8080

int createSocket(int* sockfd_)
{
    if ((*sockfd_ = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Socket creation failed");
        return -1;
    }
    printf("Socket was created succesfully\n");
    return 1;
}

void fillServerInfo(struct sockaddr_in* server_addr)
{
    // Fill server information
    memset(server_addr, 0, sizeof(*server_addr));
    server_addr->sin_family = AF_INET; // IPv4
    server_addr->sin_addr.s_addr = INADDR_ANY;
    server_addr->sin_port = htons(DNS_PORT);
}

// Bind the socket with the server address
void bindSocketToAdress(int sockfd, struct sockaddr_in* server_addr)
{
    int res = bind(sockfd, (const struct sockaddr*)server_addr, sizeof(*server_addr));
    if (res < 0)
    {
        perror("Bind failed");
        printf("errno: %d\n", errno); // Print the value of errno
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("the socket was binded to a server\n");
}

void initUdpServer(UdpServer* server_)
{
    server_->addr_len = sizeof(server_->client_addr);
    
    if (createSocket(&server_->sockfd) == 1)
    {
        fillServerInfo(&server_->server_addr);

        bindSocketToAdress(server_->sockfd, &server_->server_addr);

        printf("UDP server is running on port %d...\n", DNS_PORT);
    }
}

void closeUdpServer(UdpServer* server_)
{
    close(server_->sockfd);
}