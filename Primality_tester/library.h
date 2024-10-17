#ifndef LIBRARY_H
#define LIBRARY_H

#include<pthread.h>

/*THREAD'S ARGUEMENTS*/
typedef struct {
    pthread_t thread_id;
    int available;          // changes in 'worker' to notify 'main'
    int given_work;         // changes in 'main' and 'worker' to notify 'worker'
    int number_to_check;
    int terminate;          // takes value 1 to destroy the thread
    int finished;
} thread_infoT;

/*FUNCTIONS' PROTOTYPES*/
extern int is_prime(int num);
extern int my_read(int fd, void *buffer, int size, int *left);
extern void* worker(void* arg);
#endif
