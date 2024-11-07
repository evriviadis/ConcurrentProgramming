#ifndef LIB4H
#define LIB4H

#include "../1/library.h"
#include <pthread.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_SEATS 3

/*Train*/
typedef struct {
    pthread_t *threadId;
    mysem_t *trainSem, *blockTrain, *blockExit, *readyToExit, *endedTrip;
    int emptySeats, eof;
} train_t;

/*Passenger*/
typedef struct {
    pthread_t *threadId;
} passenger_t;

/*Function's Prototypes*/
extern train_t train;

extern int enter_train();
extern int exit_train();
extern void* trainFunc();

#endif