#include "../1/library.h"
#include "lib2.h"
#include <stdio.h>
#include <math.h>
#include <unistd.h>

/*This function calculates the primality of a number*/
int is_prime(int num){
    int i;
    if(num <= 1){
        return 0;
    }
    
    for(i = 2; i <= sqrt(num); i++){
        if(num % i == 0){
            return 0;
        }
    }
    return 1;
}

/*This function is the worker. It takes as input an 
integer array, and tells if it's content is prime or not*/
void* worker(void* arg){
    int result;
    thread_infoT* thread = (thread_infoT*) arg;
        
    while(1){
        mysem_down(thread->s2);
        if (thread->terminate) break;

        result = is_prime(thread->number_to_check); 
        if(result){
            printf("The %d is Prime: %d\n",thread->number_to_check, result);
        }
        else{
            printf("The %d is Not Prime: %d\n", thread->number_to_check, result);
        }

        mysem_up(thread->s1);
    }
    
    return NULL;
}