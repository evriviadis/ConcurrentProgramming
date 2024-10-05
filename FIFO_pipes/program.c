#include "library.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>

void* thread1(void* arg){
    printf("Im thread 1\n");
    int file_dir, characters_read;
    int startReadIndex = 0, *fd;
    
    //open file1
    file_dir = open(arg, O_RDONLY);
    if (file_dir == -1) {
        perror("");
        return(-1);
    }

    char* buffer = (char*) malloc(64 * sizeof(char));
    fd = pipe_open(64);

    do {
        characters_read = my_read(file_dir, buffer, 64, &startReadIndex);
        startReadIndex += characters_read;

        while (characters_read > 0) {
            pipe_write(&fd, buffer[64 - characters_read]);
            characters_read--;
        }
    } while (characters_read != 64);

    return NULL;
}

void* thread2(void* arg){
    print("Im thread 2\n");

    return NULL;
}


int main(int argc, char *argv[]) {
    if (argc != 2) return(1);
    pthread_t p1, p2;
    int file_dir, read_file, read_edge=0;
    char exp_buf[64]={'\0'};

    pthread_create(&p1, NULL, thread1, NULL);
    pthread_create(&p2, NULL, thread2, NULL);

    pthread_join(p1,NULL);
    pthread_join(p2,NULL);

    printf("this is from main\n");

    /* file_dir = open(argv[1], O_RDONLY);
    if (file_dir == -1) {
        perror("");
        return(-1);
    } */

    read_file = my_read(file_dir, exp_buf, 64, &read_edge);
    printf("%d characters read\n", read_file);

    printf("%s\n", exp_buf);

    return 0;
}