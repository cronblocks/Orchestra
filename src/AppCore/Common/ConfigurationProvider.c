#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "ConfigurationProvider.h"
#include "OrcPrint.h"
#include "Macros.h"
#include "StringHelper.h"

#define GENERAL_VALUES_TOTAL_VALUES                    100*1000
#define GENERAL_VALUES_SECTION_NAME_AND_KEY_LENGTH     150
#define GENERAL_VALUES_VALUE_LENGTH                    300

//------------------------------------------------------------------------------
// Configuration Variables
//------------------------------------------------------------------------------
static char* default_config_files_path = "/etc/orchestra/";

static char* data_directory = "";
static char* temp_data_directory = "";
static char* log_directory = "";
static char* scripts_directory = "";
static char* user_data_directory = "";
static char* app_data_directory = "";

static char* log_file = "";
static char* user_data_file = "";
static char* app_data_file = "";

static char* listening_address = "127.0.0.1";
static unsigned short listening_port = 15209;

static char* initial_user_name = "";
static char* initial_user_password = "";

static char general_values_keys[GENERAL_VALUES_TOTAL_VALUES][GENERAL_VALUES_SECTION_NAME_AND_KEY_LENGTH + 2];
static char general_values_values[GENERAL_VALUES_TOTAL_VALUES][GENERAL_VALUES_VALUE_LENGTH + 1];

//------------------------------------------------------------------------------
// Configuration Accessors
//------------------------------------------------------------------------------
const char* config_get_data_directory()          { return data_directory;        }
const char* config_get_temp_data_directory()     { return temp_data_directory;   }
const char* config_get_log_directory()           { return log_directory;         }
const char* config_get_scripts_directory()       { return scripts_directory;     }
const char* config_get_user_data_directory()     { return user_data_directory;   }
const char* config_get_app_data_directory()      { return app_data_directory;    }

const char* config_get_log_file()                { return log_file;              }
const char* config_get_user_data_file()          { return user_data_file;        }
const char* config_get_app_data_file()           { return app_data_file;         }

const char* config_get_listening_address()       { return listening_address;     }
const unsigned short config_get_listening_port() { return listening_port;        }

const char* config_get_initial_user_name()       { return initial_user_name;     }
const char* config_get_initial_user_password()   { return initial_user_password; }

// Generic Value
static unsigned int general_values_next_index = 0;

const char* config_get_value(const char* section, const char* key)
{
    for (unsigned int i = 0; i < general_values_next_index; i++)
    {
        char* section_and_key = general_values_keys[i];
        size_t section_and_key_size = sizeof(general_values_keys[i]);

        if (strncmp(section_and_key, section, section_and_key_size)  == 0 &&
            strncmp(section_and_key + strlen(section_and_key) + 1, key, section_and_key_size - strlen(section_and_key) - 1)  == 0)
        {
            return general_values_values[i];
        }
    }

    return NULL;
}

static unsigned int config_set_value(const char* section, const char* key, const char* value)
{
    int is_already_existing = 0; unsigned int existing_at_index = 0;

    for (unsigned int i = 0; i < general_values_next_index; i++)
    {
        char* section_and_key = general_values_keys[i];
        size_t section_and_key_size = sizeof(general_values_keys[i]);

        if (strncmp(section_and_key, section, section_and_key_size)  == 0 &&
            strncmp(section_and_key + strlen(section_and_key) + 1, key, section_and_key_size - strlen(section_and_key) - 1)  == 0)
        {
            is_already_existing = 1;
            existing_at_index = i;
            break;
        }
    }

    if (is_already_existing)
    {
        strncpy(general_values_values[existing_at_index],
                value,
                sizeof(general_values_values[existing_at_index]) - 1);

        return existing_at_index;
    }
    else
    {
        strncpy(general_values_keys[general_values_next_index],
                section,
                sizeof(general_values_keys[general_values_next_index]) - 1);
        
        strncpy(general_values_keys[general_values_next_index] + strlen(section) + 1,
                key,
                sizeof(general_values_keys[general_values_next_index]) - strlen(section) - 2);
        
        strncpy(general_values_values[general_values_next_index],
                value,
                sizeof(general_values_values[general_values_next_index]) - 1);
        
        general_values_next_index++;

        return (general_values_next_index - 1);
    }
}

