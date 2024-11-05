#ifndef LIB3H
#define LIB3H

#include "../1/library.h"
#include <pthread.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

typedef enum {
    BLUE = 0,
    RED = 1,
    NONE = 2
} color_t;

typedef struct {
    int N, numRedCars, numBlueCars;
    int i, passed, inBridge;
    color_t priority, flow;
    mysem_t **s;
} carInfo_t;

typedef struct {
    pthread_t threadId;
    color_t color;
    int semIndex;
} threadInfoT;

extern carInfo_t info;

extern int enterBridge(threadInfoT *thread);
extern int exitBridge(threadInfoT *thread);
extern int wannaEnter(threadInfoT *thread);

#endif