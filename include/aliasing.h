#ifndef ALIASING_H
#define ALIASING_H

typedef struct Alias {
    char *name;
    char *value;
} Alias;

int set_alias(char *name, char *value);
int remove_alias(char *name);
char *get_alias(char *name);
char *resolve_alias(char *input);

#endif