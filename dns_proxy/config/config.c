#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#include "user_data_types/custom_vector.h"
#include "resources.h"


typedef enum ExtractionResult
{
    eFailure,
    eExtracted
    
} ExtractionResult;

ExtractionResult extractValue(char* src, const char* start_tag, const char* end_tag, string* result_)
{
    char* begining = strstr(src, start_tag);
    if (begining != NULL)
    {
        // move to right after the tag ( i.e. strle<ip>
        begining += strlen(start_tag);
        // get the size from the begingin of vlaue up to </ip>
        char* end = strstr(begining, end_tag);
        if (end != NULL)
        {
            // ge the size from tag to tag
            int str_size = end - begining;

            strncpy(result_->data, begining, str_size);
            result_->data[str_size] = '\0';
            result_->size = str_size;

            return eExtracted;
        }
    }

    printf("Config file damaged\n");
    return eFailure;
}

InitRes initConfig(Config* config)
{
    if (config != NULL)
    {
        allocStringVector(0, &config->_blacklist);

        FILE* config_file = NULL;

        config_file = fopen(gConfigFile, "r");
        // Check if the file was opened successfully
        if (config_file == NULL)
        {
            printf("Unable to open the file.\n");
            return;
        }

        char line[1024];
      
        // Read and print each line until the end of the file
        while (fgets(line, sizeof(line), config_file))
        {
            if (strstr(line, "<ip>") != NULL)
            {
                string str;
                extractValue(line, "<ip>", "</ip>", &str);
                printf("IP: %s\n", str.data);

                // init dns addr
                config->_dns_ip = str;
                config->_dns_addr.s_addr = inet_addr(str.data);
            }
            else if (strstr(line, "<blacklist>") != NULL)
            {
                //get the next name lines unitl we reach the closing tag
                while (fgets(line, sizeof(line), config_file))
                {
                    // if the current line isnt end tag of blacklist
                    if (strstr(line, "</blacklist>") == NULL)
                    {
                        string str;
                        ExtractionResult res = extractValue(line, "<name>", "</name>", &str);
                        if (res == eFailure)
                        {
                            printf("The balcklist name wasnt processed\n");
                        }
                        else
                        {
                            appendStr(&config->_blacklist, str);
                            printf("Blacklist: %s\n", str.data);
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
            else if (strstr(line, "<responses>") != NULL)
            {
                eResponse resp[] = { eNotFound, eRefused, eResolve };
                int index = 0;
                //get the next name lines unitl we reach the closing tag
                while (fgets(line, sizeof(line), config_file))
                {
                    // if the current line isnt end tag of blacklist
                    if (strstr(line, "</responses>") == NULL)
                    {
                        string str;
                        ExtractionResult res = extractValue(line, "<resp>", "</resp>", &str);
                        if (res == eFailure)
                        {
                            printf("The response name wasnt processed\n");
                        }
                        else
                        {
                            config->_response[index].first  = resp[index];
                            config->_response[index].second = str;
                            index++;

                            printf("Responses: %s\n", str.data);
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }

        fclose(config_file);

        return eSuccess;
    }
}

void uninitConfig(Config* config)
{
    uninitStringVector(&config->_blacklist);
}

void dumpConfig(Config* config)
{
    printf("Read Dns Ip adress from config.day %s\n", inet_ntoa(config->_dns_addr));
    
    {
        int i = 0;
        printf("Blacklist : \n");
        for (i = 0; i < config->_blacklist.size; i++)
        {
            printf("%s \n", config->_blacklist.data[i]->data);
        }
    }

    {
        int i;
        for (i = 0; i < RESPONSES_SIZE; i++)
        {
            printf("enum : %d , reponse :%s\n", config->_response[i].first, config->_response[i].second.data);
        }
    }
}