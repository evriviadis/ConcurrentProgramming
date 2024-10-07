#ifndef LIBRARY_H
#define LIBRARY_H

/* 
#define F_GETPIPE_SZ  1031  // Get the size of the pipe buffer
#define F_SETPIPE_SZ  1032  // Set the size of the pipe buffer 
*/

#define PIPE_SIZE 64
#define NUM_OF_PIPES 2

/* 
typedef struct {
    int size, id, write_edge, read_edge;
} pipeT;
 */

typedef struct {
    int size;
    int write_edge;
    int read_edge;
    int write_open;
    int read_open;
    char* buffer;
} pipeT;

typedef struct{
    char* file_name;
    pipeT* pipe_write;
    pipeT* pipe_read;
}thread_argsT;

/*
extern int pipe_open(int size);
extern int pipe_write(int p, char c);
extern int pipe_writeDone(int p);
extern int pipe_read(int p, char *c);
extern int my_read(int fd, void *buffer, int size, int *left);
*/

extern pipeT* pipe_open(int size);
extern int pipe_write(pipeT* p, char c);
extern int pipe_writeDone(pipeT* p);
extern int pipe_read(pipeT* p, char *c);
extern int my_read(int fd, void *buffer, int size, int *left);

#endif