#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

#include "OrcPrint.h"
#include "ConfigurationProvider.h"

#define RTRIM(x) {size_t __inr_=0;while(x[__inr_]!=' ' && x[__inr_]!='\0')__inr_++;x[__inr_]='\0';}
#define LTRIM(x) {size_t __nsl_=0,__inl_=0;while(x[__nsl_]==' ')__nsl_++;while(x[__nsl_]!='\0'){x[__inl_]=x[__nsl_];__inl_++;__nsl_++;}x[__inl_]='\0';}
#define TRIM(x) LTRIM(x) RTRIM(x)

//------------------------------------------------------------------------------
// Configuration Variables
//------------------------------------------------------------------------------
static char* default_config_files_path = "/etc/orchestra/";

//------------------------------------------------------------------------------
// Configuration Accessors
//------------------------------------------------------------------------------

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
                orc_console_print("Reading: ");
                orc_console_print_line(full_filename);
                
                // Reading Configuration Files
                load_configuration_file("");
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

static void load_configuration_file(const char* fileName)
{
    /*
    ifstream iFile(CONFIGURATION_FILENAME);

    if (iFile.is_open())
    {
        char line[500];
        bool isEqualSignPresent;
        string sectionName = "STRINGS";
        char *namePtr;
        char *valuePtr;

        while (!iFile.eof())
        {
            iFile.getline(line, sizeof (line));

            string _v = string(line);

            if (string(line) == "[UINTS]")
            {
                sectionName = "UINTS";
                continue;
            }
            else if (string(line) == "[USHORTS]")
            {
                sectionName = "USHORTS";
                continue;
            }
            else if (string(line) == "[STRINGS]")
            {
                sectionName = "STRINGS";
                continue;
            }

            isEqualSignPresent = false;
            for (size_t i = 0; i < sizeof (line); i++)
            {
                if (line[i] == 0) break;
                if ((line[i] == '=') && (i + 1 < sizeof (line) - 1))
                {
                    line[i] = '\0';
                    isEqualSignPresent = true;
                    namePtr = &line[0];
                    valuePtr = &line[i + 1];
                    break;
                }
            }

            if (isEqualSignPresent)
            {
                TRIM(namePtr);
                TRIM(valuePtr);

                string name = string(namePtr);

                if (sectionName == "UINTS")
                {
                    uintValues[name] = (uint) atoi(valuePtr);
                }
                else if (sectionName == "USHORTS")
                {
                    ushortValues[name] = (ushort) atoi(valuePtr);
                }
                else if (sectionName == "STRINGS")
                {
                    stringValues[name] = string(valuePtr);
                }
            }
        }

        iFile.close();
    }
    */
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
