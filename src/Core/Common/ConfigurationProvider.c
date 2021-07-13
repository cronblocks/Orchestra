#include <string.h>

#include "OrcPrint.h"
#include "ConfigurationProvider.h"

void set_config_path(char* config_path)
{
    orc_console_print("Reading configuration from: ");
    orc_console_print_line(config_path);
}