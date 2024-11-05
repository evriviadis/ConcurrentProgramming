#include "lib3.h"

pthread_mutex_t print_mutex;

carInfo_t info;

int enterBridge(threadInfoT *thread) {

    while(1){
        if(info.priority == thread->color && !info.inBridge){ /* an i pleura moy exei protereotita */ 
            /* tote dilose endiaferon kai spase tin protereotita oste na min exei kaneis */
            
            wannaEnter(thread);
            info.flow = thread->color;
            printWithColor(NONE,"ENTER 1");
            printWithColor(info.flow,"flow");
            info.priority = NONE;
            break;
        }else if(info.priority != NONE){ /* an exei protereotita i alli pleura */
            
            //printWithColor(info.priority,"priority");
            /* wait */
            
        }else if(info.passed > info.N + 1){ /* exoun perasei polla autokinita apo to xroma mou */
            
            /* tote dose protereotita stin alli pleura */
            info.priority = !info.flow;
            info.passed = 0;

        }else if(testFlow(thread->color)){ /* to reuma einai sto xroma mou */
            
            /* tote dilose endiaferon ston semaphore (fae block mexri na kanei free) */
            wannaEnter(thread);
            printWithColor(NONE,"ENTER 2");
            printWithColor(info.flow,"flow");
            mysem_up(info.semFlow);
            break;
        
        }else{ ///diladi to flow einai eite NONE eite stin alli pleura
            mysem_down(info.semFlow);
    
            if(!info.inBridge){
                wannaEnter(thread);
                info.flow = thread->color;
                printWithColor(NONE,"ENTER 3");
                printWithColor(info.flow,"flow");
                mysem_up(info.semFlow);
                break;
            }
            mysem_up(info.semFlow);
        }
    }

    return 0;
}

int exitBridge(threadInfoT *thread) {
    mysem_up(info.s[thread->semIndex]);
    info.inBridge--;
    
    pthread_mutex_lock(&print_mutex);
    
    if(thread->color == RED){
        printf("\033[0;31m");
    }else if(thread->color == BLUE){
        printf("\033[0;34m");
    }
    printf("thread: %d exit the bridge\n",thread->threadIndex);

    pthread_mutex_unlock(&print_mutex);

    return 0;
}

int wannaEnter(threadInfoT *thread){

    mysem_down(info.s[info.i]);


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

int testFlow(color_t color){
    mysem_down(info.semFlow);

    if(info.flow == color){
        return 1;
    }else{
        mysem_up(info.semFlow);
        return 0;
    }
}