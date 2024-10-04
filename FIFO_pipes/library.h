#ifndef LIBRARY_H
#define LIBRARY_H


typedef struct {
    int size;
    int id;
    int write_edge;
    int read_edge;
} pipeT;





int pipe_open(int size);
int pipe_write(int p, char c);
int pipe_writeDone(int p);
int pipe_read(int p, char *c);

#endif