#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "user_data_types/custom_string.h"
#include "config/config.h"


#define PORT 8080

typedef struct UdpServer
{
    int sockfd;
    string str_buf;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t addr_len;
}UdpServer;

int createSocket(int* sockfd_);

void fillServerInfo(struct sockaddr_in* server_addr);
// Bind the socket with the server address
void bindSocketToAdress(int sockfd, struct sockaddr_in* server_addr);

void initUdpServer(UdpServer* server_);

void closeUdpServer(UdpServer* server_);