#include "coroutines.h"
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

#define NUM_COROUTINES 4
#define TIME_QUANTUM 2  // Time slice in seconds

co_t coroutines[NUM_COROUTINES];
int current_index = 0;  // Index for Round Robin
int remaining = NUM_COROUTINES;  // Tracks active coroutines

// Timer signal handler
void timer_handler(int signum) {
    // Save current context and switch to the next coroutine
    co_t *next_co;

    if (!coroutines[current_index].finished) {
        printf("Time quantum expired: Coroutine %d\n", current_index);
    }

    current_index = (current_index + 1) % NUM_COROUTINES;
    next_co = &coroutines[current_index];
    printf("current_index: %d\n",current_index);

    // Skip finished coroutines
    while (next_co->finished) {
        current_index = (current_index + 1) % NUM_COROUTINES;
        next_co = &coroutines[current_index];
    }

    mycoroutines_switchto(next_co);
}

// Coroutine function
void thread_function(void *arg) {
    int id = *((int *)arg);
    for (int i = 0; i < 5; i++) {
        printf("Coroutine %d: iteration %d\n", id, i);
        sleep(1);  // Simulate some work
    }
    printf("Coroutine %d finished\n", id);
}

void setup_timer() {
    struct itimerval timer;

    // Set the timer to expire after TIME_QUANTUM seconds
    timer.it_value.tv_sec = TIME_QUANTUM;
    timer.it_value.tv_usec = 0;

    // Set the timer to reset every TIME_QUANTUM seconds
    timer.it_interval.tv_sec = TIME_QUANTUM;
    timer.it_interval.tv_usec = 0;

    // Install the signal handler
    signal(SIGALRM, timer_handler);

    // Start the timer
    setitimer(ITIMER_REAL, &timer, NULL);
}

int main() {
    mycoroutines_init(&main_co);

    // Create coroutines
    int ids[NUM_COROUTINES] = {0, 1, 2};
    for (int i = 0; i < NUM_COROUTINES; i++) {
        mycoroutines_create(&coroutines[i], thread_function, &ids[i]);
    }

    // Set up the timer
    setup_timer();

    // Start the first coroutine
    mycoroutines_switchto(&coroutines[current_index]);

    // Scheduler loop (continues until all coroutines are finished)
    while (remaining > 0) {
        remaining = 0;
        for (int i = 0; i < NUM_COROUTINES; i++) {
            if (!coroutines[i].finished) remaining++;
        }
        pause();  // Wait for signal (timer)
    }

    // Cleanup
    for (int i = 0; i < NUM_COROUTINES; i++) {
        mycoroutines_destroy(&coroutines[i]);
    }

    printf("All coroutines have finished.\n");
    return 0;
}
