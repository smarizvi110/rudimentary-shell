#include "utils.h"

#include <stdlib.h>
#include <stdlib.h>
#include <ctype.h> 
#include <string.h>

char **tokenize(char *input, const char *delim) {
    char **tokens = malloc(MAX_STRING_LENGTH * sizeof(char *));
    int index = 0;

    while (*input && isspace((unsigned char)*input)) {
        input++;
    }

    char *token = strtok(input, delim);
    while (token != NULL) {
        while (*token && isspace((unsigned char)*token)) {
            token++;
        }

        if (*token) {
            tokens[index++] = COPY(token);
        }

        token = strtok(NULL, delim);
    }
    tokens[index] = NULL;
    return tokens;
}

char **chain_tokenizer(const char *input) {
    char **tokens = NULL;
    int token_count = 0;

    char buffer[MAX_STRING_LENGTH] = {0};
    int buffer_index = 0;
    int in_command = 0;

    for (int i = 0; input[i] != '\0'; i++) {
        if ((input[i] == '&' && input[i + 1] == '&') || 
            (input[i] == '|' && input[i + 1] == '|') ||
            input[i] == ';') {

            if (buffer_index > 0) {
                buffer[buffer_index] = '\0';
                tokens = realloc(tokens, sizeof(char *) * (token_count + 1));
                tokens[token_count] = strdup(buffer);
                token_count++;
                buffer_index = 0;
            }

            if (input[i] == ';') {
                tokens = realloc(tokens, sizeof(char *) * (token_count + 1));
                tokens[token_count] = strdup(";");
                token_count++;
            } else {
                char operator[3] = {input[i], input[i + 1], '\0'};
                tokens = realloc(tokens, sizeof(char *) * (token_count + 1));
                tokens[token_count] = strdup(operator);
                token_count++;
                i++;
            }

            in_command = 0;

        } else if (input[i] == ' ' && buffer_index > 0 && in_command) {
            buffer[buffer_index++] = ' ';
        } else if (input[i] != ' ') {
            buffer[buffer_index++] = input[i];
            in_command = 1;
        } else if (input[i] == ' ' && buffer_index == 0) {
            continue;
        }
    }

    if (buffer_index > 0) {
        buffer[buffer_index] = '\0';
        tokens = realloc(tokens, sizeof(char *) * (token_count + 1));
        tokens[token_count] = strdup(buffer);
        token_count++;
    }

    tokens = realloc(tokens, sizeof(char *) * (token_count + 1));
    tokens[token_count] = NULL;

    return tokens;
}

char **pipe_tokenizer(const char *input) {
    char **tokens = NULL;
    int token_count = 0;

    char buffer[MAX_STRING_LENGTH] = {0};
    int buffer_index = 0;
    int in_command = 0;

    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] == '|') {
            if (buffer_index > 0) {
                buffer[buffer_index] = '\0';
                tokens = realloc(tokens, sizeof(char *) * (token_count + 1));
                tokens[token_count] = strdup(buffer);
                token_count++;
                buffer_index = 0;
            }

            in_command = 0;

        } else if (input[i] == ' ' && buffer_index > 0 && in_command) {
            buffer[buffer_index++] = ' ';
        } else if (input[i] != ' ') {
            buffer[buffer_index++] = input[i];
            in_command = 1;
        } else if (input[i] == ' ' && buffer_index == 0) {
            continue;
        }
    }

    if (buffer_index > 0) {
        buffer[buffer_index] = '\0';
        tokens = realloc(tokens, sizeof(char *) * (token_count + 1));
        tokens[token_count] = strdup(buffer);
        token_count++;
    }

    tokens = realloc(tokens, sizeof(char *) * (token_count + 1));
    tokens[token_count] = NULL;

    return tokens;
}
