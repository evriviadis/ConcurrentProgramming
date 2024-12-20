#include "lib2.h"

void timer_handler(){
    printf("\n\nTimer hits!!\n\n");
    mythreads_yield();
}

void sleep_handler(){
    //printf("update sleepy thread!!!\n\n");
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


void setup_sleep_timer() {
    timer_t timer_id;
    struct sigevent sev;
    struct itimerspec ts;

    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGRTMIN;  // Use a real-time signal
    sev.sigev_value.sival_ptr = &timer_id;

    timer_create(CLOCK_REALTIME, &sev, &timer_id);

    signal(SIGRTMIN, sleep_handler);  // Set the handler

    ts.it_value.tv_sec = 1;
    ts.it_value.tv_nsec = 0 /* MIN_SLEEP_CHECK */ ;
    ts.it_interval.tv_sec = 1;
    ts.it_interval.tv_nsec =0 /* MIN_SLEEP_CHECK */ ;

    timer_settime(timer_id, 0, &ts, NULL);
}


void thread_function(void *arg) {
    int id = *((int *)arg);
    for (int i = 0; i < 5; i++) {
        printf("Coroutine %d: semdown\n", id);
        mythreads_sem_down(s);
        printf("Coroutine %d: iteration %d\n", id, i);
        mythreads_sleep(2);  // Simulate some work
        mythreads_sem_up(s);
    }
    printf("Coroutine %d finished\n", id);
    current_thread->finished = 1;
    current_thread->status = FINISH;
    mythreads_yield();
}

int main(int argc, char *argv[]){
    pthread_mutex_init(&lock, NULL);
    mythreads_init();

    int ids[3] = {0, 1, 2};
    mythr_t *thrArray[3];

    for(int i=0 ; i<3 ; i++){
        
        thrArray[i] = (mythr_t *)malloc(sizeof(mythr_t));
        mythreads_create(thrArray[i], thread_function, &ids[i]);
        
    }
    printf("All threads are created!!!\n");

    s = (mysem_t *)malloc(sizeof(mysem_t));

    mythreads_sem_create(s,1);

    setup_sleep_timer();
    setup_timer();

    //raise(SIGVTALRM);

    
    mythreads_join(thrArray[0]);
    mythreads_join(thrArray[1]);
    mythreads_join(thrArray[2]);

    printf("Im back at main\n");

    mythreads_destroy(thrArray[0]);
    mythreads_destroy(thrArray[1]);
    mythreads_destroy(thrArray[2]);

    pthread_mutex_destroy(&lock);
    return 0;
}

