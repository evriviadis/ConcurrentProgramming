#include "lib3.h"

void* thread(void* args) {
    threadInfoT* thread = (threadInfoT*) args;
    //printf("hi im thread %ld\n",thread->threadId);
    
        enterBridge(thread);
        sleep(0.1);
        exitBridge(thread);

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 4){
        perror("wrong arguments\n");
        return -1;
    }

    pthread_mutex_init(&print_mutex, NULL);

    info.semApply = (mysem_t *) malloc(sizeof(mysem_t));
    info.semApply->init = 0;
    if(mysem_init(info.semApply,1)==-1){
        printf("something wrong with init\n");
        exit(-1);
    }

    info.N = atoi(argv[1]);
    info.numRedCars = atoi(argv[2]);
    info.numBlueCars = atoi(argv[3]);
    info.priority = NONE;
    info.inBridge = 0;
    info.flow = NONE;
    info.s = (mysem_t **) malloc(sizeof(mysem_t*)*info.N);

    threadInfoT** redThreads = (threadInfoT**) malloc(sizeof(threadInfoT*) * info.numRedCars);
    threadInfoT** blueThreads = (threadInfoT**) malloc(sizeof(threadInfoT*) * info.numBlueCars);
    
    for (int i = 0; i < info.N ; i++){
        info.s[i] = (mysem_t *) malloc(sizeof(mysem_t));
        info.s[i]->init = 0;
        info.s[i]->sem_id = -1;
        if(mysem_init(info.s[i], 1) == -1){
            printf("something wrong with init\n");
            exit(-1);
        }
    }
    for (int i = 0; i < info.numRedCars; i++) {
        redThreads[i] = (threadInfoT *) malloc(sizeof(threadInfoT));
        redThreads[i]->color = RED;
        redThreads[i]->threadIndex = i;
        pthread_create(&(redThreads[i]->threadId), NULL, thread, redThreads[i]);
    }
    for (int i = 0; i < info.numBlueCars; i++) {
        blueThreads[i] = (threadInfoT *) malloc(sizeof(threadInfoT));
        blueThreads[i]->color = BLUE;
        blueThreads[i]->threadIndex = i;
        pthread_create(&(blueThreads[i]->threadId), NULL, thread, blueThreads[i]);
    }


    for (int i = 0; i < info.numRedCars; i++){
        pthread_join((redThreads[i]->threadId),NULL);
        free(redThreads[i]);
    }   
    for (int i = 0; i < info.numBlueCars; i++){
        pthread_join((blueThreads[i]->threadId),NULL);
        free(blueThreads[i]);
    }
    for (int i = 0; i < info.N ; i++){
        mysem_destroy(info.s[i]);
        free(info.s[i]);
    }

    free(info.s);
    free(blueThreads);
    free(redThreads);

    pthread_mutex_destroy(&print_mutex);
    mysem_destroy(info.semApply);

    printf("everyone passed the bridge\n");
    
    return 0;
}