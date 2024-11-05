#include "lib3.h"

carInfo_t info;

int enterBridge(threadInfoT *thread) {

    while(1){
        if(info.priority == thread->color && !info.inBridge){ /* an i pleura moy exei protereotita */ 
            /* tote dilose endiaferon kai spase tin protereotita oste na min exei kaneis */
            
            info.flow = thread->color;
            wannaEnter(thread);
            info.priority = NONE;
            break;
        }else if(info.priority != NONE){ /* an exei protereotita i alli pleura */
            
            /* wait */
            
        }else if(info.passed > info.N + 1){ /* exoun perasei polla autokinita apo to xroma mou */
            
            /* tote dose protereotita stin alli pleura */
            info.priority = !info.flow;
            info.passed = 0;

        }else if(info.flow == thread->color){ /* to reuma einai sto xroma mou */
            
            /* tote dilose endiaferon ston semaphore (fae block mexri na kanei free) */
            wannaEnter(thread);
            break;
        
        }else{ ///diladi to flow einai eite NONE eite stin alli pleura
            if(!info.inBridge){
                info.flow = thread->color;
                wannaEnter(thread);
                break;
            }
        }
    }

    return 0;
}

int exitBridge(threadInfoT *thread) {
    mysem_up(&(info.s[thread->semIndex]));
    info.inBridge--;

    return 0;
}

int wannaEnter(threadInfoT *thread){
    
    printf("hi from the wanna Enter \n");
    mysem_down(info.s[info.i]);

    printf("thread: %ld enter the bridge\n",(unsigned long)pthread_self());

    thread->semIndex = info.i;
    info.inBridge++;
    info.passed++;
    info.i++;
    if(info.i==info.N) info.i = 0;
    return(0);
}
