#include "library.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

// creates and opens a pipe for r/w | returns the pipe id


/* 
///////OLD PIPE OPEN THN KRATAME GIA TON BAGGELAKI
int pipe_open(int size, pipeT* pipe) {
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
    
    pipe = (pipeT*) malloc(sizeof(pipeT));
    pipe->id = rand()
    pipe->size = 

    return pipefds[0];
};
*/



/*This function opens a pipe- Allocates dynamically a buffer that its end matches its start (ring buffer)
then the buffer is initiallized and ready to be used. pipeopen returns pipe id*/
pipeT* pipe_open(int size) {
   pipeT* p;

   p =(pipeT *) malloc(sizeof(pipeT));
   if (p == NULL) 
      return(NULL);

   p->read_edge = 0;
   p->write_edge = 0;
   p->read_open = 1;
   p->write_open = 1;      //both edges for reading and writing are open
   p->size = size;
   p->buffer = (char*) malloc(size*sizeof(char));
   if(p->buffer == NULL)
      return(NULL);

   return(p);      //is it correct? thelw na epistrefw id (thetikos akeraios)
};

// writes 1 byte to the pipe | returns -1 if there is no
// opened pipe with that id, otherwise 1

/* 
////OLD PIPE WRITE
int pipe_write(int p, char c) {
    if (p < 0) return -1; // Invalid pipe ID
    
    int result = write(p, &c, 1); // &c is a pointer to the byte

    if (result == -1) {
        perror("write to pipe failed");
        return -1; // Return -1 on failure
    }

    return 1; // Return 1 on success
};
 */

/*This function writes 1 byte on the pipe. Returns 1 for success, -1 if the pipe isnt open for writing*/
int pipe_write(pipeT* p, char c){
   //check if the pipe is open for writing
   if((p == NULL) || (!p->write_open))
      return(-1); 

   //when the pipe is full we go again to the start - may have conflicts - check in threads!!!!!
   if(p->write_edge == p->size-1)
      p->write_edge = 0;
   else
      p->write_edge++;

   p->buffer[p->write_edge] = c;
   ///////////////////////////EDO THA PREPEI NA MPEI SINTHIKI GIA TA PIPE WRITE KAI PIPE READ ////////////////////////////////////
   return(1);
};

// closes the pipe for writing | returns -1 if there is no
// opened pipe with that id, otherwise 1
/* 
int pipe_writeDone(int p) {
    if (p < 0) return -1;

    close(p);
    return 1;
}; */


/*This function closes a pipe for writing. Returns 1 for success, -1 if the pipe isnt open for writing*/
int pipe_writeDone(pipeT* p){
   //check if the pipe doesnt exist
   if(p == NULL)                    //check condition////
        return(-1);

    p->write_open = 0;
    return(1);
};

// reads and removes 1 byte from the pipe | returns 1 for 
// success, 0 if pipe is empty or closed, -1 if there is 
// no opened pipe with that id
 
int pipe_read(pipeT* p, char *c) {
    if((p == NULL) || !(p->read_open)){ //pipe colsed or doesent exist
        return(-1);
    }else if(!(p->write_open) && (p->read_edge == p->write_edge)){ //pipe closed     /////CHECK THIS/////
        return(0);
    }          

    *c = p->buffer[p->read_edge];
    if((p->read_edge) == (p->size) - 1){
        p->read_edge = 0;
    }else{
        p->read_edge ++;                    ////PROBLEM IN CONCURRENCY/////
    }

    return(1);
};

// This function uses read and check if it opperates properly
int my_read(int fd, void *buffer, int size, int *left) {
   int res, read_already=0;

   do {
      res = read(fd, buffer+(read_already*sizeof(char)), size-read_already);
      if (res == -1)
         return(-1);
      else {
         if (res == 0){
            *left = size-read_already;
            return(read_already);
         }
      }
      read_already += res;
   } while(read_already < size);
   return(read_already); 
};

