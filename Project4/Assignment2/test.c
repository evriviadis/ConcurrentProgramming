/* #include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

void sleep_handler(int sig) {
    printf("Sleep timer fired!\n");
}

int main() {
    struct itimerval timer;

    timer.it_value.tv_sec = 1;  // Initial expiration in 1 second
    timer.it_value.tv_usec = 0;

    timer.it_interval.tv_sec = 1;  // Repeating interval of 1 second
    timer.it_interval.tv_usec = 0;

    signal(SIGVTALRM, sleep_handler);

    if (setitimer(ITIMER_VIRTUAL, &timer, NULL) == -1) {
        perror("setitimer failed");
        exit(EXIT_FAILURE);
    }

    printf("Timer started, waiting for signals...\n");

    while (1) {
        pause();  // Wait for signals
    }

    return 0;
} */

#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

void sleep_handler(int sig) {
    printf("Real-time signal %d fired!\n", sig);
}

int main() {
    timer_t timer_id;
    struct sigevent sev;
    struct itimerspec ts;

    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGRTMIN;  // Use a real-time signal
    sev.sigev_value.sival_ptr = &timer_id;

    timer_create(CLOCK_REALTIME, &sev, &timer_id);

    signal(SIGRTMIN, sleep_handler);  // Set the handler

    ts.it_value.tv_sec = 1;
    ts.it_value.tv_nsec = 0;
    ts.it_interval.tv_sec = 1;
    ts.it_interval.tv_nsec = 0;

    timer_settime(timer_id, 0, &ts, NULL);

    while (1) {
        pause();
    }

    return 0;
}
