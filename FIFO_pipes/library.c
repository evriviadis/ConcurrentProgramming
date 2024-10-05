#include "library.h"
#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>

// creates and opens a pipe for r/w | returns the pipe id
int pipe_open(int size) {
    int pipefds[2];
    
    //open and check pipe
    if (pipe(pipefds) == -1){
      perror("pipe");
      return -1;
    }

    //set buffer to pipe
    if (fcntl(pipefds[0], F_SETPIPE_SZ, size) == -1){
      perror("size buffer");
      return -1;
    }
    
    return pipefds[0];
};

// writes 1 byte to the pipe | returns -1 if there is no
// opened pipe with that id, otherwise 1
int pipe_write(int p, char c) {
    if (p < 0) return -1; // Invalid pipe ID
    
    int result = write(p, &c, 1); // &c is a pointer to the byte

    if (result == -1) {
        perror("write to pipe failed");
        return -1; // Return -1 on failure
    }

    return 1; // Return 1 on success
};

// closes the pipe for writing | returns -1 if there is no
// opened pipe with that id, otherwise 1
int pipe_writeDone(int p) {
    if (p < 0) return -1;

    close(p);
    return 1;
};

// reads and removes 1 byte from the pipe | returns 1 for 
// success, 0 if pipe is empty or closed, -1 if there is 
// no opened pipe with that id
int pipe_read(int p, char *c) {
    if (p < 0) return -1;

    int result = read(p, c, 1); // 'c' is a pointer to store the byte

    if (result) return 1;
    else return 0;
};

// This function uses read and check if it opperates properly
int my_read(int fd, void *buffer, int size, int *left) {
   int res, read_already=0;

   do {
      res = read(fd, buffer+(read_already*sizeof(char)), size-read_already);
      if (res == -1)
         return(-1);
      else {
         if(res == 0){
            *left = size-read_already;
            return(read_already);
         }
      }
      read_already += res;
   } while(read_already < size);
   return(read_already); 
};