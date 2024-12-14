#ifndef LIBRARY1_H
#define LBIRARY1_H

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

/*BINARY SEMAPHORE*/
typedef struct{
    int val;
    int init;
    pthread_mutex_t mtx;
    pthread_cond_t queue;
} mysem_t

/*FUNCTIONS*/
extern int mysem_init(mysem_t *s, int n);
extern int mysem_down(mysem_t *s);
extern int mysem_up(mysem_t *s);
extern int mysem_destroy(mysem_t *s);
#endif
