#include "lib2.h"

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

/*This function initalizes a monitor*/
void monitor_init() {
    if (pthread_mutex_init(&monitor.mutex, NULL)) {
        perror("ERROR: Mutex Init");
    }
    if (pthread_cond_init(&monitor.cond, NULL)) {
        perror("ERROR: Cond Init");
    }   

    //Initialize anything else you put into monitor
}

void assign_work(thread_infoT *thread, int number_to_check) {
    pthread_mutex_lock(&monitor.mutex);

    thread->number_to_check = number_to_check;
    pthread_cond_signal(&monitor.condition);

    pthread_mutex_unlock(&monitor.mutex);
}

void monitor_destroy() {
    pthread_mutex_destroy(&monitor.mutex);
    pthread_cond_destroy(&monitor.condition);
}

/*This function is the worker. It takes as input an 
integer array, and tells if it's content is prime or not*/
void* worker(void* arg){
    int result;
    thread_infoT* thread = (thread_infoT*) arg;
        
    while(1){

        //wait until a job is given
        //monitor wait
    
        pthread_cond_wait(&monitor.cond, &monitor.mutex);
    
        //mysem_down(thread->s2);
        if (thread->terminate) break;

        result = is_prime(thread->number_to_check); 
        if(result){
            printf("The %d is Prime: %d\n",thread->number_to_check, result);
        }
        else{
            printf("The %d is Not Prime: %d\n", thread->number_to_check, result);
        }
        //mysem_up(thread->s1);
    }
    
    return NULL;
}