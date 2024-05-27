#pragma once

#include "custom_server/dns_server.h"
#include "custom_server/udp_server.h"


int receiveDnsQuery(UdpServer* server_, string* buf);

// Forward declarations
void handleDnsRequest(const UdpServer* server_
    , string* buf
    , Config* config_
    , DnsServer* dns_server_);

void forwardDnsRequest(const DnsServer* _dns_addr
    , string* buf
    , string* response_);