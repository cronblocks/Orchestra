#include "about_info.h"

#include "app_info.h"
#include "orc_print.h"

static void print_banner() {
    orc_console_print(APP_NAME);
    orc_console_print(" v");
    orc_console_print_line(APP_VERSION);
    orc_console_print_line("===================");
    orc_console_print_line("");
}

void print_about() {
    print_banner();
    orc_console_print_line("Orchestra is an open-source application intended to provide");
    orc_console_print_line("an  easy  and  light-weight  interface  to  remote-machines");
    orc_console_print_line("in order to manage remote-applications' execution.");
    orc_console_print_line("");
    orc_console_print_line("In a sense, to provide graphical and command-line interface");
    orc_console_print_line("to headless remote-machines for ease of management, and  to");
    orc_console_print_line("automate various tasks, make Orchestra's reason to exist.");
    orc_console_print_line("");
    orc_console_print_line("For more information, please see:");
    orc_console_print_line("         https://github.com/cronblocks/orchestra");
    orc_console_print_line("");
}

void print_help() {
    print_banner();
    orc_console_print_line("Use:");
    orc_console_print_line("  --about                           : To print information about the software");
    orc_console_print_line("  --config-path <config-files-path> : To provide path containing *.conf files");
    orc_console_print_line("  --help                            : To print this help information");
    orc_console_print_line("");
}
