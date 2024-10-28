#include "utils.h"
#include "log.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tokenize.h" 

#define MAX_ALIAS_COUNT MAX_STRING_LENGTH

typedef struct Alias {
    char *name;
    char *value;
} Alias;

Alias *alias_table[MAX_ALIAS_COUNT];
int alias_count = 0;

int set_alias(char *name, char *value) {
    for (int i = 0; i < alias_count; i++) {
        if (strcmp(alias_table[i]->name, name) == 0) {
            free(alias_table[i]->value);
            alias_table[i]->value = COPY(value);
            return EXIT_SUCCESS;
        }
    }

    if (alias_count < MAX_ALIAS_COUNT) {
        Alias *alias = malloc(sizeof(Alias));
        alias->name = COPY(name);
        alias->value = COPY(value);
        alias_table[alias_count++] = alias;
        return EXIT_SUCCESS;
    } else {
        LOG_ERROR("Alias table overflow\n");
        return EXIT_FAILURE;
    }
}

int remove_alias(char *name) {
    for (int i = 0; i < alias_count; i++) {
        if (strcmp(alias_table[i]->name, name) == 0) {
            free(alias_table[i]->name);
            free(alias_table[i]->value);
            free(alias_table[i]);

            for (int j = i; j < alias_count - 1; j++) {
                alias_table[j] = alias_table[j + 1];
            }

            alias_count--;
            return EXIT_SUCCESS;
        }
    }
    LOG_ERROR("Alias '%s' not found\n", name);
    return EXIT_FAILURE;
}


char *get_alias(char *name) {
    for (int i = 0; i < alias_count; i++) {
        if (strcmp(alias_table[i]->name, name) == 0) {
            return alias_table[i]->value;
        }
    }
    return NULL;
}

char *resolve_alias(char *input) {
    char* original_input = COPY(input);
    char **tokens = tokenize(input, " ");
    char *alias_value = get_alias(tokens[0]);
    
    if (alias_value) {
        char *resolved_command = COPY(alias_value);
        for (int i = 1; tokens[i] != NULL; i++) {
            strcat(resolved_command, " ");
            strcat(resolved_command, tokens[i]);
        }
        return resolved_command;
    }
    return original_input;
}
