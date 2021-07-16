#include <string.h>

#include "StringHelper.h"

// Find "match" in "target" and replace it with "replacement"
void string_replace(char* target, const char* match, const char* replacement, unsigned int target_size)
{
    // TODO:
}

// Replace from "start_index" to "end_index" in "target" with "replacement"
void string_replace_at(char* target, int start_index, int end_index, const char* replacement, unsigned int target_size)
{
    char temp[target_size + strlen(replacement)];
    memset(temp, 0, sizeof(temp));

    strncpy(temp, target, start_index);
    strncat(temp, replacement, sizeof(temp) - 1);
    strncat(temp, &target[end_index + 1], sizeof(temp) - 1);

    strncpy(target, temp, target_size - 1);
}

int string_ends_with(const char* target, const char* ends)
{
    if (strlen(target) < strlen(ends)) return 0;

    int index_file = strlen(target) - 1;
    int index_ends = strlen(ends) - 1;

    while (index_ends >= 0)
    {
        if (target[index_file] == ends[index_ends])
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
