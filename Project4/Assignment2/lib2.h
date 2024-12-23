#ifndef COROUTINES_H
#define COROUTINES_H

#include <ucontext.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define STACK_SIZE 2048
#define TIME_QUANTUM 1
#define MIN_SLEEP_CHECK 100000

typedef enum status {
    READY = 3,
    SLEEP = 1,
    BLOCK = 2,
    FINISH = 4
} status_t;

typedef struct co {
    ucontext_t context;
    void *stack;
    int finished;
} co_t;

typedef struct mythr {
    struct mythr *next;
    co_t coroutine;
    int finished;
    status_t status;
    int id;
    int sleepTime;
    struct mythr *blocked;
} mythr_t;

typedef struct mysem {
    int value;
    mythr_t *blocked;
}mysem_t;


// with static current_co and main_co can't be accessed by other files other than coroutines.c
extern co_t *current_co;
extern co_t main_co;
extern mythr_t *current_thread;
extern mythr_t main_thread;
extern int idCounter;
extern int minSleepTime;
extern pthread_mutex_t lock;
extern mysem_t *s;


extern void print_chain();
extern void update_sleep();

extern int mycoroutines_init(co_t *main);
extern int mycoroutines_create(co_t *co, void (body)(void *), void *arg);
extern int mycoroutines_switchto(co_t *co);
extern int mycoroutines_destroy(co_t *co);

extern int mythreads_init();
extern int mythreads_create(mythr_t *thr, void (body)(void *), void *arg);
extern int mythreads_yield();
extern int mythreads_sleep(int secs);
extern int mythreads_join(mythr_t *thr);
extern int mythreads_destroy(mythr_t *thr);
extern int mythreads_sem_create(mysem_t *s, int val);
extern int mythreads_sem_down(mysem_t *s);
extern int mythreads_sem_up(mysem_t *s);
extern int mythreads_sem_destroy(mysem_t *s);


#endif