//------------------------------------------------------------------------------
// Loading the Configuration from File(s) & Environment Variables
//------------------------------------------------------------------------------
static void load_configuration_file(const char* file_name);
static void load_environment_variables(const char** environment_variables);
static void expand_encoded_configuration_values();

void load_configuration(const char* config_files_path, const char** environment_variables)
{
    int is_using_default_path = 0;
    DIR* config_dir = NULL;

    // Trying to read from provided path
    if (config_files_path != NULL)
    {
        is_using_default_path = 0;

        orc_console_print("Reading configuration from: ");
        orc_console_print_line(config_files_path);

        config_dir = opendir(config_files_path);
        if (config_dir == NULL)
        {
            orc_console_print("Cannot read ");
            orc_console_print_line(config_files_path);
        }
    }

    // Trying to read from default path
    if (config_dir == NULL)
    {
        is_using_default_path = 1;

        orc_console_print("Reading configuration from default location: ");
        orc_console_print_line(default_config_files_path);

        config_dir = opendir(default_config_files_path);
        if (config_dir == NULL)
        {
            orc_console_print("Cannot read ");
            orc_console_print_line(default_config_files_path);
        }
    }

    // Scanning the opened directory
    if (config_dir != NULL)
    {
        struct dirent* direntry;
        struct stat direntry_stat;
        char full_filename[1500];

        while((direntry = readdir(config_dir)) != NULL)
        {
            // Joining the path with file name
            if (is_using_default_path)
            {
                strncpy(full_filename, default_config_files_path, sizeof(full_filename));
            }
            else
            {
                strncpy(full_filename, config_files_path, sizeof(full_filename));
            }
            if(full_filename[strlen(full_filename) - 1] != '/')
            {
                strncat(full_filename, "/", sizeof(full_filename) - 1);
            }
            strncat(full_filename, direntry->d_name, sizeof(full_filename) - 1);

            // Getting information about current entry
            stat(full_filename, &direntry_stat);
            if (S_ISREG(direntry_stat.st_mode) &&       // it is a regular file
                string_ends_with(full_filename, ".conf")) // name ends with .conf
            {
                // Reading Configuration File
                load_configuration_file(full_filename);
            }
        }

        closedir(config_dir);
    }
    
    // Reading Environment Variables
    load_environment_variables(environment_variables);

    // Expanding the encoded values
    expand_encoded_configuration_values();
}

static void process_configuration_value(const char* section, const char* key, const char* value, const int line_number, const int is_environment_variable);

static void load_configuration_file(const char* file_name)
{
    orc_console_print("Reading: ");
    orc_console_print_line(file_name);
    
    FILE* i_file = fopen(file_name, "r");

    if (i_file != NULL)
    {
        char line[1000];
        int line_number = 0;

        // Process variables
        int is_equal_sign_present;
        char section_name[50];
        char *name_ptr;
        char *value_ptr;

        strncpy(section_name, "_", sizeof(section_name) - 1);

        while (fgets(line, sizeof(line), i_file) != NULL)
        {
            TRIM(line);
            line_number++;

            if(line[0] != '#' &&  // Leaving the comments
               strlen(line) >= 2) // Should atleast be like "a=" a blank value
            {
                if (line[0] == '[')
                {
                    int is_section_name = 0;
                    for (int i = 1; i < strlen(line); i++)
                    {
                        if (line[i] == ']')
                        {
                            is_section_name = 1;
                            line[i] = '\0';
                            break;
                        }
                    }

                    if (is_section_name)
                    {
                        strncpy(section_name, line + 1, sizeof(section_name) - 1);
                        continue;
                    }
                }

                is_equal_sign_present = 0;
                for (unsigned int i = 0; i < strlen(line); i++)
                {
                    if ((line[i] == '=') && (i + 1 < sizeof (line) - 1))
                    {
                        line[i] = '\0';

                        name_ptr = &line[0];
                        value_ptr = &line[i + 1];

                        is_equal_sign_present = 1;
                        break;
                    }
                }

                if (is_equal_sign_present)
                {
                    TRIM(name_ptr);
                    TRIM(value_ptr);
                    
                    process_configuration_value(section_name, name_ptr, value_ptr, line_number, 0);
                }
                else
                {
                    orc_console_print("Ignoring wrongly formatted configuration value ");
                    orc_console_print(line);
                    orc_console_print(" in section ");
                    orc_console_print(section_name);
                    orc_console_print(" at line# ");
                    orc_console_print_int(line_number);
                    orc_console_print_line("");
                }
            }
        }

        fclose(i_file);
    }
    else
    {
        orc_console_print("Error! Cannot read file: ");
        orc_console_print_line(file_name);
    }
}

