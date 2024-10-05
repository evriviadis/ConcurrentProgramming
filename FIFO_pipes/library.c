#include "library.h"
#include <stdio.h>
#include <pthread.h>

// creates and opens a pipe for r/w | returns the pipe id
int pipe_open(int size) {
    
};

// writes 1 byte to the pipe | returns -1 if there is no
// opened pipe with that id, otherwise 1
int pipe_write(int p, char c) {

};

// closes the pipe for writing | returns -1 if there is no
// opened pipe with that id, otherwise 1
int pipe_writeDone(int p) {

};

// reads and removes 1 byte from the pipe | returns 1 for 
// success, 0 if pipe is empty or closed, -1 if there is 
// no opened pipe with that id
int pipe_read(int p, char *c) {

};
