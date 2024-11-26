#ifndef LIBRARY_H
#define LIBRARY_H

#include "../1/library.h"
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond1, cond2;
    int cond1_val, cond2_val;
} monitor_t;

typedef struct {
    // mysem_t *s1, *s2;
    monitor_t *m;
    pthread_t thread_id;
    int number_to_check, terminate;
} thread_infoT;

extern int is_prime(int num);
extern void* worker(void* arg);

extern void monitor_init(Monitor *mon);
extern void monitor_wait(Monitor *mon);
extern void monitor_signal(Monitor *mon);
extern void monitor_destroy(Monitor *mon);

#endif