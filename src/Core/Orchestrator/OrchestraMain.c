#include <string.h>
#include <stdlib.h>

#include "Helpers.h"
#include "ConfigurationProvider.h"
#include "TcpConnectionManager.h"
#include "CryptoServiceProvider.h"
#include "ModulesExecutionManager.h"

void parse_command_line_arguments(int argc, char** argv);
void setup_execution_environment();
void execute();

int main(int argc, char** argv)
{
    if (argc > 1)
    {
        parse_command_line_arguments(argc, argv);
    }

    setup_execution_environment();

    execute();
}

void parse_command_line_arguments(int argc, char** argv)
{
    int isPrintAbout = 0;
    int isPrintHelp = 0;

    char* configFilePath;
    int isSetConfig = 0;

    for (int i=0; i<argc; i++)
    {
        char* argument = argv[i];

        if (strcmp(argument, "--about") == 0)
        {
            isPrintAbout = 1;
        }

        if (strcmp(argument, "--help") == 0)
        {
            isPrintHelp = 1;
        }

        if (strcmp(argument, "--config") == 0)
        {
            if (i < argc-1)
            {
                configFilePath = argv[++i];
                isSetConfig = 1;
            }
        }
    }

    if (isPrintAbout)
    {
        print_about();
        exit(0);
    }
    else if (isPrintHelp)
    {
        print_help();
        exit(0);
    }
    else if(isSetConfig)
    {
        set_config_path(configFilePath);
    }
}

void setup_execution_environment()
{
    load_configuration();
}

void execute()
{
    //
}