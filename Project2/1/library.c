#include "library.h"

int counter = 0;

/*THis function initializes a semaphore with n value.*/
int mysem_init(mysem_t *s, int n){
    if(n!=0 && n!=1){ //Check n value
        printf("Init failed..wrong n\n");
        return(0);
    }
    if(s->init != 0){ //Check if sem is already initialized
        printf("Sem is already initialized\n");
        return(-1);
    }    

    pthread_mutex_lock(&s->mutex);

    //Create semaphore
    int key = ftok("./file", counter);
    counter++;
    if(key == -1){
        perror("ftok");
    }

    s->sem_id = semget(key, 1, IPC_CREAT | 0666);
    if(s->sem_id == -1){
        perror("semget IPC_PRIVATE");
        pthread_mutex_unlock(&s->mutex);
        return(-1);
    }
    printf("this is the id %d\n", s->sem_id);

    //Initialize semaphore to n value
    if(semctl(s->sem_id, 0, SETVAL, n) == -1){
        perror("semctl proto");
        pthread_mutex_unlock(&s->mutex);
        return(-1);
    }

    s->init = 1;
    pthread_mutex_unlock(&s->mutex);
    
    return(1);
}

// This function reduses semaphore's value by one
int mysem_down(mysem_t *s){
    if(!s->init) return(-1);
    
    pthread_mutex_lock(&s->mutex);

    // printf("mpika stin down sto mutex %lu\n", (unsigned long)pthread_self());

    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = -1;
    sb.sem_flg = 0;
    if (semop(s->sem_id, &sb, 1) == -1) {
        perror("semop down failed");
        return -1;
    }
    
    //pthread_mutex_unlock(&s->mutex);

    return(0);
}

// This function increases semaphore's value by one/
int mysem_up(mysem_t *s){
    if (!s->init) return(-1);
    
    //pthread_mutex_lock(&s->mutex);
    
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = 1;
    sb.sem_flg = 0;
    if (semop(s->sem_id, &sb, 1) == -1) {
        perror("semop up failed");
        return -1;
    }
    
    pthread_mutex_unlock(&s->mutex);

    // printf("brika apo to mutex %lu\n", (unsigned long)pthread_self());

    return 1;
}

/*This function destroys a semaphore*/
int mysem_destroy(mysem_t *s){
    if (s->init != 1){ // Semaphore not initialized or destroyed
        printf("Semaphore not init or already destroyed\n");
        return(-1);
    }
    if (semctl(s->sem_id, 0, IPC_RMID) == -1) { // Destroy semaphore
        perror("semctl IPC_RMID");
        return(-1);
    }

    pthread_mutex_lock(&s->mutex);

    semctl(s->sem_id, 0, IPC_RMID);
    s->init = 0;

    pthread_mutex_unlock(&s->mutex);

    pthread_mutex_destroy(&s->mutex);

    return(1);
}