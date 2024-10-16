#include "library.h"
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    //File name is taken as arguement
    if (argc != 2) {
        printf("Wrong number of arguemets\n");
        return(-1);
    }

    int number, read_result, i=0, array_size=0, read=0;
    char *buffer = (char *) malloc(sizeof(char));
    int *array = (int *) malloc(array_size*sizeof(int));
    int file_dir = open(argv[1], O_RDONLY);

    while (1) {
        read_result = my_read(file_dir, &buffer[i], 1, &read);

        if (!read_result){
            //File is fully read
            number = atoi(buffer);
            array_size++;
            array = (int *) realloc(array, array_size*sizeof(int));
            array[array_size-1] = number;
            
            close(file_dir);
            free(buffer);
            break;
        }

        //When file_dir finds space or \n that's the end
        if ((buffer[i] == '\n') || (buffer[i] == ' ')) {
            i = 0;
            number = atoi(buffer);
            array_size++;
            array = (int *) realloc(array, array_size*sizeof(int));
            array[array_size-1] = number;
            buffer = (char *) realloc(buffer, sizeof(char));
        }else{
            i++;
            buffer = (char *) realloc(buffer,(i+1)*sizeof(char));
        }
    }

    printf("- - - BEFORE SORTING - - -\n");
    for (int i = 0; i < array_size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n\n");
    
    //Initialize matrix
    arrayT* matrix = (arrayT *) malloc(sizeof(arrayT));
    matrix->array = array;
    matrix->length = array_size;
    matrix->left = 0;

    //Calliing parallel merge
    parallel_merge(matrix);

    printf("- - - AFTER SORTING - - - %d\n", matrix->length);
    for (int i = 0; i < array_size; i++) {
        printf("%d ", matrix->array[i]);
    }
    printf("\n");

    free(array);
    free(matrix);
    return 0;
}
