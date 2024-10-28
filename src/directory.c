#include "utils.h"
#include "log.h"

#include <stdlib.h>
#include <stdio.h>

#define MAX_STACK_SIZE 1024

char *dir_stack[MAX_STACK_SIZE];
int stack_top = -1;

int push_directory(char *path) {
    if (stack_top < MAX_STACK_SIZE - 1) {
        dir_stack[++stack_top] = COPY(path);
        return EXIT_SUCCESS;
    } else {
        LOG_ERROR("Directory stack overflow\n");
        return EXIT_FAILURE;
    }
    return EXIT_FAILURE;
}

char *pop_directory() {
    if (stack_top >= 0) {
        return dir_stack[stack_top--];
    } else {
        LOG_ERROR("Directory stack underflow\n");
        return NULL;
    }
    return NULL;
}


void print_directory_stack() {
    if (stack_top == -1) {
        LOG_PRINT("Directory stack is empty\n");
        return;
    }
    for (int i = stack_top; i >= 0; i--) {
        LOG_PRINT("%s ", dir_stack[i]);
    }
    LOG_PRINT("\n");
}
