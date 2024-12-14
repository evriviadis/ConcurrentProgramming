#include "coroutines.h"

// Initialization of the main coroutine
int mycoroutines_init(co_t *main) {
    if (!main) return -1;

    if (getcontext(&main->context) == -1) return -1;

    main->stack = NULL; // the main coroutine doesn't need excess stack
    main->finished = 0;
    current_co = main;
    return 0;
}

// Creation of a new coroutine
// 'body' is a pointer to a function to be executed by the coroutine
int mycoroutines_create(co_t *co, void (body)(void *), void *arg) {
    if (!co || !body) return -1;

    co->stack = (void*) malloc(STACK_SIZE);
    if (!co->stack) return -1;

    if (getcontext(&co->context) == -1) return -1;
    
    co->context.uc_stack.ss_sp = co->stack;
    co->context.uc_stack.ss_size = STACK_SIZE;
    co->context.uc_link = &main_co.context; // when coroutine finishes return to &main_co.context
    co->finished = 0;
    makecontext(&co->context, (void (*)(void))body, 1, arg);
    return 0;
}

int mycoroutines_switchto(co_t *co) {
    if (!co || co->finished) return -1;

    co_t *prev_co = current_co;
    current_co = co;
    if (swapcontext(&prev_co->context, &co->context) == -1) return -1;
    return 0;
}

int mycoroutines_destroy(co_t *co) {
    if (!co || co->finished) return -1;

    if (co->stack != NULL) free(co->stack);
    else printf("UNsuccess free \n");
    co->stack = NULL;
    co->finished = 1;
    printf("success destroy\n");
    return 0;
}