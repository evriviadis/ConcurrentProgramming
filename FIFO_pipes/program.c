#include "library.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>
#include <string.h>

//TA THREAD1 KAI THREAD2 FUNCTION THA PREPEI NA BGOUNE STO TELOS
//GIATI TO FILE AUTO THA EINAI MONO GIA TESTING KAI THA TA KALEI OLA APO TIN MAIN 

void* thread1(void* arg){
    printf("Hi I'm thread 1\n");
    int file_dir, read=0, /* p_write, */ active;
    char byte='\0'; 
    thread_argsT* args;
    args = (thread_argsT *)arg;  
    
    
    //open file1
    file_dir = open(args->file_name, O_RDONLY);
    if (file_dir == -1) {
        perror("file");
        return((void *)-1);
    }


    while(1) {

        //sinthiki edo giati if it continuous studying it will find eof
        //printf("%d write edge\n", args->pipe_write->write_edge);
        //printf("%d read edge\n", args->pipe_write->read_edge);

        if(args->pipe_write->write_edge >= args->pipe_write->read_edge){
            active = my_read(file_dir, &byte, 1, &read);
            if(!active)
                break;
            /* p_write =  */pipe_write(args->pipe_write, byte); // prepei na baloyme sinthiki gia na stamataei kai na perimenei na diabasei o allos prin ta kanei over write 
            
            //printf("%c", byte);
        }
        else{
            printf("wait\n");                               ///////EDO GINETAI TO WAIT- SVHNOUME TO ELSE///////
            break;
        }
    }
    printf("\n");

    //pipe write done
    pipe_writeDone(args->pipe_write);
    printf("%d\n", args->pipe_write->write_open);

    //check pipe 2

    return NULL;
}

void* thread2(void* arg){
    printf("Im thread 2\n");

    return NULL;
}

int main(int argc, char *argv[]) {
    pipeT** pipebase;
    pthread_t p1, p2;
    thread_argsT thread1_args, thread2_args;
    int i;

    // check if starting args are correct
    if(argc != 2)
        return(1);

    //allocate memory for the pipebase
    pipebase = (pipeT**) malloc(NUM_OF_PIPES*sizeof(pipeT*));
    if(pipebase == NULL) return(-1);

    //create the pipes
    for(i=0; i<NUM_OF_PIPES; i++){
        pipebase[i] = pipe_open(PIPE_SIZE);
        printf("pipe[%d] : %p, size: %d, write open: %d\n", i, pipebase[i], pipebase[i]->size, pipebase[i]->write_open);
    }

    //initialize thread1's arguements
    thread1_args.file_name = (char*) malloc(sizeof(char)*(strlen(argv[1])+1));
    if(thread1_args.file_name == NULL){
        perror("file name:");
        return(-1);
    }

    strcpy(thread1_args.file_name, argv[1]);
    thread1_args.pipe_write = pipebase[0];
    thread1_args.pipe_read = pipebase[1];

    //initialize thread2's arguements
    thread2_args.file_name = thread1_args.file_name;
    thread2_args.pipe_write = pipebase[1];
    thread2_args.pipe_read = pipebase[0];
    
    //create  threads and wait them to finish
    pthread_create(&p1, NULL, thread1, &thread1_args);
    pthread_create(&p2, NULL, thread2, &thread2_args);

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    //remember to free memory!!!!!! - for pipebase!!!
    free(thread1_args.file_name);
    return 0;
}