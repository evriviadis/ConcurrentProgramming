#ifndef LIBRARY_HQQ
#define LIBRARY_HQQ

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

// Semaphore type
typedef struct {
    int sem_id;
    int init;
    pthread_mutex_t mutex;
} mysem_t;

extern int counter;

extern int mysem_init(mysem_t *s, int n);
extern int mysem_down(mysem_t *s);
extern int mysem_up(mysem_t *s);
extern int mysem_destroy(mysem_t *s);
#endif