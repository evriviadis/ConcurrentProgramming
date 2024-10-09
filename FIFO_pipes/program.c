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
    int file_dir, read=0, p_write /*  active*/;
    char byte='\0'; 
    int writeCycles, readCycles, writeEdge, readEdge;
    thread_argsT* args = (thread_argsT *) arg;  
    
    //open file1
    file_dir = open(args->file_name, O_RDONLY);
    if (file_dir == -1) {
        perror("file");
        return(NULL);
    }

    while(1) {

        writeCycles = args->pipe_write->cyclesWrite;
        readCycles = args->pipe_write->cyclesRead;
        writeEdge = args->pipe_write->write_edge;
        readEdge = args->pipe_write->read_edge;
        
        printf("1| readCyc: %d writeCyc: %d,  write_edge: %d, read_edge: %d\n", readCycles, writeCycles, writeEdge, readEdge);
        //printf("1 write_edge: %d\n", writeEdge);
        //printf("1 read_edge: %d\n", readEdge);
        
        if ( ( (writeCycles == readCycles) && (writeEdge >= readEdge) ) || ( (writeCycles > readCycles) && (writeEdge < readEdge) ) ) {
            if (!my_read(file_dir, &byte, 1, &read))
                break;
            p_write = pipe_write(args->pipe_write->id, byte); // prepei na baloyme sinthiki gia na stamataei kai na perimenei na diabasei o allos prin ta kanei over write 
            //printf("%c", byte);
        }
        else {
            printf("thread 1 is waitting...\n");  
            //printf("buffer from thread 1: %s\n",args->pipe_write->buffer);     
            sleep(2);                     ///////EDO GINETAI TO WAIT- SVHNOUME TO ELSE///////
        }
    }
    printf("\n");


    //pipe write done
    pipe_writeDone(args->pipe_write->id);
    printf("1| girizoume to write open se %d\n", args->pipe_write->write_open);

    //check pipe 2

    return NULL;
}

void* thread2(void* arg){
    printf("Hi I'm thread 2\n");
    thread_argsT* args;
    args = (thread_argsT *)arg;
    int readResult= -34;
    char byte = '\0';
    int writeCycles, readCycles, writeEdge, readEdge;

    while(1) {
        writeCycles = args->pipe_read->cyclesWrite;
        readCycles = args->pipe_read->cyclesRead;
        writeEdge = args->pipe_read->write_edge;
        readEdge = args->pipe_read->read_edge;

        printf("2| readCyc: %d writeCyc: %d,  write_edge: %d, read_edge: %d\n", readCycles, writeCycles, writeEdge, readEdge);
        //printf("2 readcycl: %d writecycl: %d\n", readCycles, writeCycles);
        //printf("2 write_edge: %d\n", writeEdge);
        //printf("2 read_edge: %d\n", readEdge);
    
        //args->pipe_write->write_edge > args->pipe_write->read_edge
            if ( ( (writeCycles == readCycles) && (writeEdge > readEdge) ) || ( (writeCycles > readCycles) && (writeEdge <= readEdge) )  ) {
                readResult = pipe_read(args->pipe_read->id, &byte);
                if(readResult == 0){
                    printf("MPAGASA TO EXOUME \n");
                    break;
                }
                //an epistrefei 0 kleinei to pipe
            }else{
                printf("2| result: %d\n",readResult);
                printf("2| thread 2 is waitting...\n");
                printf("2| write is open:??: %d\n", args->pipe_read->write_open);
                sleep(1);
            }

            if((!args->pipe_read->write_open) && (args->pipe_read->read_edge == args->pipe_read->write_edge) && (args->pipe_read->cyclesRead == args->pipe_read->cyclesWrite)){
                printf("2| pame gia ipno\n");
                break;
            }
    }

    return (NULL);
}

int main(int argc, char *argv[]) {
    pthread_t p1, p2;
    thread_argsT thread1_args, thread2_args;

    // check if starting args are correct
    if(argc != 2)
        return(1);

    //allocate memory for the pipebase
    pipebase = (pipeT**) malloc(NUM_OF_PIPES*sizeof(pipeT*));
    if (pipebase == NULL) return(-1);

    //create the pipes
    int pipeId1 = pipe_open(PIPE_SIZE);
    printf("pipe[%d] : %p, size: %d, write open: %d\n", pipeId1, pipebase[pipeId1], pipebase[pipeId1]->size, pipebase[pipeId1]->write_open);

    int pipeId2 = pipe_open(PIPE_SIZE);
    printf("pipe[%d] : %p, size: %d, write open: %d\n", pipeId2, pipebase[pipeId2], pipebase[pipeId2]->size, pipebase[pipeId2]->write_open);

    //initialize thread1's arguements
    thread1_args.file_name = (char*) malloc(sizeof(char)*(strlen(argv[1])+1));
    if (thread1_args.file_name == NULL) {
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
    
    //create threads and wait them to finish
    pthread_create(&p1, NULL, thread1, &thread1_args);
    pthread_create(&p2, NULL, thread2, &thread2_args);

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    //remember to free memory!!!!!! - for pipebase!!!
    free(thread1_args.file_name);
    
    return 0;
}