#include "library.h"
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>

int my_read(int fd, void *buffer, int size, int *left){
   int res, read_already=0;

    do{
        res = read(fd, buffer+(read_already*sizeof(char)), size-read_already);
        if (res == -1){

            return(-1);
        }else if(res == 0){
            *left = size-read_already;
            return(read_already);
        }

        read_already += res;
    }while(read_already < size);
    return(read_already); 
};

void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;  // Size of the left half
    int n2 = right - mid;     // Size of the right half

    int L[n1], R[n2];

    // Copy data to temporary arrays L[] and R[]
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    // Merge the temporary arrays back into arr[left..right]
    int i = 0, j = 0, k = left;

    // Merge process
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy remaining elements of L[], if any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy remaining elements of R[], if any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        // Recursively sort the first and second halves
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        // Merge the sorted halves
        merge(arr, left, mid, right);
    }
}

void* parallel_merge(void* arg) {
    arrayT *matrix = (arrayT*) arg;
    printf("arxiko matrix: len: %d, left %d\n", matrix->length, matrix->left);
    for (int i = 0; i < matrix->length; i++) {
        printf("%d ", matrix->array[i]);
    }
    printf("\n\nmpainei if \n\n");

    if (matrix->length > MIN_SIZE) {
        arrayT *matrix1 = (arrayT *) malloc(sizeof(arrayT));
        arrayT *matrix2 = (arrayT *) malloc(sizeof(arrayT));
        
        matrix1->length = matrix->length / 2;
        matrix1->left = matrix->left;
        matrix2->length = matrix->length - matrix1->length;
        matrix2->left = matrix->left + matrix1->length;
        matrix1->array = matrix->array;
        matrix2->array = matrix->array + matrix1->length;

        /*for (int i = 0; i < matrix1->length; i++) {
            printf("%d ", matrix1->array[i]);
        }
        printf("----");
        for (int i = 0; i < matrix2->length; i++) {
            printf("%d ", matrix2->array[i]);
        }
        printf("\n");*/

        pthread_t t1, t2;
        pthread_create(&t1, NULL, parallel_merge, matrix1);
        pthread_create(&t2, NULL, parallel_merge, matrix2);
        pthread_join(t1, NULL);
        pthread_join(t2, NULL);

        printf("\n\nmeta ta threads -> left : %d, mid: %d, ri %d\n", matrix->left, matrix->left + (int) (matrix->length/2), matrix->left + matrix->length - 1);
        //merge(matrix->array, matrix->left, matrix->left + matrix->length / 2 - 1, matrix->left + matrix->length - 1);

        /*for (int i = 0; i < matrix->length; i++) {
            printf("%d ", matrix->array[i]);
        }
        printf("\n");*/

        free(matrix1);
        free(matrix2);
    }
    else { // if the matrix has size <= MIN_SIZE
        printf("edv ginetai mergesort\n");
        printf("left %d right %d\n", matrix->left, matrix->left + matrix->length - 1);
        for (int i = 0; i < matrix->length-1; i++) {
           printf("edo skaei %d\n ", matrix->array[i]);
        }
        //mergeSort(matrix->array, matrix->left, matrix->left + matrix->length - 1);
        for (int i = 0; i < matrix->length; i++) {
            printf("%d ", matrix->array[i]);
        }
        printf("\n\nend of ms\n\n");
        return(NULL);
    }
    return(NULL);
}