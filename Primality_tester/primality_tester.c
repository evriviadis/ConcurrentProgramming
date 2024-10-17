#include "library.h"
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

/*This is main. It assigns jobs to all threads created,
and tells them when it's time to finish.*/
int main(int argc, char* argv[]) {
    int N = atoi(argv[1]), found_available_thread, input;
    thread_infoT** threads = (thread_infoT**) malloc(sizeof(thread_infoT*) * N);

    if(argc != 2){
        printf("Wrong number of args\n");
        return(-1);
    }

    // Create and initialize threads
    for(int i = 0; i < N; i++){
        threads[i] = (thread_infoT*) malloc(sizeof(thread_infoT));
        
        threads[i]->finished = 0;
        threads[i]->available = 1;
        threads[i]->given_work = 0;
        threads[i]->terminate = 0;
        threads[i]->number_to_check = 0;
        
        if (pthread_create(&(threads[i]->thread_id), NULL, worker, threads[i]) != 0) {
            perror("Failed to create thread:");
            return 1;
        }
    }  

    //Get input data and assign work to the threads
    printf("Enter integers (Ctrl+D to stop):\n");
    while (scanf("%d", &input) != EOF) {
        // Loop to search until we find available thread
        found_available_thread = 0;
        while (!found_available_thread) {
            for (int i = 0; i < N; i++) {
                if(threads[i]->available){
                    threads[i]->number_to_check = input; // give input to the thread first! 
                    threads[i]->given_work = 1;          // and then start processing.
                    found_available_thread = 1;
                    threads[i]->available = 0;   
                    break;
                }
            }
        }
    }
    
    //After EOF wait for workers to finish their job
    for(int i = 0; i < N; i++){   
        if(threads[i]->available && !threads[i]->given_work){
            threads[i]->terminate = 1;
        }else{
            i--;
        }
    }

    //Free workers' memory and destroy them
    for(int i = 0; i < N; i++){
        if(threads[i]->finished){
            free(threads[i]);
        }else{
            i--;
        }
    }
    
    free(threads);
    return 0;
}#include "library.h"
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

/*This is main. It assigns jobs to all threads created,
and tells them when it's time to finish.*/
int main(int argc, char* argv[]) {
    int N = atoi(argv[1]), found_available_thread, input;
    thread_infoT** threads = (thread_infoT**) malloc(sizeof(thread_infoT*) * N);

    if(argc != 2){
        printf("Wrong number of args\n");
        return(-1);
    }

    // Create and initialize threads
    for(int i = 0; i < N; i++){
        threads[i] = (thread_infoT*) malloc(sizeof(thread_infoT));
        
        threads[i]->finished = 0;
        threads[i]->available = 1;
        threads[i]->given_work = 0;
        threads[i]->terminate = 0;
        threads[i]->number_to_check = 0;
        
        if (pthread_create(&(threads[i]->thread_id), NULL, worker, threads[i]) != 0) {
            perror("Failed to create thread:");
            return 1;
        }
    }  

    //Get input data and assign work to the threads
    printf("Enter integers (Ctrl+D to stop):\n");
    while (scanf("%d", &input) != EOF) {
        // Loop to search until we find available thread
        found_available_thread = 0;
        while (!found_available_thread) {
            for (int i = 0; i < N; i++) {
                if(threads[i]->available){
                    threads[i]->number_to_check = input; // give input to the thread first! 
                    threads[i]->given_work = 1;          // and then start processing.
                    found_available_thread = 1;
                    threads[i]->available = 0;   
                    break;
                }
            }
        }
    }
    
    //After EOF wait for workers to finish their job
    for(int i = 0; i < N; i++){   
        if(threads[i]->available && !threads[i]->given_work){
            threads[i]->terminate = 1;
        }else{
            i--;
        }
    }

    //Free workers' memory and destroy them
    for(int i = 0; i < N; i++){
        if(threads[i]->finished){
            free(threads[i]);
        }else{
            i--;
        }
    }
    
    free(threads);
    return 0;
}
