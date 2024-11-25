#ifndef LIBRARY_HQQ
#define LBIRARY_HQQ

#include <pthread.h>

typedef struct{
    int val;
    int init;
    pthread_mutex_t mtx;
    pthread_cond_t queue;
} mysem_t;

#endif