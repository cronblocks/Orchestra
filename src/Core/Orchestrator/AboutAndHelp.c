#include "AppInfo.h"
#include "Helpers.h"
#include "OrcPrint.h"

static void print_banner()
{
    orc_console_print(APP_NAME);
    orc_console_print(" v");
    orc_console_print_line(APP_VERSION);
    orc_console_print_line("===================");
    orc_console_print_line("");
}

void print_about()
{
    print_banner();
    orc_console_print_line("Orchestra is an open-source application intended to provide");
    orc_console_print_line("an easy and lightweight interface to remote machines");
    orc_console_print_line("in order to manage remote applications' execution. In a sense");
    orc_console_print_line("it is to provide graphical and command-line interface to the");
    orc_console_print_line("remote machines for easy management. The prime reason for its");
    orc_console_print_line("existence is to handle remote Linux machines that do not have");
    orc_console_print_line("graphical environments installed for various reasons.");
    orc_console_print_line("");
}

void print_help()
{
    print_banner();
    orc_console_print_line("    --config-path config-files-path");
    orc_console_print_line("");
}