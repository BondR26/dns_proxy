#include "dns_filter.h"
#include <stdlib.h>


string getDomainName(const string* query)
{
    string domain_name;

    int p = 0, jumped = 0, offset;
    int i, length = 0;

    unsigned char* quetion_dns_part = &query->data[12];

    while (*quetion_dns_part != 0)
    {
        // either the name is pointer to previous packet
        if (*quetion_dns_part >= 192) { // Compressed name
            offset = (*quetion_dns_part) * 256 + *(quetion_dns_part + 1) - 49152;
            quetion_dns_part = query->data + offset;
            jumped = 1;
        }
        else 
        {
            // or it is the sequence of labels 
            // first byte is the length of next string(exmaple), then the lenght before . and then the domain(com)
            int label_len = *quetion_dns_part;
            
            quetion_dns_part++;
            length += label_len + 1; // Add label length and the length byte
            
            for (i = 0; i < label_len; i++) 
            {
                domain_name.data[p++] = *quetion_dns_part;
                quetion_dns_part++;
            }
            domain_name.data[p++] = '.';
        }

        if (jumped == 0) {
            length++;
        }
    }

    domain_name.data[p - 1] = '\0'; // Null terminate the domain name
    domain_name.size = length + 1; // Include the null terminator in the length

    return domain_name;
}

bool isInBlacklist(const StringVector* blacklist, const string* const name_)
{
    if (isInSortedAscVector(blacklist, name_))
    {
        return true;
    }
    else
    {
        return false;
    }
}

eResponse filterDnsQuery(Config* config, string* query_)
{
	string dname = getDomainName(query_);
    
    printf("Request for %s\n", dname.data);

    if (!config->_blacklist.is_sortedAscending)
    {
        sortAscending(&config->_blacklist);
    }

    //dumpConfig(config);

    if (isInBlacklist(&config->_blacklist, &dname))
    {
        return eRefused;
    }

    return eResolve;
}