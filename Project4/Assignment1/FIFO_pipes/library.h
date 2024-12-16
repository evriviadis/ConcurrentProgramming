#ifndef LIBRARY_H
#define LIBRARY_H

#define PIPE_SIZE 64
#define NUM_OF_PIPES 2

#include "../coroutines.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>
#include <string.h>

/*PIPE*/
typedef struct {
    int id;
    int size;
    int write_edge;
    int read_edge;
    int write_open;
    int read_open;
    int cyclesWrite;
    int cyclesRead;
    char* buffer;
} pipeT;

/*THREAD'S ARGUEMENTS*/
typedef struct{
    char* file_name;
    pipeT* pipe_write;
    pipeT* pipe_read;
    int finished;
}thread_argsT;

/*GLOBAL VARIABLES*/
extern co_t coroutine1, coroutine2;
extern pipeT** pipebase;
extern int id_counter;

/*FUNCTION'S PROTOTYPES*/
extern int pipe_open(int size);
extern int pipe_write(int p, char c);
extern int pipe_writeDone(int p);
extern int pipe_read(int p, char *c);
extern int my_read(int fd, void *buffer, int size, int *left);
extern int my_write(int *fd, void *buffer, int size);
#endif