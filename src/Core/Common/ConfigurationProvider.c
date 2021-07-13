#include <string.h>

#include "OrcPrint.h"
#include "ConfigurationProvider.h"

#define RTRIM(x) {size_t __inr_=0;while(x[__inr_]!=' ' && x[__inr_]!='\0')__inr_++;x[__inr_]='\0';}
#define LTRIM(x) {size_t __nsl_=0,__inl_=0;while(x[__nsl_]==' ')__nsl_++;while(x[__nsl_]!='\0'){x[__inl_]=x[__nsl_];__inl_++;__nsl_++;}x[__inl_]='\0';}
#define TRIM(x) LTRIM(x) RTRIM(x)

//------------------------------------------------------------------------------
// Configuration Variables
//------------------------------------------------------------------------------
static char* config_files_path = "/etc/orchestra/";

//------------------------------------------------------------------------------
// Updating the Configuration Load Path
//------------------------------------------------------------------------------
void set_config_path(char* config_path)
{
    config_files_path = config_path;
}

//------------------------------------------------------------------------------
// Configuration Accessors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Loading the Configuration from File(s) & Environment Variables
//------------------------------------------------------------------------------
static void load_configuration_file(char* fileName);
static void load_environment_variables(char** environment_variables);

void load_configuration(char** environment_variables)
{
    orc_console_print("Reading configuration from: ");
    orc_console_print_line(config_files_path);

    // Reading Configuration Files
    load_configuration_file("");

    // Reading Environment Variables
    load_environment_variables(environment_variables);
}

static void load_configuration_file(char* fileName)
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

static void load_environment_variables(char** environment_variables)
{
    if (environment_variables != NULL)
    {
        int i = 0;
        while (environment_variables[i] != NULL)
        {
            // orc_console_print_line(environment_variables[i++]);
        }
    }
}
