#include "library.h"

void *thread(void *args){
    mysem_t *s = (mysem_t *) args;
    
    mysem_down(s);
    
    sleep(1);
    printf("hi this is a thread boi %lu \n", (unsigned long)pthread_self());

    mysem_up(s);
    
    return (NULL);
}

int main(int argc, char *argv[]){
    pthread_t th[5];
    mysem_t s;

    s.init = 0;
    s.sem_id = -1;
    pthread_mutex_init(&s.mutex, NULL);

    if(mysem_init(&s, 1) == -1){
        printf("error\n");
        return(-1);
    }

    for (int i = 0; i < 5; i++){
        pthread_create(&th[i], NULL, thread, &s);
    } 
        
    for (int i = 0; i < 5; i++){
        pthread_join(th[i], NULL);
    }

    mysem_destroy(&s);

    return(0);
}