static void load_environment_variables(const char** environment_variables)
{
    // Required Format: Section Name__Key=Value
    if (environment_variables != NULL)
    {
        int env_iter = 0;
        char line[1000];

        int is_equal_sign_present = 0;
        int is_section_name_present = 0;
        char *section_name, *key, *value;
        int section_end_index = 0;

        while (environment_variables[env_iter] != NULL)
        {
            strncpy(line, environment_variables[env_iter], sizeof(line) - 1);
            TRIM(line);

            is_equal_sign_present = 0;
            is_section_name_present = 0;

            section_name = &line[0];

            for (int i = 0; i < strlen(line); i++)
            {
                if (i >= 2 &&
                    is_section_name_present == 0 &&
                    line[i] == '_' && line[i-1] == '_' &&
                    (i + 1) < strlen(line))
                {
                    is_section_name_present = 1;
                    key = &line[i+1];
                    section_end_index = i - 1;
                    continue;
                }

                if (is_section_name_present &&
                    is_equal_sign_present == 0 &&
                    line[i] == '=' && (i+1) <= strlen(line))
                {
                    line[section_end_index] = '\0';
                    line[i] = '\0';
                    is_equal_sign_present = 1;
                    value = &line[i+1];
                    break;
                }
            }

            if (is_equal_sign_present && is_section_name_present)
            {
                TRIM(section_name);
                TRIM(key);
                TRIM(value);

                process_configuration_value(section_name, key, value, 0, 1);
            }
            
            env_iter++;
        }
    }
}

static void process_configuration_value(
    const char* section, const char* key, const char* value,
    const int line_number, const int is_environment_variable)
{
    // Storing all the key->value pairs for general availability
    unsigned int inserted_index = config_set_value(section, key, value);

    if (strcmp(section, "STANDARD_DIRECTORIES") == 0)
    {
        if (strcmp(key, "DataDirectory") == 0)
        {
            data_directory = general_values_values[inserted_index];
        }
        else if (strcmp(key, "TempDataDirectory") == 0)
        {
            temp_data_directory = general_values_values[inserted_index];
        }
        else if (strcmp(key, "LogDirectory") == 0)
        {
            log_directory = general_values_values[inserted_index];
        }
        else if (strcmp(key, "ScriptsDirectory") == 0)
        {
            scripts_directory = general_values_values[inserted_index];
        }
        else if (strcmp(key, "UserDataDirectory") == 0)
        {
            user_data_directory = general_values_values[inserted_index];
        }
        else if (strcmp(key, "AppDataDirectory") == 0)
        {
            app_data_directory = general_values_values[inserted_index];
        }
        else
        {
            orc_console_print("Ignoring the unrecognized standard configuration value ");
            orc_console_print(key);
            orc_console_print(" = ");
            orc_console_print(value);
            orc_console_print(" in section STANDARD_DIRECTORIES ");

            if (is_environment_variable == 0)
            {
                orc_console_print(" at line# ");
                orc_console_print_int(line_number);
            }
            else
            {
                orc_console_print(" of Environment Variables");
            }
            
            orc_console_print_line("");
        }
    }
    else if (strcmp(section, "STANDARD_FILES") == 0)
    {
        if (strcmp(key, "LogFile") == 0)
        {
            log_file = general_values_values[inserted_index];
        }
        else if (strcmp(key, "UserDataFile") == 0)
        {
            user_data_file = general_values_values[inserted_index];
        }
        else if (strcmp(key, "AppDataFile") == 0)
        {
            app_data_file = general_values_values[inserted_index];
        }
        else
        {
            orc_console_print("Ignoring the unrecognized standard configuration value ");
            orc_console_print(key);
            orc_console_print(" = ");
            orc_console_print(value);
            orc_console_print(" in section STANDARD_FILES ");

            if (is_environment_variable == 0)
            {
                orc_console_print(" at line# ");
                orc_console_print_int(line_number);
            }
            else
            {
                orc_console_print(" of Environment Variables");
            }

            orc_console_print_line("");
        }
    }
    else if (strcmp(section, "NETWORK") == 0)
    {
        if (strcmp(key, "ListeningAddress") == 0)
        {
            listening_address = general_values_values[inserted_index];
        }
        else if (strcmp(key, "ListeningPort") == 0)
        {
            listening_port = (unsigned short) atoi(value);
        }
        else
        {
            orc_console_print("Ignoring the unrecognized standard configuration value ");
            orc_console_print(key);
            orc_console_print(" = ");
            orc_console_print(value);
            orc_console_print(" in section NETWORK ");
            
            if (is_environment_variable == 0)
            {
                orc_console_print(" at line# ");
                orc_console_print_int(line_number);
            }
            else
            {
                orc_console_print(" of Environment Variables");
            }

            orc_console_print_line("");
        }
    }
    else if (strcmp(section, "INITIAL_USER") == 0)
    {
        if (strcmp(key, "UserName") == 0)
        {
            initial_user_name = general_values_values[inserted_index];
        }
        else if (strcmp(key, "Password") == 0)
        {
            initial_user_password = general_values_values[inserted_index];
        }
        else
        {
            orc_console_print("Ignoring the unrecognized standard configuration value ");
            orc_console_print(key);
            orc_console_print(" = ");
            orc_console_print(value);
            orc_console_print(" in section INITIAL_USER ");
            
            if (is_environment_variable == 0)
            {
                orc_console_print(" at line# ");
                orc_console_print_int(line_number);
            }
            else
            {
                orc_console_print(" of Environment Variables");
            }

            orc_console_print_line("");
        }
    }
}

