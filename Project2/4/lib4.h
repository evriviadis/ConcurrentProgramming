#ifndef LIB3H
#define LIB3H

#include "../1/library.h"
#include <pthread.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_SEATS 3

typedef struct {
    pthread_t *threadId;
} passenger_t;





#endif