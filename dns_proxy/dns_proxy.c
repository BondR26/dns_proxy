// dns_proxy.cpp : Defines the entry point for the application.
//

#include "dns_proxy.h"
#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>

#include "config/config.h"
#include "custom_server/dns_server.h"
#include "custom_server/udp_server.h"
#include "query_manage_api/dns_filter.h"
#include "query_manage_api/query_manage_api.h"


int main()
{
	// read configuration file
	Config config;

	InitRes res = initConfig(&config);

	//dumpConfig(&config);

	if (res == eSuccess)
	{
		// set up our udp server to receive queries
		UdpServer server;
		initUdpServer(&server);

		// set up dns server to forward queries to
		DnsServer dns_server;
		setUpDnsServer(&config, &dns_server);

		string str;
		while (1)
		{
			if (1 == receiveDnsQuery(&server, &str))
			{
				printf("Received request from client\n");
				if ( eRefused == filterDnsQuery(&config, &str))
				{
					continue;
				}

				handleDnsRequest(&server, &str, &config, &dns_server);

				memset(str.data, '\0', str.size);
			}
		}

		freeDnsServer(&dns_server);
		closeUdpServer(&server);
	}
	else
	{
		if (res == NoFileExist)
		{
			printf("Config file doesnt exist\n");
		}
		else if (res == eUnableToInitConfig)
		{
			printf("Unable to init config\n");
		}
	}
	//
	uninitConfig(&config);

	return 0;
}
