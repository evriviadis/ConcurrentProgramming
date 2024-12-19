#include "lib2.h"

void timer_handler(){
    printf("\n\nTimer hits!!\n\n");
    mythreads_yield();
}

void sleep_handler(){
    printf("update sleepy thread!!!\n\n");
    update_sleep();
}

void setup_timer() {
    struct itimerval timer;

    timer.it_value.tv_sec = TIME_QUANTUM;
    timer.it_value.tv_usec = 0;

    timer.it_interval.tv_sec = TIME_QUANTUM;
    timer.it_interval.tv_usec = 0;

    signal(SIGALRM, timer_handler);
    

    setitimer(ITIMER_REAL, &timer, NULL);
}


void setup_sleep_timer(double interval) {
    struct itimerval timer;

    timer.it_value.tv_sec = interval;
    timer.it_value.tv_usec = 0;

    timer.it_interval.tv_sec = interval;
    timer.it_interval.tv_usec = 0;


    signal(SIGUSR1, sleep_handler);

    if (setitimer(ITIMER_REAL, &timer, NULL) == -1) {
        perror("setitimer");
        exit(EXIT_FAILURE);
    }
}


void thread_function(void *arg) {
    int id = *((int *)arg);
    for (int i = 0; i < 5; i++) {
        printf("Coroutine %d: iteration %d\n", id, i);
        mythreads_sleep(1);  // Simulate some work
    }
    printf("Coroutine %d finished\n", id);
    current_co->finished = 1;
    mythreads_yield();
}

int main(int argc, char *argv[]){
    mythreads_init();

    int ids[3] = {0, 1, 2};

    for(int i=0 ; i<3 ; i++){
        mythr_t *thr = (mythr_t *)malloc(sizeof(mythr_t));
        thr->id = i;
        mythreads_create(thr, thread_function, &ids[i]);
        idCounter++;
    }
    printf("All threads are created!!!\n");

    setup_sleep_timer(MIN_SLEEP_CHECK);
    setup_timer();

    mythreads_yield();

    printf("Im back at main\n");
    
    return 0;
}