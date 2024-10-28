#include "utils.h"
#include "log.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "directory.h"
#include "aliasing.h" 

int is_builtin(char **args) {
    const char *builtins[] = {"exit", "pwd", "cd", "pushd", "popd", "dirs", "alias", "unalias", "echo"};
    int num_builtins = sizeof(builtins) / sizeof(char *);
    
    for (int i = 0; i < num_builtins; i++) {
        if (strcmp(args[0], builtins[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int execute_builtin(char **args) {
    if (strcmp(args[0], "exit") == 0) {
        exit(EXIT_SUCCESS);
    } else if (strcmp(args[0], "pwd") == 0) {
        char cwd[MAX_STRING_LENGTH];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            LOG_PRINT("%s\n", cwd);
            return EXIT_SUCCESS;
        } else {
            LOG_ERROR("pwd failed: could not get current directory\n");
            return EXIT_FAILURE;
        }
    } else if (strcmp(args[0], "cd") == 0) {
        if (chdir(args[1]) != 0) {
            LOG_ERROR("cd failed: could not change directory to %s\n", args[1]);
            return EXIT_FAILURE;
        }
        LOG_PRINT("Changed directory to %s\n", args[1]);
        return EXIT_SUCCESS;
    } else if (strcmp(args[0], "pushd") == 0) {
        char cwd[MAX_STRING_LENGTH];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            if (chdir(args[1]) != 0) {
                LOG_ERROR("pushd failed: could not change directory to %s\n", args[1]);
                return EXIT_FAILURE;
            } else {
                if (push_directory(cwd) == EXIT_FAILURE) {
                    return EXIT_FAILURE;
                }
                print_directory_stack();
                return EXIT_SUCCESS;
            }
        }
        LOG_ERROR("getcwd failed\n");
        return EXIT_FAILURE;
    } else if (strcmp(args[0], "popd") == 0) {
        char *prev_dir = pop_directory();
        if (prev_dir) {
            if (chdir(prev_dir) != 0) {
                LOG_ERROR("popd failed: could not change directory to %s\n", prev_dir);
                free(prev_dir);
                return EXIT_FAILURE;
            } else {
                print_directory_stack();
                free(prev_dir);
                return EXIT_SUCCESS;
            }
        }
        LOG_ERROR("No directory to pop\n");
        return EXIT_FAILURE;
    } else if (strcmp(args[0], "dirs") == 0) {
        print_directory_stack();
        return EXIT_SUCCESS;
    } else if (strcmp(args[0], "echo") == 0) {
        for (int i = 1; args[i] != NULL; i++) {
            LOG_PRINT("%s", args[i]);
            if (args[i + 1] != NULL) {
                LOG_PRINT(" ");
            }
        }
        LOG_PRINT("\n"); 
        return EXIT_SUCCESS;
    } else if (strcmp(args[0], "alias") == 0) {
        if (args[1] != NULL && args[2] != NULL) {
            if (set_alias(args[1], args[2]) == EXIT_FAILURE) {
                return EXIT_FAILURE;
            }
            return EXIT_SUCCESS;
        } else {
            LOG_ERROR("alias failed: No name or value specified\n");
            return EXIT_FAILURE;
        }
    } else if (strcmp(args[0], "unalias") == 0) {
        if (args[1] != NULL) {
            if (remove_alias(args[1]) == EXIT_FAILURE) {
                return EXIT_FAILURE;
            }
            return EXIT_SUCCESS;
        } else {
            LOG_ERROR("unalias failed: No name specified\n");
            return EXIT_FAILURE; 
        }
    }
    return EXIT_FAILURE;
}
