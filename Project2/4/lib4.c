#include "lib4.h"

train_t train;
int maxSeats;

int enter_train(){
    printf("-ready to enter train-\n");
    if(mysem_down(train.trainSem) == -1){
        printf("error in down\n");
    }
    printf("-entered train-\n");
    train.emptySeats--;
    
    return 0;
}

int exit_train(){
    printf("-ready to exit-\n");
    if(mysem_down(train.blockExit) == -1){
        printf("error in down exit train\n");
    }
    train.emptySeats++;
    printf("-exited train-\n");

    mysem_up(train.readyToExit);

    return 0;
}

void* trainFunc(){
    do{
        printf("\n-train ready to go-\n");
        mysem_down(train.blockTrain);
        
        if (train.eof) {
            int sem_res = mysem_up(train.trainSem);
            if(sem_res == 0){
                printf("lost call of up\n");
            }else if(sem_res == -1){
                printf("error in up\n");
            }
        }
        else {
            for (int i = 0; i < MAX_SEATS; i++) {
                int sem_res = mysem_up(train.trainSem);
                if(sem_res == 0){
                    printf("lost call of up\n");
                }else if(sem_res == -1){
                    printf("error in up\n");
                }
                printf("time: %d\n",i);
            }
        }

        sleep(3);
        
        printf("-train is going-\n\n\n");
        if (train.eof) {
            int up_res = mysem_up(train.blockExit);
            if(up_res == 0){
                printf("lost call of up\n in time:\n\n\n");
            }else if(up_res == -1){
                printf("error in up\n");
            }  
        }else{
            for (int i = 0; i < MAX_SEATS; i++) {
                int up_res = mysem_up(train.blockExit);
                if(up_res == 0){
                    printf("lost call of up\n in time: %d\n\n\n",i);
                }else if(up_res == -1){
                    printf("error in up\n");
                }   
                mysem_down(train.readyToExit);
            }
        }

        printf("-train ended trip-\n");
        mysem_up(train.endedTrip);
    }while(!train.eof);    
        
    return NULL;
}