#include "library.h"

int counter = 0;

/*This function initializes a semaphore with n value.*/
int mysem_init(mysem_t *s, int n){
    //Check n value
    if(n!=0 && n!=1){
        printf("Wrong arguement\n");
        return(0);
    }

    //Initialize and lock mutex
    if(pthread_mutex_init(&s->mutex, NULL) == -1){
        perror("mutex init");
        return(-1);
    }

    pthread_mutex_lock(&s->mutex);

    //Check if sem is already initialized
    if(s->init != 0){
        printf("Sem is already initialized\n");
        return(-1);
    }

    //Create semaphore
    int key = ftok("./file", counter);
    if(key == -1){
        perror("ftok");
        pthread_mutex_unlock(&s->mutex);
        return(-1);
    }
    counter++;

    s->sem_id = semget(key, 1, IPC_CREAT | 0666);
    if(s->sem_id == -1){
        perror("semget");

        pthread_mutex_unlock(&s->mutex);
        return(-1);
    }
    printf("Init |this is the id %d\n", s->sem_id);

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
    //Check if sem is initialized
    if(!s->init) return(-1);
    
    //pthread_mutex_lock(&s->mutex);
    // printf("mpika stin down sto mutex %lu\n", (unsigned long)pthread_self());

    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = -1;
    sb.sem_flg = 0;
    if (semop(s->sem_id, &sb, 1) == -1) {
        perror("semop down failed");
        return (-1);
    }
    
    //pthread_mutex_unlock(&s->mutex);
    return(0);
}

// This function increases semaphore's value by one/
int mysem_up(mysem_t *s){
    if (!s->init) return(-1);
    
    pthread_mutex_lock(&s->mutex);

    if(!s->init){
        pthread_mutex_unlock(&s->mutex);
        return(-1);
    }

    //Check semaphores value before calling 'up'
    int sem_value = semctl(s->sem_id, 0, GETVAL);
    if( sem_value == 1){
        pthread_mutex_unlock(&s->mutex);
        return(0);
    }else if(sem_value == -1){
        perror("semctl UP");
        pthread_mutex_unlock(&s->mutex);
        return(-1);
    }
    
    //Calling up 
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = 1;
    sb.sem_flg = 0;
    if(semop(s->sem_id, &sb, 1) == -1){
        perror("semop up failed");
        pthread_mutex_unlock(&s->mutex);
        return (-1);
    }
    
    //printf("sem value %d\n", semctl(s->sem_id, 0, GETVAL));
    pthread_mutex_unlock(&s->mutex);
    //printf("brika apo to mutex %lu\n", (unsigned long)pthread_self());
    return (1);
}

/*This function destroys a semaphore*/
int mysem_destroy(mysem_t *s){
    pthread_mutex_lock(&s->mutex); 
    
    // Semaphore not initialized or destroyed
    if(s->init != 1){
        printf("Semaphore not init or already destroyed\n");
        return(-1);
    }

    // Destroy semaphore
    if(semctl(s->sem_id, 0, IPC_RMID) == -1){ 
        perror("semctl IPC_RMID");
        //pthread_mutex_unlock(&s->mutex);
        return(-1);
    }

    s->init = 0;
    pthread_mutex_unlock(&s->mutex);
    pthread_mutex_destroy(&s->mutex);
    return(1);
}