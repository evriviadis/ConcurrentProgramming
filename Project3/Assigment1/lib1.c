/*In this file there is an implementation of binary semaphores using monitor functionality in C.*/
#include "lib1.h"
#include <pthread.h>
#include <stdio.h>

/*Initialize Semaphore*/
int mysem_init(mysem_t *s, int n){
    // Check arguement's value
    if(n!=0 && n!=1){
        printf("Wrong n value\n");
        return(0);
    }

    // Check sem initiallization        ------- CHECK MUTUAL EXCLUSION
    if(s->init != 0){
        printf("Sem already initiallized\n");
        return(0);
    }

    // Initiallization of mtx and cond for monitor
    if(pthread_mutex_init(&s->mtx, NULL) == -1){
        perror("Mutex init");
        return(0);
    }

    if(pthread_cond_init(&s->queue, NULL) == -1){
        perror("Cond init");
        return(0);
    }

    s->val = n;
    s->init = 1;
    return(1)
}

/*Semaphore decrease*/
int mysem_down(mysem_t *s){    
    // Check sem initiallization            ------- CHECK MUTUAL EXCLUSION
    if(s->init != 1){
        printf("Sem not initiallized yet\n");
        return(-1);
    }

    pthread_mutex_lock(&s->mtx);

    if(s->val == 0){                          //check if it needs while
        printf("thread is going to wait\n");
        pthread_cond_wait(&s->queue, &s->mtx);
    }

    s->val--;
    pthread_mutex_unlock(&s->mtx);
    
    return(1);
}

/*Semaphore Increase*/
int mysem_up(mysem_t *s){
    pthread_mutex_lock(&s->mtx);

    if(s->val == 1){
        printf("Vlaue is already 1 exiting..\n");
        pthread_mutex_unlock(&s->mtx);
        return(0);
    }

    s->val++;
    printf("Going to wake up the first one ");
    pthread_cond_signal(&s->queue);
    pthread_mutex_unlock(&s->mtx);
    return(1);
}

/*Semaphore Destroy*/
int mysem_destroy(mysem_t *s){

    // Check initialization
    if(!s->init){
        printf("Semaphore already destroyed or initialized\n");
        return(-1);
    }

    // Semaphore destroy
    s->init = 0;
    pthread_mutex_destroy(&s->mtx);
    pthread_cond_destroy(&s->queue);
    return(1);
}