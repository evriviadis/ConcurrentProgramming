#include "library.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/*This function merges two sorted arrays*/
void merge(int arr[], int left, int mid, int right) {
    int i, j;

    int n1 = mid - left + 1;  // Size of the left half
    int n2 = right - mid;     // Size of the right half
    int L[n1], R[n2];

    // Copy data to temporary arrays L[] and R[]
    for(int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for(int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    // Merge the temporary arrays back into arr[left..right]
    i = 0;
    j = 0; 
    int k = left;

    // Merge process
    while(i < n1 && j < n2){
        if(L[i] <= R[j]){
            arr[k] = L[i];
            i++;
        }else{
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy remaining elements of L[] and R[], if any
    while(i < n1){
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
    return;
}

/*This function implements MergeSort algorithm*/
void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        //Recursively sort the first and second halves
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        //Merge the sorted halves
        merge(arr, left, mid, right);
    }
    return;
}

/*This function merges an array using threads*/
void* parallel_merge(void* arg) {
    pthread_t t1, t2;
    arrayT *matrix = (arrayT*) arg;
    //int  i;
   
   //Matrix has size greater than MIN_SIZE
    if (matrix->length > MIN_SIZE) {
        arrayT *matrix1 = (arrayT *) malloc(sizeof(arrayT));
        arrayT *matrix2 = (arrayT *) malloc(sizeof(arrayT));
        
        //Initialize the recrusively made array halves
        matrix1->length = matrix->length / 2;
        matrix1->left = matrix->left;
        matrix2->length = matrix->length - matrix1->length;
        matrix2->left = matrix->left;
        matrix1->array = matrix->array;
        matrix2->array = matrix->array + matrix1->length;

        /*
        GIA TON AGAPHMENO EVRI
        printf("|1| matrix: len: %d, left %d\n", matrix1->length, matrix1->left);
        for (i = 0; i < matrix1->length; i++) {
            printf("%d ", matrix1->array[i]);
        }
        printf("\n\n");

        printf("|2| matrix: len: %d, left %d\n", matrix2->length, matrix2->left);
        for (i = 0; i < matrix2->length; i++) {
            printf("%d ", matrix2->array[i]);
        }
        printf("\n\n");
        */

       //Create recursively threads to sort the array
        pthread_create(&t1, NULL, parallel_merge, matrix1);
        pthread_create(&t2, NULL, parallel_merge, matrix2);

        //Wait for threads to finish
        pthread_join(t1, NULL);
        pthread_join(t2, NULL);

        //Recursively merge two already sorted arrays
        merge(matrix->array, matrix->left, matrix->length/2-1, matrix->length - 1);

        free(matrix1);
        free(matrix2);
    }else{ 
        mergeSort(matrix->array, matrix->left, matrix->length - 1);
    }
    return(NULL);
}

/*This function uses read and check if it opperates properly*/
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
