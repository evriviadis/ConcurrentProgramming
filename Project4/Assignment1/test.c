#include "coroutines.h"

// Producer-Consumer test
#define BUFFER_SIZE 5
int buffer[BUFFER_SIZE];
int count = 0;

co_t producer_co, consumer_co;

void producer(void *arg) {
    for (int i = 0; i < 10; i++) {
        while (count == BUFFER_SIZE) {
            mycoroutines_switchto((co_t *)arg);
        }
        buffer[count++] = i;
        printf("Produced: %d\n", i);
    }
    // mycoroutines_switchto(arg);
}

void consumer(void *arg) {
    for (int i = 0; i < 10; i++) {
        while (count == 0) {
            mycoroutines_switchto((co_t *)arg);
        }
        int item = buffer[--count];
        printf("Consumed: %d\n", item);
    }
    // mycoroutines_switchto(arg);
}

int main() {
    mycoroutines_init(&main_co);

    mycoroutines_create(&producer_co, producer, &consumer_co);
    mycoroutines_create(&consumer_co, consumer, &producer_co);

    while (!producer_co.finished || !consumer_co.finished) {
        if (!producer_co.finished) mycoroutines_switchto(&producer_co);
        printf("i finished producting\n");
        if (!consumer_co.finished) mycoroutines_switchto(&consumer_co);
    }

    printf("current co %d\n", current_co->finished);

    printf("\n\n--Here im in the end--\n\n");
    return 0;
}