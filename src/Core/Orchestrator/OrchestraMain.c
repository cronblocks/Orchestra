#include <string.h>
#include <stdlib.h>

#include "Helpers.h"
#include "ConfigurationProvider.h"
#include "TcpConnectionManager.h"
#include "CryptoServiceProvider.h"
#include "ModulesExecutionManager.h"

static void parse_command_line_arguments(int argc, char** argv);
static void setup_execution_environment();
static void execute();

static char* config_path = NULL;

int main(int argc, char** argv, char** env)
{
    if (argc > 1)
    {
        parse_command_line_arguments(argc, argv);
    }
    
    load_configuration(config_path, env);

    setup_execution_environment();
    execute();
}

void parse_command_line_arguments(int argc, char** argv)
{
    int isPrintAbout = 0;
    int isPrintHelp = 0;

    int isSetConfig = 0;

    for (int i=0; i<argc; i++)
    {
        char* argument = argv[i];

        if (strcmp(argument, "--about") == 0)
        {
            isPrintAbout = 1;
            break;
        }

        if (strcmp(argument, "--help") == 0)
        {
            isPrintHelp = 1;
            break;
        }

        if (strcmp(argument, "--config") == 0)
        {
            if (i < argc-1)
            {
                config_path = argv[++i];
                isSetConfig = 1;
            }
        }
    }

    // Exitting on doing the needful
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
    
    // Processing the command-line switches
    //
}

void setup_execution_environment()
{
    //
}

void execute()
{
    //
}