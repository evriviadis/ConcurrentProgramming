#include "library.h"
#include "../coroutines.h"

co_t coroutine1, coroutine2;

void* thread1(void* arg){
    printf("Hi I'm thread 1\n");
    int file_dir, read=0, p_write;
    char byte='\0'; 
    int writeCycles, readCycles, writeEdge, readEdge;
    thread_argsT* args = (thread_argsT *) arg;  
    int fileCopy2Length, copy2_fd;
    char *fileCopy2;
    int writeResult, readResult;
    
    //open file1
    file_dir = open(args->file_name, O_RDONLY);
    if (file_dir == -1) {
        perror("file");
        args->finished = 1;
        return(NULL);
    }

    while(1) {

        writeCycles = args->pipe_write->cyclesWrite;
        readCycles = args->pipe_write->cyclesRead;
        writeEdge = args->pipe_write->write_edge;
        readEdge = args->pipe_write->read_edge;
        
        //printf("1|write| readCyc: %d writeCyc: %d,  write_edge: %d, read_edge: %d\n", readCycles, writeCycles, writeEdge, readEdge);
        
        if ( ( (writeCycles == readCycles) && (writeEdge >= readEdge) ) || ( (writeCycles > readCycles) && (writeEdge < readEdge) ) ) {
            if (!my_read(file_dir, &byte, 1, &read))
                break;
            p_write = pipe_write(args->pipe_write->id, byte); 
            
            if(p_write == -1){
                args->finished = 1;
                return(NULL);
            }
        }
        else {
            printf("1|write| thread 1 is waitting...\n");                   
        }
    }
    printf("\n");

    //pipe write done
    pipe_writeDone(args->pipe_write->id);
    printf("1|write| pipe write done..write open is now: %d\n", args->pipe_write->write_open);

    //check pipe 2
    //Creates the new file's name
    fileCopy2Length = strlen(args->file_name) + strlen(".copy2") + 1;
    fileCopy2 = (char *)malloc(fileCopy2Length * sizeof(char));
    strcpy(fileCopy2, args->file_name);
    strcat(fileCopy2, ".copy2");
    
    //Opens/creates file 
    copy2_fd = open(fileCopy2, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (copy2_fd == -1){
        perror("copy2");
        args->finished = 1;
        return(NULL);
    }

    while(1){
        writeCycles = args->pipe_read->cyclesWrite;
        readCycles = args->pipe_read->cyclesRead;
        writeEdge = args->pipe_read->write_edge;
        readEdge = args->pipe_read->read_edge;

        //printf("1|read| readCyc: %d writeCyc: %d,  write_edge: %d, read_edge: %d\n", readCycles, writeCycles, writeEdge, readEdge);
    
        //args->pipe_write->write_edge > args->pipe_write->read_edge
        if ( ( (writeCycles == readCycles) && (writeEdge > readEdge) ) || ( (writeCycles > readCycles) && (writeEdge <= readEdge) )  ) {
           
            readResult = pipe_read(args->pipe_read->id, &byte);
           
            if(readResult == 1){
           
                //printf("1|read| character to write %c\n",byte);
                writeResult = my_write(&copy2_fd, &byte, 1);
                if(writeResult == -1){
                    args->finished = 1;
                    return(NULL);
                }
            }else if(readResult == 0){
           
                //an epistrefei 0 kleinei to pipe
                printf("1|read| pipe must be destroyed\n");
                break;
            }
            
        }else{
            printf("1|read| thread 1 is waitting...\n");
            printf("1|read| is pipe write open? : %d\n", args->pipe_read->write_open);
        }

        if((!args->pipe_read->write_open) && (args->pipe_read->read_edge == args->pipe_read->write_edge) && (args->pipe_read->cyclesRead == args->pipe_read->cyclesWrite)){
            printf("1|read| read from pipe 2 done\n");
            break;
        }
    }

    free(fileCopy2);
    args->finished = 1;
    return NULL;
}

void* thread2(void* arg){
    printf("Hi I'm thread 2\n");
    thread_argsT* args;
    args = (thread_argsT *)arg;
    int readResult= -34, copy_fd, writeResult, read=0;
    char byte = '\0';
    int writeCycles, readCycles, writeEdge, readEdge;
    int fileCopyLength;
    char *fileCopy;

    //Creates the new file's name
    fileCopyLength = strlen(args->file_name) + strlen(".copy") + 1;
    fileCopy = (char *)malloc(fileCopyLength * sizeof(char));
    strcpy(fileCopy, args->file_name);
    strcat(fileCopy, ".copy");
    
    //Opens/creates file 
    copy_fd = open(fileCopy, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (copy_fd == -1){
        perror("copy");
        args->finished = 1;
        return(NULL);
    }

    while(1){
        writeCycles = args->pipe_read->cyclesWrite;
        readCycles = args->pipe_read->cyclesRead;
        writeEdge = args->pipe_read->write_edge;
        readEdge = args->pipe_read->read_edge;

        //printf("2|read| readCyc: %d writeCyc: %d,  write_edge: %d, read_edge: %d\n", readCycles, writeCycles, writeEdge, readEdge);
    
        if ( ( (writeCycles == readCycles) && (writeEdge > readEdge) ) || ( (writeCycles > readCycles) && (writeEdge <= readEdge) )  ) {
           
            readResult = pipe_read(args->pipe_read->id, &byte);
            if(readResult == 1){
           
                //printf("2|read| character to write %c\n",byte);
                writeResult = my_write(&copy_fd, &byte, 1);
                if(writeResult == -1){
                    args->finished = 1;
                    return(NULL);
                }
            }else if(readResult == 0){
           
                //an epistrefei 0 kleinei to pipe
                printf("2|read| pipe must be destroyed\n");
                break;
            }
        }else{
            printf("2|read| thread 2 is waitting...\n");
            printf("2|read| is pipe write open?: %d\n", args->pipe_read->write_open);
        }

        if((!args->pipe_read->write_open) && (args->pipe_read->read_edge == args->pipe_read->write_edge) && (args->pipe_read->cyclesRead == args->pipe_read->cyclesWrite)){
            printf("2|read| read from pipe 1 done\n");
            break;
        }
    }

    if(close(copy_fd) == -1){
        perror("close file:");
        args->finished = 1;
        return(NULL);
    }

    //WRITTNG IN PIPE 2
    copy_fd = open(fileCopy, O_RDWR , 0644);
    if (copy_fd == -1){
        perror("copy");
        args->finished = 1;
        return(NULL);
    }

    while(1)
    {
        writeCycles = args->pipe_write->cyclesWrite;
        readCycles = args->pipe_write->cyclesRead;
        writeEdge = args->pipe_write->write_edge;
        readEdge = args->pipe_write->read_edge;

        //printf("2|write| readCyc: %d writeCyc: %d,  write_edge: %d, read_edge: %d\n", readCycles, writeCycles, writeEdge, readEdge);
        
        if ( ( (writeCycles == readCycles) && (writeEdge >= readEdge) ) || ( (writeCycles > readCycles) && (writeEdge < readEdge) ) ) {
            int resultofread = my_read(copy_fd, &byte, 1, &read);
            //printf("2|writeeeeee| resultofread: %d\n\n\n",resultofread);
            if(resultofread==0){
                break;
            }
            
            //printf("2|write| THIS IS THE BYTE THAT I JUST READ: %c\n",read);
            if(pipe_write(args->pipe_write->id, byte) == -1){
                args->finished = 1;
                return(NULL);
            }
        }else{
            printf("2|write| thread 2 is waitting...\n");                   
        }
    }

    //pipe write done
    pipe_writeDone(args->pipe_write->id);
    printf("2|write| girizoume to write open se %d\n", args->pipe_write->write_open);

    free(fileCopy);
    args->finished = 1;
    return (NULL);
}

int main(int argc, char *argv[]) {
    pthread_t p1, p2;
    thread_argsT thread1_args, thread2_args;

    // check if starting args are correct
    if(argc != 2)
        return(1);

    mycoroutines_init(&main_co);

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
    thread1_args.finished = 0;

    //initialize thread2's arguements
    thread2_args.file_name = thread1_args.file_name;
    thread2_args.pipe_write = pipebase[1];
    thread2_args.pipe_read = pipebase[0];
    thread2_args.finished = 0;
    
    //create threads and wait them to finish
   /*  pthread_create(&p1, NULL, thread1, &thread1_args);
    pthread_create(&p2, NULL, thread2, &thread2_args); */
    

    mycoroutines_create(&coroutine1, thread1, &coroutine2);
    mycoroutines_create(&coroutine2, thread2, &coroutine1);

    mycoroutines_switchto(&coroutine1);
    mycoroutines_switchto(&coroutine2);

    //Waiting for threads to finish
    mycoroutines_destroy(&coroutine1);
    mycoroutines_destroy(&coroutine2);

    //remember to free memory!!!!!! - for pipebase!!!
    free(thread1_args.file_name);
    return 0;
}