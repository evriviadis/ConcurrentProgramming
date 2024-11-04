#include "lib3.h"

carInfo_t info;

int enterBridge() {

    while(1){
        if(/* an i pleura moy exei protereotita */){
     
            /* tote dilose endiaferon kai spase tin protereotita oste na min exei kaneis */

        }else if(/* an exei protereotita i alli pleura */){
            
            /* wait */
            
        }else if(/* exoun perasei polla autokinita apo to xroma mou */){
            
            /* tote dose protereotita stin alli pleura */
            
        }else if(/* to reuma einai sto xroma mou */){
            
            /* tote dilose endiaferon ston semaphore (fae block mexri na kanei free) */
            mysemdown(info.s[info.i]);
            info.i++;
            if(info.i==info.N) info.i = 0;
        
        }else{
            /* dilose endiaferon na peraseis */
            mysemdown(info.s[info.i]);
            info.i++;
            if(info.i==info.N) info.i = 0;
        }
    }

    return 0;
}

int exitBridge() {

    return 0;
}
