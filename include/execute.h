#ifndef EXECUTE_H
#define EXECUTE_H

#include "utils.h"
#include "log.h"
#include "tokenize.h" 
#include "aliasing.h"

int execute_command(char *input);
void handle_piping(char **commands);

#endif
