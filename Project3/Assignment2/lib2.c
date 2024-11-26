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

void monitor_init(monitor_t *mon) {
    if (pthread_mutex_init(&mon->mutex, NULL)) {
        perror("ERROR: Mutex Init");
    }
    if (pthread_cond_init(&mon->cond1, NULL) || pthread_cond_init(&mon->cond2, NULL)) {
        perror("ERROR: Cond Init");
    }
    mon->cond1_val = 0; mon->cond1_va2 = 0;
}

void assign_work(monitor_t *mon, number_to_check) {
    
}

void monitor_wait(monitor_t *mon) {
    pthread_mutex_lock(&mon->mutex);
    while (mon->resource_flag == 0) {  // Wait for resource to become available
        pthread_cond_wait(&mon->condition, &mon->mutex);
    }
    mon->resource_flag = 0;
    pthread_mutex_unlock(&mon->mutex);
}

void monitor_signal(monitor_t *mon) {
    pthread_mutex_lock(&mon->mutex);

    if (mon->resource_flag == 1) {
        printf("ERROR: Resource flag is already 1\n");
    }
    else {
        mon->resource_flag = 1;
        pthread_cond_signal(&mon->condition);
    }
    pthread_mutex_unlock(&mon->mutex);
}

void monitor_destroy(monitor_t *mon) {
    pthread_mutex_destroy(&mon->mutex);
    pthread_cond_destroy(&mon->cond1);
    pthread_cond_destroy(&mon->cond2);
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