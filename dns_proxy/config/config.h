#pragma once

#include <netinet/in.h>
#include <stdio.h>

#include "user_data_types/custom_vector.h"
#include "user_data_types/custom_pair.h"

#define RESPONSES_SIZE 3

typedef struct DnsProxyConfig
{
	struct in_addr _dns_addr;
	string		   _dns_ip;
	StringVector   _blacklist;
	KeyValPair     _response[RESPONSES_SIZE];
	
} Config;

typedef enum eInit
{
	eSuccess,
	NoFileExist,
	eUnableToInitConfig
}InitRes;

InitRes initConfig(Config* config);

void uninitConfig(Config* config);

void dumpConfig(Config* config);
