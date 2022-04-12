#include <stddef.h>

#include "commandline_parser.h"
#include "configuration_provider.h"
#include "tcp_connection_manager.h"
#include "crypto_service_provider.h"
#include "modules_execution_manager.h"

static void setup_execution_environment();
static void execute();

static char* config_path = NULL;

int main(int argc, char** argv, char** env) {
    if (argc > 1) {
        parse_command_line_arguments(argc, argv, &config_path);
    }
    
    load_configuration(config_path, (const char**)env);

    setup_execution_environment();
    execute();
}

void setup_execution_environment() {
    //
}

void execute() {
    //
}