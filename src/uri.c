/*
** uri.c
** File description:
** URI related functions
*/

#include "dragon.h"

bool is_uri(char *uri) {
    for (int i=0; uri[i]; i++)
        if (uri[i] == '/')
            return false;
        else if (uri[i] == ':')
            return true;
    return false;
}

bool is_file_uri(char *uri) {
    char *prefix = "file:";
    return strncmp(prefix, uri, strlen(prefix)) == 0;
}

