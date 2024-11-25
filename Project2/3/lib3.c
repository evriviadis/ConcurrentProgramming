#include "lib3.h"

pthread_mutex_t print_mutex;
pthread_mutex_t clear_mutex;

carInfo_t info;

int enterBridge(threadInfoT *thread) {

    while(1){

        mysem_down(info.semApply);
        printWithColor(info.priority,"priority");
       
        if(info.priority == thread->color && !info.inBridge){ /* an i pleura moy exei protereotita */ 
            /* tote dilose endiaferon kai spase tin protereotita oste na min exei kaneis */
            
            printf("\033[0;37m");
            printf("GATE 1 %d\n",thread->threadIndex);

            info.passed = 0;
            wannaEnter(thread);
            info.flow = thread->color;
            printWithColor(info.flow,"flow");
            info.priority = NONE;
            break;

        }else if(info.priority != NONE && info.inBridge){ /* an exei protereotita i alli pleura */
            
            /* wait */

            mysem_up(info.semApply);
            
        }else if(info.passed > info.N + 1){ /* exoun perasei polla autokinita apo to xroma mou */
            
            printf("MPIKA KAI EDO MESA  %d \n\n\n",thread->threadIndex);

            /* tote dose protereotita stin alli pleura */
            
            info.priority = !info.flow;
            info.passed = 0;
            info.flow = NONE;
            mysem_up(info.semApply);

        }else if(info.flow == thread->color){ /* to reuma einai sto xroma mou */

            printf("\033[0;37m");
            printf("GATE 2 %d\n",thread->threadIndex);
            
            /* tote dilose endiaferon ston semaphore (fae block mexri na kanei free) */
            wannaEnter(thread);
            printWithColor(info.flow,"flow");
            break;
        
        }else{ ///diladi to flow einai eite NONE eite stin alli pleura

            if(!info.inBridge && info.priority == NONE){
                
                printf("\033[0;37m");
                printf("GATE 3 %d\n",thread->threadIndex);

                info.passed = 0;
                wannaEnter(thread);
                info.flow = thread->color;
                printWithColor(info.flow,"flow");
                break;
            
            }else{
                if(!info.numRedCars || !info.numBlueCars){
                    info.priority = NONE;
                }
                mysem_up(info.semApply);
            }
        }
    }
    mysem_up(info.semApply);

    return 0;
}

int exitBridge(threadInfoT *thread) {
    
    mysem_up(info.s[thread->semIndex]);

    mysem_down(info.semApply);
    info.inBridge--;
    mysem_up(info.semApply);
    

    pthread_mutex_lock(&print_mutex);
    
    if(thread->color == RED){
        printf("\033[0;31m");
    }else if(thread->color == BLUE){
        printf("\033[0;34m");
    }
    printf("thread: %d exit the bridge\n",thread->threadIndex);

    pthread_mutex_unlock(&print_mutex);

    clearPriority(thread);

    return 0;
}

int wannaEnter(threadInfoT *thread){
    
    printf("\033[0;37m");
    printf("passed streak: %d, inbridge: %d\n",info.passed, info.inBridge);
    mysem_down(info.s[info.i]);

    // THE CODE IS LIKE THIS IN THIS AREA CUZ THE RIGHT PART IS FOR PRINTING WITH COLOR AND VARIABLES IN THE SAME TIME
                                            pthread_mutex_lock(&print_mutex);

                                            if(thread->color == RED){
                                                printf("\033[0;31m");
                                            }else if(thread->color == BLUE){
                                                printf("\033[0;34m");
                                            }
                                            printf("thread: %d enter the bridge\n",thread->threadIndex);

                                            pthread_mutex_unlock(&print_mutex);

    thread->semIndex = info.i;
    info.inBridge++;
    info.passed++;
    info.i++;
    if(info.i==info.N) info.i = 0;
    return(0);
}

void printWithColor(color_t color,char *text){
    pthread_mutex_lock(&print_mutex);

    if(color == RED){
        printf("\033[0;31m");
    }else if(color == BLUE){
        printf("\033[0;34m");
    }else{
        printf("\033[0;37m");
    }
    printf("%s\n",text);

    pthread_mutex_unlock(&print_mutex);
}


void clearPriority(threadInfoT *thread){

    pthread_mutex_lock(&clear_mutex);
    
    if(thread->color == RED){
        if(info.numRedCars == 1){
            info.priority = NONE;
            info.passed = 0;
            info.flow = NONE;        
        }
        info.numRedCars--;
    }else{
        if(info.numBlueCars == 1){
            info.priority = NONE;
            info.passed = 0;
            info.flow = NONE;
        } 
        info.numBlueCars--;
    }

    pthread_mutex_unlock(&clear_mutex);
    
}