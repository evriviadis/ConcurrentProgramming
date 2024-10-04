#include "library.h"
#include <stdio.h>
#include <pthread.h>

void * function(void* arg){
    printf("hello there\n");

    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t p1, p2;

    pthread_create(&p1, NULL, function, NULL);
    pthread_create(&p2, NULL, function, NULL);

    pthread_join(p1,NULL);
    pthread_join(p2,NULL);


    printf("this is from main\n");

    return 0;
}