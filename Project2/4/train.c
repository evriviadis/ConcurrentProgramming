#include "lib4.h"

void * passFunction(void *args){
    passenger_t *thread = (passenger_t *)args;
}

int main(int argc, char* argv[]){

    //Check arguements
    if(argc != 2){
        printf("Invalid arguements\n");
        return(-1);
    }

    int K = atoi(argv[1]);

    passenger_t **passengers = (passenger_t **)malloc(sizeof(passenger_t*)*K);
    

    for(int i=0 ; i < passengers ; i++){
        passengers[i] = (passenger_t *) malloc(sizeof(passenger_t));
        pthread_create(passengers[i]->threadId, NULL, passFunction, passengers[i]);
    }

    return(0);
}