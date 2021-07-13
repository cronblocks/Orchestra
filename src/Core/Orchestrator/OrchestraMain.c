#include <stdio.h>
#include <string.h>

#include "./Helpers.h"
#include "./Common/ConfigurationProvider.h"
#include "./Connectivity/TcpConnectionManager.h"
#include "./Crypto/CryptoServiceProvider.h"
#include "./ExecutionModulesManagement/ModulesExecutionManager.h"

int main(int argc, char** argv)
{
    if (argc > 1)
    {
        for (int i=0; i<argc; i++)
        {
            char* argument = argv[i];

            if (strcmp(argument, "--help") == 0)
            {
                print_help();
                return 0;
            }

            if (strcmp(argument, "--about") == 0)
            {
                print_about();
                return 0;
            }
        }
    }

    printf("\n");
}
