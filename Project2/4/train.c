#include "lib4.h"

void* passFunction(void *args) {
    enter_train();
 
    exit_train();

    return(NULL);
}

int main(int argc, char* argv[]) {
    // Initialize train
    train.threadId = (pthread_t *) malloc(sizeof(pthread_t));
    train.emptySeats = MAX_SEATS;
    train.trainSem = (mysem_t*) malloc(sizeof(mysem_t));
    train.blockTrain = (mysem_t*) malloc(sizeof(mysem_t));
    train.blockExit = (mysem_t*) malloc(sizeof(mysem_t));
    train.readyToExit = (mysem_t*) malloc(sizeof(mysem_t));
    train.endedTrip = (mysem_t*) malloc(sizeof(mysem_t));
    train.trainSem->init = 0; train.trainSem->sem_id = -1;
    train.blockTrain->init = 0; train.blockTrain->sem_id = -1; 
    train.blockExit->init = 0; train.blockExit->sem_id = -1;
    train.readyToExit->init = 0; train.readyToExit->sem_id = -1;
    train.endedTrip->init = 0; train.endedTrip->sem_id = -1;

    train.eof = 0;
    
    if (mysem_init(train.trainSem, 0) == -1 || 
        mysem_init(train.blockTrain, 0) == -1 ||
        mysem_init(train.readyToExit, 0) == -1 ||
        mysem_init(train.blockExit, 0) == -1 ||
        mysem_init(train.endedTrip, 0) == -1) {
        printf("error in train semaphore initialization\n");
        exit(-1);
    }
    pthread_create(train.threadId, NULL, trainFunc, NULL);

    // Create new passengers
    passenger_t **passengers = (passenger_t **) malloc(sizeof(passenger_t *));
    int totalPass = 0;
    int currentPass = 0;
    while (1) {
        int newPass;
        printf("currentPass: %d\n",currentPass);
        printf("Enter a number: ");
        if(scanf("%d", &newPass) == EOF){
            train.eof = 1;
            mysem_up(train.blockTrain);
            break;
        }

        // Initialize passengers
        passengers = (passenger_t **) realloc(passengers, sizeof(passenger_t *) * (totalPass+newPass));
        for (int i = totalPass; i < newPass+totalPass; i++) {
            passengers[i] = (passenger_t *) malloc(sizeof(passenger_t));
            passengers[i]->threadId = (pthread_t *) malloc(sizeof(pthread_t));
            pthread_create(passengers[i]->threadId, NULL, passFunction, NULL);
        }

        totalPass += newPass;
        currentPass += newPass;
        while(currentPass >= MAX_SEATS){
            mysem_up(train.blockTrain);
            currentPass -= MAX_SEATS;
            mysem_down(train.endedTrip);
        }
    }
    
    // Wait for passengers to finish
    for (int i = 0; i < totalPass; i++) {
        pthread_join(*(passengers[i]->threadId), NULL);
    }
    // Wait for train to finish

    pthread_join(*(train.threadId), NULL);    


    mysem_destroy(train.trainSem);
    mysem_destroy(train.blockExit);
    mysem_destroy(train.blockTrain);
    mysem_destroy(train.readyToExit);
    mysem_destroy(train.endedTrip);
    free(train.endedTrip);
    free(train.threadId);
    free(train.trainSem);
    free(train.blockExit);
    free(train.blockTrain);
    free(train.readyToExit);

    for (int i = 0; i < totalPass; i++) {
        free(passengers[i]->threadId);
        free(passengers[i]);
    }
    free(passengers);

    return(0);
}