#include "lib3.h"

pthread_mutex_t print_mutex;

carInfo_t info;

int enterBridge(threadInfoT *thread) {

    while(1){

        mysem_down(info.semApply);
       
        if(info.priority == thread->color && !info.inBridge){ /* an i pleura moy exei protereotita */ 
            /* tote dilose endiaferon kai spase tin protereotita oste na min exei kaneis */
            
            info.passed = 0;
            wannaEnter(thread);
            info.flow = thread->color;
            printWithColor(NONE,"ENTER 1");
            printWithColor(info.flow,"flow");
            info.priority = NONE;
            break;

        }else if(info.priority != NONE && info.inBridge){ /* an exei protereotita i alli pleura */
            
            /* wait */

            mysem_up(info.semApply);
            
        }else if(info.passed > info.N + 1){ /* exoun perasei polla autokinita apo to xroma mou */
            
            /* tote dose protereotita stin alli pleura */
            
            info.priority = !info.flow;
            info.passed = 0;
            mysem_up(info.semApply);

        }else if(info.flow == thread->color){ /* to reuma einai sto xroma mou */
            
            /* tote dilose endiaferon ston semaphore (fae block mexri na kanei free) */
            wannaEnter(thread);
            printWithColor(NONE,"ENTER 2");
            printWithColor(info.flow,"flow");
            break;
        
        }else{ ///diladi to flow einai eite NONE eite stin alli pleura
            
            if(!info.inBridge){

                info.passed = 0;
                wannaEnter(thread);
                info.flow = thread->color;
                printWithColor(NONE,"ENTER 3");
                printWithColor(info.flow,"flow");
                break;
            
            }else{

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

    return 0;
}

int wannaEnter(threadInfoT *thread){
    
    printf("\033[0;37m");
    printf("passed: %d, inbridge: %d\n",info.passed, info.inBridge);
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