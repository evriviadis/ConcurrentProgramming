#include "library.h"
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]) {
    // File name is taken as arguement
    if(argc != 2){
        printf("Wrong number of arguemets\n");
        return(-1);
    }

    thread_argsT* file = (thread_argsT*) malloc(sizeof(thread_argsT));
    if(file == NULL){
        return(0);
    }

    // Initialization
    file->file_name = (char*) malloc(sizeof(char) * (strlen(argv[1])+1));
    if(file->file_name == NULL){
        perror("file name:");
        return(-1);
    }
    strcpy(file->file_name, argv[1]);

    // Open a file multiple times to monitor it's start mid end
    file->start_fd = open(file->file_name, O_RDWR);
    file->end_fd = open(file->file_name, O_RDWR);
    if(file->start_fd < 0 || file->end_fd < 0){
        perror("open");
        return(-1);
    }

    file->offset = 0;
    file->size = lseek(file->end_fd, 0, SEEK_END);
    file->size = file->size / sizeof(int);
    printf("size: %d\n", file->size);

    //callinggg
    parallel_merge(file);

    int *buffer;
    buffer = (int*)malloc(sizeof(int)*(file->size));
    if(buffer == NULL){
        perror("malloc:");
        return(-1);
    }

    int left=0;
    my_read(file->start_fd, buffer, sizeof(int)*file->size, &left);
    lseek(file->start_fd, -sizeof(int)*file->size, SEEK_CUR);

    printf("\nTHIS IS FROM MAIN - AFTER MERGE - HOPEFULLY PERFECT\n");
    for(int i=0; i<file->size; i++){
        printf("%d ", buffer[i]);
    }
    printf("\n");

    close(file->start_fd);
    close(file->end_fd);

    free(buffer);
    free(file->file_name);
    free(file);
    return(0);
}