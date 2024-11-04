#include "lib3.h"

void* thread(void* args) {
    threadInfoT* thread = (threadInfoT*) args;
    printf("hi im thread %ld\n",thread->threadId);
    
        enterBridge();
        sleep(3);
        exitBridge();

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 4){
        perror("wrong arguments\n");
        return -1;
    } 

    info.N = atoi(argv[1]);
    info.numRedCars = atoi(argv[2]);
    info.numBlueCars = atoi(argv[3]);
    info.s = (mysem_t *) malloc(sizeof(mysem_t)*info.N);

    threadInfoT** redThreads = (threadInfoT**) malloc(sizeof(threadInfoT*) * info.numRedCars);
    threadInfoT** blueThreads = (threadInfoT**) malloc(sizeof(threadInfoT*) * info.numBlueCars);
    
    for (int i = 0; i < info.numRedCars; i++) {
        redThreads[i] = (threadInfoT *) malloc(sizeof(threadInfoT));
        pthread_create(&(redThreads[i]->threadId), NULL, thread, redThreads[i]);
    }
    for (int i = 0; i < info.numBlueCars; i++) {
        blueThreads[i] = (threadInfoT *) malloc(sizeof(threadInfoT));
        pthread_create(&(blueThreads[i]->threadId), NULL, thread, blueThreads[i]);
    }


    sleep(2);

    for (int i = 0; i < info.numRedCars; i++)  free(redThreads[i]);
    for (int i = 0; i < info.numBlueCars; i++) free(blueThreads[i]);
    free(blueThreads);
    free(redThreads);
    
    return 0;
}