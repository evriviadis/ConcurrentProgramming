#include "library.h"

pipeT** pipebase = NULL;
int id_counter = 0 ;
co_t coroutine1, coroutine2;

/*This function opens a pipe- Allocates dynamically
and initializes a ring buffer. Returns pipe id.*/
int pipe_open(int size) {
   pipeT* p = (pipeT *) malloc(sizeof(pipeT));
   if (p == NULL) 
      return(-1);

   p->id = id_counter;
   p->read_edge = 0;
   p->write_edge = 0;
   p->read_open = 1;
   p->write_open = 1; 
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

   return(p->id); 
};

/*This function writes 1 byte on the pipe. 
Returns 1 for success, -1 if the pipe isnt open for writing*/
int pipe_write(int p, char c){
   pipeT* pipe = pipebase[p];
   //check if the pipe is open for writing
   if ((pipe == NULL) || (!pipe->write_open))
      return(-1); 

   pipe->buffer[pipe->write_edge] = c;
   //printf("1| buffer from pipe write: %c, write Edge inside pipewrite: %d\n",pipebase[p]->buffer[pipe->write_edge], pipe->write_edge);
   
   //when the pipe is full we go again to the start of the buffer
   if (pipe->write_edge == pipe->size-1){
      pipe->cyclesWrite++;
      pipe->write_edge = 0;
   }else{
      pipe->write_edge++;
   }

   return(1);
};

/*This function closes a pipe for writing. 
Returns 1 for success, -1 if the pipe isnt open for writing*/
int pipe_writeDone(int p){
   pipeT* pipe = pipebase[p];
   //check if the pipe doesnt exist
   if(pipe == NULL)                    
      return(-1);

    pipe->write_open = 0; 
    return(1);
};

/*This function reads and removes 1 byte from the pipe.
Returns 1 for success, 0 for empty or closed pipe, -1 otherwise.*/
int pipe_read(int p, char *c) {
   pipeT* pipe = pipebase[p];

   if ((pipe == NULL) || !(pipe->read_open)) { //pipe colsed or doesent exist
      return(-1);
   } else if((!pipe->write_open) && (pipe->read_edge == pipe->write_edge) &&
                   (pipe->cyclesRead == pipe->cyclesWrite)) { //pipe closed
      return(0);
   }  

   //printf("readEdge: %d\n", pipe->read_edge);
   //printf("buffer: %c\n", pipe->buffer[pipe->read_edge]);
   *c = pipe->buffer[pipe->read_edge];
   if ((pipe->read_edge) == (pipe->size) - 1) {
      pipe->cyclesRead++;
      pipe->read_edge = 0;
   } else {
      pipe->read_edge++;
   }

   return(1);
}; 

/*This function uses read and check if it opperates properly*/
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

/*This function uses write and checks if it operates properly.*/
int my_write(int *fd, void *buffer, int size){
	int res, write_already=0;

	do{
		res = write(*fd, buffer+(write_already*sizeof(buffer)), size-write_already);
		if(res == -1){
			return(-1);
      }else{ 
      if(res == 0)
			return(0);
      }
		write_already += res;
	}while(write_already != size);

	return(1);
};