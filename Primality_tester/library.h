#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <stdlib.h>

typedef struct {
    pthread_t thread_id;
    int available;          // changes in 'worker' to notify 'main'
    int given_work;         // changes in 'main' and 'worker' to notify 'worker'
    int number_to_check;
    int terminate;          // takes value 1 to destroy the thread
} thread_infoT;

extern int is_prime(int num);
extern int my_read(int fd, void *buffer, int size, int *left);
extern void* worker(void* arg);