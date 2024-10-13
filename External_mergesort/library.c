#include "library.h"

int my_read(int fd, void *buffer, int size, int *left){
   int res, read_already=0;

    do{
        res = read(fd, buffer+(read_already*sizeof(char)), size-read_already);
        if (res == -1){

            return(-1);
        }else if(res == 0){
            *left = size-read_already;
            return(read_already);
        }

        read_already += res;
    }while(read_already < size);
    return(read_already); 
};