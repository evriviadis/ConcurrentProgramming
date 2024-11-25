#ifndef LIBRARY_H
#define LIBRARY_H

#include "../1/library.h"
#include <pthread.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
    pthread_t thread_id;
    mysem_t *s1, *s2;
    int number_to_check;
    int terminate; // takes value 1 to destroy the thread
} thread_infoT;

extern int is_prime(int num);
extern void* worker(void* arg);
#endif