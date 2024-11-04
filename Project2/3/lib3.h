#ifndef LIB3H
#define LIB3H

#include "../1/library.h"
#include <pthread.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
    int N, numRedCars, numBlueCars;
    int i;
    mysem_t *s;
} carInfo_t;

typedef struct {
    pthread_t threadId;
} threadInfoT;

extern carInfo_t info;


extern int enterBridge();
extern int exitBridge();

#endif