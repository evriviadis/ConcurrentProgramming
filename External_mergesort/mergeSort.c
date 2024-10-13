#include "library.h"
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


int main(int argc, char* argv[]) {
    if(argc != 2){
        perror("Wrong number of args\n");
        return(-1);
    }
    int file_dir, number, read_result, i=0, j=0, read=0;
    char *buffer;
    int *array;
    
    ////// take the array to sort from file /////////

    buffer = (char *)malloc(sizeof(char));
    array = (int *)malloc(j*sizeof(int));
    file_dir = open(argv[1], O_RDONLY);

    while(1){
        read_result = my_read(file_dir, &buffer[i], 1, &read);
        if(!read_result){
            perror("my_read");
            close(file_dir);
            free(buffer);
            break;
        }

        if(buffer[i] == '\0'){
            printf("brikame to telos tou file\n\n");
            close(file_dir);
            free(buffer);
            break;
            /////////   ME AYTO TON TROPO DEN DIABAZEI TO TELEYTAIO NUMBER FIX IT!!!  ////////////
        }
        
        if((buffer[i] == '\n') || (buffer[i] == ' ')){
            i = 0;
            number = atoi(buffer);
            j++;
            array = (int *)realloc(array, j*sizeof(int));
            array[j-1] = number;
            buffer = (char *)realloc(buffer, sizeof(char));
        }else{
            i++;
            buffer = (char *)realloc(buffer,(i+1)*sizeof(char));
        }
    }

    ////// ME AYTO MPOROYME NA PERNOYME TON ARRAY ME NUMBERS APO TO FILE////////


    free(array);
    return 0;
}
