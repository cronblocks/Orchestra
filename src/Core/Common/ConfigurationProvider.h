#pragma once

void load_configuration(const char* config_files_path, const char** environment_variables);

// Value Getters
const char* config_get_data_directory();
const char* config_get_temp_data_directory();
const char* config_get_log_directory();
const char* config_get_scripts_directory();
const char* config_get_user_data_directory();
const char* config_get_app_data_directory();

const char* config_get_log_file();
const char* config_get_user_data_file();
const char* config_get_app_data_file();

const char* config_get_listening_address();
const unsigned short config_get_listening_port();

const char* config_get_initial_user_name();
const char* config_get_initial_user_password();
