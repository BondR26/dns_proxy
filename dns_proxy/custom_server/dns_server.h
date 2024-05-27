#pragma once

#include "config/config.h"


typedef struct DnsServer
{
    int sockfd;
    struct sockaddr_in dns_server_addr;
    socklen_t addr_len;
}DnsServer;


void setUpDnsServer(Config* config, DnsServer* dns_server_);

void freeDnsServer(DnsServer* dns_server_);