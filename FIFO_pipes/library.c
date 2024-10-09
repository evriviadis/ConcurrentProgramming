#include "library.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

pipeT** pipebase = NULL;
int id_counter = 0 ;

/*This function opens a pipe- Allocates dynamically a buffer that its end matches its start (ring buffer)
then the buffer is initiallized and ready to be used. pipeopen returns pipe id*/
int pipe_open(int size) {
   pipeT* p = (pipeT *) malloc(sizeof(pipeT));
   
   if (p == NULL) 
      return(-1);

   p->id = id_counter;
   p->read_edge = 0;
   p->write_edge = 0;
   p->read_open = 1;
   p->write_open = 1;      //both edges for reading and writing are open
   p->cyclesRead = 0;
   p->cyclesWrite = 0;
   p->size = size;
   p->buffer = (char*) malloc(size*sizeof(char));
   if(p->buffer == NULL)
      return(-1);
   else{
      pipebase[id_counter] = p;
      id_counter++;
   }

   return(p->id);      //is it correct? thelw na epistrefw id (thetikos akeraios)
};

// writes 1 byte to the pipe | returns -1 if there is no
// opened pipe with that id, otherwise 1

/*This function writes 1 byte on the pipe. Returns 1 for success, -1 if the pipe isnt open for writing*/
int pipe_write(int p, char c){
   pipeT* pipe = pipebase[p];
   //check if the pipe is open for writing
   if ((pipe == NULL) || (!pipe->write_open))
      return(-1); 

   //when the pipe is full we go again to the start - may have conflicts - check in threads!!!!!
   if (pipe->write_edge == pipe->size-1){
      pipe->write_edge = 0;
      pipe->cyclesWrite++;
   }else{
      pipe->write_edge++;
   }

   pipe->buffer[pipe->write_edge] = c;
   printf("2| buffer from pipe write: %c\n",pipebase[p]->buffer[pipe->write_edge]);
   ///////////////////////////EDO THA PREPEI NA MPEI SINTHIKI GIA TA PIPE WRITE KAI PIPE READ ////////////////////////////////////
   return(1);
};

/*This function closes a pipe for writing. Returns 1 for success, -1 if the pipe isnt open for writing*/
int pipe_writeDone(int p){
   pipeT* pipe = pipebase[p];
   //check if the pipe doesnt exist
   if(pipe == NULL)                    //check condition////
        return(-1);

    pipe->write_open = 0; 
    return(1);
};

// reads and removes 1 byte from the pipe | returns 1 for 
// success, 0 if pipe is empty or closed, -1 if there is 
// no opened pipe with that id
int pipe_read(int p, char *c) {
   pipeT* pipe = pipebase[p];

   if ((pipe == NULL) || !(pipe->read_open)) { //pipe colsed or doesent exist
      return(-1);
   } else if(!(pipe->write_open) && (pipe->read_edge == pipe->write_edge) && (pipe->cyclesRead == pipe->cyclesWrite)) { //pipe closed     /////CHECK THIS/////
      return(0);
   }  

   printf("readEdge: %d\n", pipe->read_edge);
   printf("buffer: %s\n", pipe->buffer);
   *c = pipe->buffer[pipe->read_edge];
   if ((pipe->read_edge) == (pipe->size) - 1) {
      pipe->read_edge = 0;
      pipe->cyclesRead++;
   } else {
      pipe->read_edge++;                    ////PROBLEM IN CONCURRENCY/////
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