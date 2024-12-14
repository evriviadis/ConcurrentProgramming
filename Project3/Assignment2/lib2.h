#ifndef LIBRARY_H
#define LIBRARY_H

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t work_ready, workers_available;
    // int resource_flag, workers_available;
    // pthread_cond_t *workers_cond;
} monitor_t;

typedef struct {
    pthread_t thread_id;
    int number_to_check;
    // int terminate;
} thread_infoT;

monitor_t monitor;
int N;

extern int is_prime(int num);
extern void* worker(void* arg);

extern void monitor_init();
extern void monitor_wait();
extern void monitor_assign(int number);
extern void monitor_destroy();

#endif