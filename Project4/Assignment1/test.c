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
    printf("producer finished\n");
    mycoroutines_switchto(&main_co);
}

void consumer(void *arg) {
    for (int i = 0; i < 10; i++) {
        while (count == 0) {
            mycoroutines_switchto((co_t *)arg);
        }
        int item = buffer[--count];
        printf("Consumed: %d\n", item);
    }
    printf("consumer finished\n");
    mycoroutines_switchto(&main_co);
}

int main() {
    mycoroutines_init(&main_co);

    mycoroutines_create(&producer_co, producer, &consumer_co);
    mycoroutines_create(&consumer_co, consumer, &producer_co);

    mycoroutines_switchto(&producer_co);
    printf("i finished producting\n");
    mycoroutines_switchto(&consumer_co);
    printf("i finished consuming\n");
    mycoroutines_destroy(&producer_co);
    mycoroutines_destroy(&consumer_co);

    free(main_co.context.uc_stack.ss_sp);
    printf("\n\n--Here im in the end--\n\n");

    return 0;
}