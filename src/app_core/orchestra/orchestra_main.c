#include <string.h>
#include <stdlib.h>

#include "helpers.h"
#include "configuration_provider.h"
#include "tcp_connection_manager.h"
#include "crypto_service_provider.h"
#include "modules_execution_manager.h"

static void set_defaults();
static void parse_command_line_arguments(int argc, char** argv);
static void setup_execution_environment();
static void execute();

static char* config_path = NULL;

int main(int argc, char** argv, char** env) {
    set_defaults();

    if (argc > 1) {
        parse_command_line_arguments(argc, argv);
    }
    
    load_configuration(config_path, (const char**)env);

    setup_execution_environment();
    execute();
}

void set_defaults() {
    //
}

void parse_command_line_arguments(int argc, char** argv) {
    int isPrintAbout = 0;
    int isPrintHelp = 0;

    for (int i = 0; i < argc; i++) {
        char* argument = argv[i];

        if (strcmp(argument, "--about") == 0) {
            isPrintAbout = 1;
            break;
        }

        if (strcmp(argument, "--help") == 0) {
            isPrintHelp = 1;
            break;
        }

        if (strcmp(argument, "--config-path") == 0) {
            if (i < argc-1) {
                config_path = argv[++i];
            }
        }
    }

    // Exitting after showing the help
    if (isPrintAbout) {
        print_about();
        exit(0);
    } else if (isPrintHelp) {
        print_help();
        exit(0);
    }
}

void setup_execution_environment() {
    //
}

void execute() {
    //
}