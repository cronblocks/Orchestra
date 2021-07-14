#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "OrcPrint.h"
#include "ConfigurationProvider.h"

#define RTRIM(x) {size_t __inr_=strlen(x)-1;while(__inr_>0&&x[__inr_]==' ')__inr_--;x[__inr_+1]='\0';}
#define LTRIM(x) {size_t __nsl_=0,__inl_=0;while(x[__nsl_]==' ')__nsl_++;while(x[__nsl_]!='\0'){x[__inl_]=x[__nsl_];__inl_++;__nsl_++;}x[__inl_]='\0';}
#define TRIM_CHARS(x) {size_t __tgt_=0,__src_=0;while(__src_<=strlen(x)){x[__tgt_]=x[__src_];__src_++;if(x[__tgt_]!='"'&&x[__tgt_]!='\''&&x[__tgt_]!='\n')__tgt_++;}}
#define TRIM(x) RTRIM(x) LTRIM(x) TRIM_CHARS(x)

#define MAX_DIRECTORY_NAME_LENGTH                      300
#define MAX_FILE_NAME_LENGTH                           300
#define MAX_IP_ADDRESS_LENGTH                          100
#define MAX_USER_NAME_LENGTH                           50
#define MAX_USER_PASSWORD_LENGTH                       50

#define GENERAL_VALUES_TOTAL_VALUES                    100*1000
#define GENERAL_VALUES_SECTION_NAME_AND_KEY_LENGTH     150
#define GENERAL_VALUES_VALUE_LENGTH                    150

//------------------------------------------------------------------------------
// Configuration Variables
//------------------------------------------------------------------------------
static char* default_config_files_path = "/etc/orchestra/";

static char data_directory[MAX_DIRECTORY_NAME_LENGTH + 1];
static char temp_data_directory[MAX_DIRECTORY_NAME_LENGTH + 1];
static char log_directory[MAX_DIRECTORY_NAME_LENGTH + 1];
static char scripts_directory[MAX_DIRECTORY_NAME_LENGTH + 1];
static char user_data_directory[MAX_DIRECTORY_NAME_LENGTH + 1];
static char app_data_directory[MAX_DIRECTORY_NAME_LENGTH + 1];

static char log_file[MAX_FILE_NAME_LENGTH + 1];
static char user_data_file[MAX_FILE_NAME_LENGTH + 1];
static char app_data_file[MAX_FILE_NAME_LENGTH + 1];

static char listening_address[MAX_IP_ADDRESS_LENGTH + 1];
static unsigned short listening_port = 15209;

static char initial_user_name[MAX_USER_NAME_LENGTH + 1];
static char initial_user_password[MAX_USER_PASSWORD_LENGTH + 1];

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

static void config_set_value(const char* section, const char* key, const char* value)
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
    }
}

//------------------------------------------------------------------------------
// Loading the Configuration from File(s) & Environment Variables
//------------------------------------------------------------------------------
static int name_ends_with(const char* file_name, const char* ends);
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
                name_ends_with(full_filename, ".conf")) // name ends with .conf
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
    //////////////////////
    orc_console_print("DataDir: ");
    orc_console_print_line(data_directory);
    orc_console_print("TempDataDir: ");
    orc_console_print_line(temp_data_directory);
    orc_console_print("LogDataDir: ");
    orc_console_print_line(log_directory);
    orc_console_print("ScriptsDir: ");
    orc_console_print_line(scripts_directory);
    orc_console_print("UserDataDir: ");
    orc_console_print_line(user_data_directory);
    orc_console_print("AppDataDir: ");
    orc_console_print_line(app_data_directory);

    orc_console_print("LogFile: ");
    orc_console_print_line(log_file);
    orc_console_print("UserDataFile: ");
    orc_console_print_line(user_data_file);
    orc_console_print("AppDataFile: ");
    orc_console_print_line(app_data_file);

    orc_console_print("ListeningIP: ");
    orc_console_print_line(listening_address);
    orc_console_print("ListeningPort: ");
    orc_console_print_int(listening_port);
    orc_console_print_line("");

    orc_console_print("InitUser: ");
    orc_console_print_line(initial_user_name);
    orc_console_print("InitPwd: ");
    orc_console_print_line(initial_user_password);
    ////////////////////
}

static int name_ends_with(const char* file_name, const char* ends)
{
    if (strlen(file_name) < strlen(ends)) return 0;

    int index_file = strlen(file_name) - 1;
    int index_ends = strlen(ends) - 1;

    while (index_ends >= 0)
    {
        if (file_name[index_file] == ends[index_ends])
        {
            index_file--;
            index_ends--;
        }
        else
        {
            return 0;
        }
    }

    return 1;
}

