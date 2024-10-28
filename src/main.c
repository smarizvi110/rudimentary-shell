/**
 * @file main.c
 * @author S. M. Aqdas Rizvi (s.muhammadaqdasrizvi@gmail.com)
 * @brief This is the main file for the shell. It contains the main function that runs the shell.
 * @version 0.1
 * @date 2024-09-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "utils.h"
#include "log.h"

#include <stdlib.h>
#include <stdio.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "tokenize.h" 
#include "execute.h"

void execute_chained_commands(char *input) {
    char **tokens = chain_tokenizer(input);
    
    int last_status = -1;
    int i = 0;

    int is_true_false_command(char *cmd) {
        if (strcmp(cmd, "true") == 0) {
            return 0;
        } else if (strcmp(cmd, "false") == 0) {
            return 1; 
        }
        return -1;
    }

    if (tokens[i] != NULL) {
        int status = is_true_false_command(tokens[i]);
        if (status == -1) {
            last_status = execute_command(tokens[i]);
        } else {
            last_status = status;
        }
        i++;
    }

    while (tokens[i] != NULL) {
        char *operator = tokens[i++];

        if (tokens[i] == NULL) {
            break;
        }

        if (strcmp(operator, "&&") == 0) {
            if (last_status == 0) {
                int status = is_true_false_command(tokens[i]);
                if (status == -1) {
                    last_status = execute_command(tokens[i]);
                } else {
                    last_status = status;
                }
            } else {
                continue;
            }
        }
        else if (strcmp(operator, "||") == 0) {
            if (last_status != 0) {
                int status = is_true_false_command(tokens[i]);
                if (status == -1) {
                    last_status = execute_command(tokens[i]);
                } else {
                    last_status = status;
                }
            } else {
                continue;
            }
        }
        else if (strcmp(operator, ";") == 0) {
            int status = is_true_false_command(tokens[i]);
            if (status == -1) {
                last_status = execute_command(tokens[i]);
            } else {
                last_status = status;
            }
        }

        i++;
    }

    for (int j = 0; tokens[j] != NULL; j++) {
        free(tokens[j]);
    }
    free(tokens);
}


void parse_chaining(char *input) {
    if ((strstr(input, " |") || strstr(input, "| ")) && !strstr(input, "||")) {
        char **piped_commands = pipe_tokenizer(input);
        handle_piping(piped_commands);
    } else {
        execute_chained_commands(input);
    }
}

void interactive_mode() {
    char *input;
    while (420 /*blaze it*/) { 
        input = readline("basic_shell> ");
        if (input == NULL) {
            LOG_PRINT("\n");
            break;
        }
        if (strlen(input) > 0) {
            add_history(input);
            parse_chaining(input);
        }
        free(input);
    }
}

void script_mode(char *script_file) {
    FILE *file = fopen(script_file, "r");
    if (file == NULL) {
        LOG_ERROR("Unable to open script file: %s\n", script_file);
        exit(EXIT_FAILURE);
    }
    
    char line[MAX_STRING_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = 0;
        
        if (strlen(line) > 0) {
            parse_chaining(line);
        }
    }
    
    fclose(file);
}

/**
 * @brief This is the main function for the shell.
 * 
 * @return int 
 */
int main(int argc, char** argv)
{
    // LOG_PRINT("Hello World!\n");
    // LOG_DEBUG("A debug statement\n");
    // LOG_ERROR("An error statement\n");

    if (argc == 1) {
        interactive_mode();
    } else if (argc == 2) {
        script_mode(argv[1]);
    } else {
        LOG_ERROR("Usage: %s [script_file]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}
