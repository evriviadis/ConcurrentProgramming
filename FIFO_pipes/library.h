#ifndef LIBRARY_H
#define LIBRARY_H

typedef struct {
    int size, id, write_edge, read_edge;
} pipeT;

extern int pipe_open(int size);
extern int pipe_write(int p, char c);
extern int pipe_writeDone(int p);
extern int pipe_read(int p, char *c);

#endif