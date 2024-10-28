#ifndef TOKENIZE_H
#define TOKENIZE_H

#include "utils.h" 
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

char **tokenize(char *input, const char *delim);
char **chain_tokenizer(const char *input);
char **pipe_tokenizer(const char *input);

#endif
