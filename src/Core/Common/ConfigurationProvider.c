#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "OrcPrint.h"
#include "ConfigurationProvider.h"

#define RTRIM(x) {size_t __inr_=strlen(x)-1;while(__inr_>0&&x[__inr_]==' ')__inr_--;x[__inr_+1]='\0';}
#define LTRIM(x) {size_t __nsl_=0,__inl_=0;while(x[__nsl_]==' ')__nsl_++;while(x[__nsl_]!='\0'){x[__inl_]=x[__nsl_];__inl_++;__nsl_++;}x[__inl_]='\0';}
#define TRIM_CHARS(x) {size_t __tgt_=0,__src_=0;while(__src_<=strlen(x)){x[__tgt_]=x[__src_];__src_++;if(x[__tgt_]!='"'&&x[__tgt_]!='\''&&x[__tgt_]!='\n')__tgt_++;}}
#define TRIM(x) RTRIM(x) LTRIM(x) TRIM_CHARS(x)

//------------------------------------------------------------------------------
// Configuration Variables
//------------------------------------------------------------------------------
static char* default_config_files_path = "/etc/orchestra/";

static char data_directory[300];
static char temp_data_directory[300];
static char log_directory[300];
static char scripts_directory[300];
static char user_data_directory[300];
static char app_data_directory[300];

static char log_file[300];
static char user_data_file[300];
static char app_data_file[300];

static char listening_address[100];
static unsigned short listening_port = 15209;

static char initial_user_name[50];
static char initial_user_password[50];

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

//------------------------------------------------------------------------------
// Loading the Configuration from File(s) & Environment Variables
//------------------------------------------------------------------------------
static int name_ends_with(const char* file_name, const char* ends);
static void load_configuration_file(const char* file_name);
static void load_environment_variables(const char** environment_variables);

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
                strncpy(full_filename, default_config_files_path, sizeof(full_filename) - 1);
            }
            else
            {
                strncpy(full_filename, config_files_path, sizeof(full_filename) - 1);
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

static void load_configuration_file(const char* file_name)
{
    orc_console_print("Reading: ");
    orc_console_print_line(file_name);
    
    FILE* i_file = fopen(file_name, "r");

    if (i_file != NULL)
    {
        char line[1000];
        int is_equal_sign_present;
        char section_name[50];
        char *name_ptr;
        char *value_ptr;

        strncpy(section_name, "", sizeof(section_name));

        while (fgets(line, sizeof(line), i_file) != NULL)
        {
            TRIM(line);

            if(line[0] != '#' &&  // Leaving the comments
               strlen(line) >= 2) // Should atleast be like "a=" a blank value
            {
                orc_console_print_line(line);
            }
            /*
            string _v = string(line);

            if (string(line) == "[UINTS]")
            {
                section_name = "UINTS";
                continue;
            }
            else if (string(line) == "[USHORTS]")
            {
                section_name = "USHORTS";
                continue;
            }
            else if (string(line) == "[STRINGS]")
            {
                section_name = "STRINGS";
                continue;
            }

            is_equal_sign_present = false;
            for (size_t i = 0; i < sizeof (line); i++)
            {
                if (line[i] == 0) break;
                if ((line[i] == '=') && (i + 1 < sizeof (line) - 1))
                {
                    line[i] = '\0';
                    is_equal_sign_present = true;
                    name_ptr = &line[0];
                    value_ptr = &line[i + 1];
                    break;
                }
            }

            if (is_equal_sign_present)
            {
                TRIM(name_ptr);
                TRIM(value_ptr);

                string name = string(namePtr);

                if (section_name == "UINTS")
                {
                    uintValues[name] = (uint) atoi(valuePtr);
                }
                else if (section_name == "USHORTS")
                {
                    ushortValues[name] = (ushort) atoi(valuePtr);
                }
                else if (section_name == "STRINGS")
                {
                    stringValues[name] = string(value_ptr);
                }
            }*/
        }

        fclose(i_file);
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
