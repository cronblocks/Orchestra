#pragma once

void orc_log_print(const char* message);
void orc_log_print_int(int value);
void orc_log_print_float(float value);
void orc_log_print_double(double value);
void orc_log_print_line(const char* message);

void orc_console_print(const char* message);
void orc_console_print_int(int value);
void orc_console_print_float(float value);
void orc_console_print_double(double value);
void orc_console_print_line(const char* message);

void orc_control_link_print(const char* message);
void orc_control_link_print_int(int value);
void orc_control_link_print_float(float value);
void orc_control_link_print_double(double value);
void orc_control_link_print_line(const char* message);