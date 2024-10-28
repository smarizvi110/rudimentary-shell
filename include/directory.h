#ifndef DIRECTORY_H
#define DIRECTORY_H

#define MAX_STACK_SIZE 1024

extern int stack_top;
extern char *dir_stack[MAX_STACK_SIZE];

int push_directory(const char *path);
char * pop_directory(void);
void print_directory_stack(void);

#endif
