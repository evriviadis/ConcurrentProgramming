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
    printf("Im thread 1\n");
    int file_dir, characters_read = 0;
    int startReadIndex = 0, fd;
    
    //open file1
    file_dir = open(arg, O_RDONLY);
    if (file_dir == -1) {
        perror("file");
        return((void *)-1);
    }

    char* buffer = (char*) malloc(64 * sizeof(char));

    do {
        memset(buffer, 0, 64);
        characters_read = 0;

        characters_read = my_read(file_dir, buffer, 64, &startReadIndex);
        startReadIndex += characters_read;

        printf("chararcters = %d\n",characters_read);
        printf("file: %s\n", buffer);
        
        for(int i=0; i<characters_read; i++){
            pipe_write(fd, buffer[i]);    
        }
        
    } while (characters_read == 64);

    return NULL;
}

void* thread2(void* arg){
    printf("Im thread 2\n");

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) return(1);
    pthread_t p1, p2;
    pipeT **pipebase;

    pipebase = (pipeT**) malloc(2* sizeof(pipeT*)); //pipebase array allocate
 
    int pipe_id1 = pipe_open(64, );

    pthread_create(&p1, NULL, thread1, argv[1]);
    pthread_create(&p2, NULL, thread2, NULL);

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    printf("this is from main\n");

    return 0;
}