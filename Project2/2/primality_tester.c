#include "lib2.h"

/*This is main. It assigns jobs to all threads created,
and tells them when it's time to finish.*/
int main(int argc, char* argv[]){
    int N = atoi(argv[1]);
    int input, i;
    thread_infoT** threads;

    if(argc != 2){
        printf("Wrong number of args\n");
        return(-1);
    }

    //Allocate memory for all the workers
    threads = (thread_infoT**) malloc(sizeof(thread_infoT*) * N);
    if(threads == NULL){
        perror("malloc");
        return(-1);
    }

    //Create and initialize each worker
    for(int i = 0; i < N; i++){
        threads[i] = (thread_infoT*) malloc(sizeof(thread_infoT));
        if(threads[i] == NULL){
            perror("malloc");
            return(-1);
        }

        threads[i]->s1 = (mysem_t*) malloc(sizeof(mysem_t));
        threads[i]->s2 = (mysem_t*) malloc(sizeof(mysem_t));

        threads[i]->s1->init = 0;
        threads[i]->s2->init = 0;

        if(mysem_init((threads[i]->s1), 1) == -1){
            printf("something wrong 1\n");
            return(-1);
        }

        if(mysem_init((threads[i]->s2), 0) == -1){
            printf("something wrong 2\n");
            return(-1);
        }

        threads[i]->terminate = 0; 
        threads[i]->number_to_check = 0;
        
        if(pthread_create(&(threads[i]->thread_id), NULL, worker, threads[i]) != 0){
            perror("Failed to create thread");
            return(-1);
        }
    }  

    //Get input data and assign work to the threads
    printf("Enter integers (Ctrl+D to stop):\n");
    int j = 0;
    while(scanf("%d", &input) != EOF){
        for(; j<N; j++){
            mysem_down(threads[j]->s1);
            threads[j]->number_to_check = input;
            mysem_up(threads[j]->s2);
            break;
        }
        if(j>=N) j = 0;
    }

    //After EOF wait for workers to finish their job
    for(i = 0; i < N; i++){   
        mysem_down(threads[i]->s1);
        threads[i]->terminate = 1;
        mysem_up(threads[i]->s2);
        
        if(pthread_join(threads[i]->thread_id, NULL) != 0){
            perror("pthread_join");
        }

        if(mysem_destroy(threads[i]->s1) == -1){
            printf("error in detsroy\n");
        }

        if(mysem_destroy(threads[i]->s2) == -1){
            printf("error in detsroy\n");
        }

        free(threads[i]->s1);
        free(threads[i]->s2);
        free(threads[i]);
    }

    free(threads);
    return 0;
}