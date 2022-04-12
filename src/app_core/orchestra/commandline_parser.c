#include <string.h>
#include <stdlib.h>

#include "commandline_parser.h"

#include "about_info.h"

void parse_command_line_arguments(int argc, char** argv, char** config_path) {
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
                *config_path = argv[++i];
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
