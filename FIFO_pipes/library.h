#ifndef LIBRARY_H
#define LIBRARY_H

#define F_GETPIPE_SZ  1031  // Get the size of the pipe buffer
#define F_SETPIPE_SZ  1032  // Set the size of the pipe buffer

typedef struct {
    int size, id, write_edge, read_edge;
} pipeT;

extern int pipe_open(int size);
extern int pipe_write(int p, char c);
extern int pipe_writeDone(int p);
extern int pipe_read(int p, char *c);
extern int my_read(int fd, void *buffer, int size, int *left);

#endif