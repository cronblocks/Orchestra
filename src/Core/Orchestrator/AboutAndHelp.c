#include <stdio.h>

#include "Helpers.h"

static void print_banner()
{
    printf("Orchestra\n");
    printf("=========\n");
    printf("\n");
}

void print_about()
{
    print_banner();
    printf("Orchestra is an open-source application intended to provide\n");
    printf("an easy and lightweight interface to remote machines\n");
    printf("in order to manage remote applications' execution. In a sense\n");
    printf("it is to provide graphical and command-line interface to the\n");
    printf("remote machines for easy management. The prime reason for its\n");
    printf("existence is to handle remote Linux machines that do not have\n");
    printf("graphical environments installed for various reasons.\n");
    printf("\n");
}

void print_help()
{
    print_banner();
    printf("    --config=config-files-folder\n");
    printf("\n");
}