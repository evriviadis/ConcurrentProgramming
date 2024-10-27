#ifndef LIBRARY_H
#define LIBRARY_H

#define PIPE_SIZE 64
#define NUM_OF_PIPES 2

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

extern pipeT** pipebase;
extern int id_counter;

typedef struct{
    char* file_name;
    pipeT* pipe_write;
    pipeT* pipe_read;
}thread_argsT;

extern int pipe_open(int size);
extern int pipe_write(int p, char c);
extern int pipe_writeDone(int p);
extern int pipe_read(int p, char *c);
extern int my_read(int fd, void *buffer, int size, int *left);
extern int my_write(int *fd, void *buffer, int size);

#endif