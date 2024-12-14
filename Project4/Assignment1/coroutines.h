#ifndef COROUTINES_H

#include <ucontext.h>
#include <stdlib.h>
#include <stdio.h>

#define STACK_SIZE 2048

typedef struct co {
    ucontext_t context;
    void *stack;
    int finished;
} co_t;

// with static current_co and main_co can't be accessed by other files other than coroutines.c
static co_t *current_co;
static co_t main_co;

extern int mycoroutines_init(co_t *main);
extern int mycoroutines_create(co_t *co, void (body)(void *), void *arg);
extern int mycoroutines_switchto(co_t *co);
extern int mycoroutines_destroy(co_t *co);

#endif