static void process_configuration_values(const char* section, const char* key, const char* value, const int line_number);

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
                    
                    process_configuration_values(section_name, name_ptr, value_ptr, line_number);
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

static void process_configuration_values(const char* section, const char* key, const char* value, const int line_number)
{
    if (strcmp(section, "DIRECTORIES") == 0)
    {
        if (strcmp(key, "DataDirectory") == 0)
        {
            strncpy(data_directory, value, sizeof(data_directory) - 1);
        }
        else if (strcmp(key, "TempDataDirectory") == 0)
        {
            strncpy(temp_data_directory, value, sizeof(temp_data_directory) - 1);
        }
        else if (strcmp(key, "LogDirectory") == 0)
        {
            strncpy(log_directory, value, sizeof(log_directory) - 1);
        }
        else if (strcmp(key, "ScriptsDirectory") == 0)
        {
            strncpy(scripts_directory, value, sizeof(scripts_directory) - 1);
        }
        else if (strcmp(key, "UserDataDirectory") == 0)
        {
            strncpy(user_data_directory, value, sizeof(user_data_directory) - 1);
        }
        else if (strcmp(key, "AppDataDirectory") == 0)
        {
            strncpy(app_data_directory, value, sizeof(app_data_directory) - 1);
        }
        else
        {
            orc_console_print("Ignoring the unrecognized configuration value ");
            orc_console_print(key);
            orc_console_print(" = ");
            orc_console_print(value);
            orc_console_print(" in section DIRECTORIES ");
            orc_console_print(" at line# ");
            orc_console_print_int(line_number);
            orc_console_print_line("");
        }
    }
    else if (strcmp(section, "FILES") == 0)
    {
        if (strcmp(key, "LogFile") == 0)
        {
            strncpy(log_file, value, sizeof(log_file) - 1);
        }
        else if (strcmp(key, "UserDataFile") == 0)
        {
            strncpy(user_data_file, value, sizeof(user_data_file) - 1);
        }
        else if (strcmp(key, "AppDataFile") == 0)
        {
            strncpy(app_data_file, value, sizeof(app_data_file) - 1);
        }
        else
        {
            orc_console_print("Ignoring the unrecognized configuration value ");
            orc_console_print(key);
            orc_console_print(" = ");
            orc_console_print(value);
            orc_console_print(" in section FILES ");
            orc_console_print(" at line# ");
            orc_console_print_int(line_number);
            orc_console_print_line("");
        }
    }
    else if (strcmp(section, "NETWORK") == 0)
    {
        if (strcmp(key, "ListeningAddress") == 0)
        {
            strncpy(listening_address, value, sizeof(listening_address) - 1);
        }
        else if (strcmp(key, "ListeningPort") == 0)
        {
            listening_port = (unsigned short) atoi(value);
        }
        else
        {
            orc_console_print("Ignoring the unrecognized configuration value ");
            orc_console_print(key);
            orc_console_print(" = ");
            orc_console_print(value);
            orc_console_print(" in section NETWORK ");
            orc_console_print(" at line# ");
            orc_console_print_int(line_number);
            orc_console_print_line("");
        }
    }
    else if (strcmp(section, "INITIAL_USER") == 0)
    {
        if (strcmp(key, "UserName") == 0)
        {
            strncpy(initial_user_name, value, sizeof(initial_user_name) - 1);
        }
        else if (strcmp(key, "Password") == 0)
        {
            strncpy(initial_user_password, value, sizeof(initial_user_password) - 1);
        }
        else
        {
            orc_console_print("Ignoring the unrecognized configuration value ");
            orc_console_print(key);
            orc_console_print(" = ");
            orc_console_print(value);
            orc_console_print(" in section INITIAL_USER ");
            orc_console_print(" at line# ");
            orc_console_print_int(line_number);
            orc_console_print_line("");
        }
    }
    else
    {
        config_set_value(section, key, value);
    }
}

static void load_environment_variables(const char** environment_variables)
{
    if (environment_variables != NULL)
    {
        int i = 0;
        while (environment_variables[i] != NULL)
        {
            //orc_console_print_line(environment_variables[i]);
            i++;
        }
    }
}

static void expand_encoded_configuration_values()
{
    // TODO: to expand values like "LogFile = $(LogDirectory)/log-file.log"
}
