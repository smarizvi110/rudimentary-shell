#include "utils.h"
#include "log.h"

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

#include "tokenize.h" 
#include "builtin.h"
#include "aliasing.h"

int execute_command(char *input) {
    char *resolved_input = resolve_alias(input);
    char **args = tokenize(resolved_input, " ");

    if (is_builtin(args)) {
        return execute_builtin(args);
    }

    pid_t pid = fork();
    if (pid == 0) {
        execvp(args[0], args);
        LOG_ERROR("execvp failed\n");
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        LOG_ERROR("fork failed\n");
        return EXIT_FAILURE;
    } else {
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        } else {
            return EXIT_FAILURE;
        }
    }
}

void handle_piping(char **commands) {
    int pipefd[2];
    int fd_in = STDIN_FD;
    pid_t pid;

    for (int i = 0; commands[i] != NULL; i++) {
        char *resolved_command = resolve_alias(commands[i]);
        
        char **args = tokenize(resolved_command, " ");

        if (commands[i + 1] != NULL) {
            if (pipe(pipefd) == -1) {
                LOG_ERROR("pipe failed\n");
                exit(EXIT_FAILURE);
            }
        }

        pid = fork();
        if (pid == 0) {
            if (fd_in != STDIN_FD) {
                if (dup2(fd_in, STDIN_FD) == -1) {
                    LOG_ERROR("dup2 for input redirection failed\n");
                    exit(EXIT_FAILURE);
                }
                close(fd_in);
            }

            if (commands[i + 1] != NULL) {
                if (dup2(pipefd[PIPE_WRITE_END], STDOUT_FD) == -1) {
                    LOG_ERROR("dup2 for output redirection failed\n");
                    exit(EXIT_FAILURE);
                }
            }

            close(pipefd[PIPE_READ_END]); 
            close(pipefd[PIPE_WRITE_END]);

            if (is_builtin(args)) {
                execute_builtin(args);
            } else {
                if (execvp(args[0], args) == -1) {
                    LOG_ERROR("execvp failed for command: %s\n", args[0]);
                    exit(EXIT_FAILURE);
                }
            }
            exit(EXIT_SUCCESS);
        } else if (pid < 0) {
            LOG_ERROR("fork failed\n");
            exit(EXIT_FAILURE);
        }

        if (commands[i + 1] != NULL) {
            close(pipefd[PIPE_WRITE_END]);
        }

        waitpid(pid, NULL, 0);

        if (fd_in != STDIN_FD) {
            close(fd_in);
        }

        if (commands[i + 1] != NULL) {
            fd_in = pipefd[PIPE_READ_END];
        }
    }
}