static void find_expansion_token(
    int* dollar_sign_index,
    int* start_bracket_index, int* collon_sign_index, int* end_bracket_index,
    const char* value)
{
    *dollar_sign_index = -1;
    *start_bracket_index = -1;
    *collon_sign_index = -1;
    *end_bracket_index = -1;

    for (int i = 0; i < strlen(value); i++)
    {
        if (value[i] == '$') { *dollar_sign_index   = i; continue; }
        if (value[i] == '(') { *start_bracket_index = i; continue; }
        if (value[i] == ':') { *collon_sign_index   = i; continue; }
        if (value[i] == ')') { *end_bracket_index   = i; continue; }
    }
}

// Replaces $(Section:Key) by its value in all configuration values
static void expand_encoded_configuration_values()
{
    int dollar_sign_index,
        start_bracket_index, collon_sign_index, end_bracket_index;
    
    char temp[GENERAL_VALUES_VALUE_LENGTH + 1];

    for (unsigned int i = 0; i < general_values_next_index; i++)
    {
        do
        {
            strncpy(temp, general_values_values[i], sizeof(temp) - 1);

            find_expansion_token(
                &dollar_sign_index,
                &start_bracket_index, &collon_sign_index, &end_bracket_index,
                temp);
            
            if (dollar_sign_index >= 0 &&
                dollar_sign_index < start_bracket_index &&
                start_bracket_index < collon_sign_index &&
                collon_sign_index < end_bracket_index)
            {
                char* section = &temp[start_bracket_index + 1];
                char* key = &temp[collon_sign_index + 1];

                temp[collon_sign_index] = '\0';
                temp[end_bracket_index] = '\0';

                const char* replacement = config_get_value(section, key);
                if (replacement == NULL)
                {
                    orc_console_print("Cannot find: ");
                    orc_console_print(section);
                    orc_console_print(":");
                    orc_console_print(key);
                    orc_console_print(" for expanding ");
                    orc_console_print_line(general_values_values[i]);

                    break;
                }
                else
                {
                    string_replace_at(
                        general_values_values[i],
                        dollar_sign_index, end_bracket_index, replacement,
                        sizeof(general_values_values[i]) - 1);
                }
            }
            else
            {
                if (dollar_sign_index >= 0)
                {
                    // TODO: Specific error message to be printed (if applicable)
                }

                break;
            }
        } while (1);
    }
}
