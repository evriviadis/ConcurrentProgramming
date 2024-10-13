#include "library.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/*This function uses read and check if it opperates properly*/
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

int is_prime(int num){
    if(num <= 1){
        return 0;
    }
    
    for(int i = 2; i <= sqrt(num); i++){
        if(num % i == 0){
            return 0;
        }
    }
    return 1;
}

void* worker(void* arg){
    thread_infoT* thread = (thread_infoT*) arg;
    int result, number;

    while(1){

        if(thread->given_work){
            number = thread->number_to_check;
            
            result = is_prime(number);
            if(result){
                printf("The %d is Prime: %d\n", number, result);
            }
            else{
                printf("The %d is Not Prime: %d\n", number, result);
            }
            thread->given_work = 0;
            thread->available = 1;
        }else if(thread->terminate && !(thread->given_work)){
            //printf("terminate\n");
            break;
        }
    }
    return NULL;
}