#ifndef BUILTIN_H
#define BUILTIN_H

#include "utils.h"
#include "log.h"

int is_builtin(char **args);
int execute_builtin(char **args);

#endif