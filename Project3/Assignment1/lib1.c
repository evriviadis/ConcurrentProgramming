/* In this file there is an implementation of binary semaphores using monitor functionality */
#include "lib1.h"

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

    // Initiallization of mtx and condition for monitor
    if(pthread_mutex_init(&s->mtx, NULL) == -1){
        perror("Mutex init");
        return(0);
    }

    if(pthread_cond_init(&s->queue, NULL) == -1){
        perror("Cond init");
        return(0);
    }

    // Init of semaphore
    s->val = n;
    s->init = 1;
    return(1);
}

int mysem_down(mysem_t *s){    
    // Check sem initiallization            ------- CHECK MUTUAL EXCLUSION
    if(s->init != 1){
        printf("Sem not initiallized yet\n");
        return(-1);
    }

    pthread_mutex_lock(&s->mtx);

    while(s->val == 0){                          //check if it needs while - not correct
        // printf("thread is going to wait\n");
        pthread_cond_wait(&s->queue, &s->mtx);
    }

    s->val--;
    pthread_mutex_unlock(&s->mtx);
    
    return(1);
}

int mysem_up(mysem_t *s){
    pthread_mutex_lock(&s->mtx);

    if(s->val == 1){
        printf("ERROR: Vlaue is already 1 exiting..\n");
        pthread_mutex_unlock(&s->mtx);
        return(0);
    }

    s->val++;
    // printf("Going to wake up the first one ");
    pthread_cond_signal(&s->queue);
    pthread_mutex_unlock(&s->mtx);
    return(1);
}

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