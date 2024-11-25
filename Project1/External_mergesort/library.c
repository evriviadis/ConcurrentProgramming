#include "library.h"
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

/*This function merges two sorted arrays*/
void merge(int arr[], int left, int mid, int right) {
    int i, j;

    int n1 = mid - left + 1;  // Size of the left half
    int n2 = right - mid;     // Size of the right half

    int L[n1], R[n2];

    //Copy data to temporary arrays L[] and R[]
    for(int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for(int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    //Merge the temporary arrays back into arr[left..right]
    i = 0;
    j = 0;
    int k = left;

    //Merge process
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

    //Copy remaining elements of L[] and R[], if any
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
    if (left < right){
        int mid = left + (right - left) / 2;

        //Recursively sort the first and second halves
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        //Merge the sorted halves
        merge(arr, left, mid, right);
    }
    return;
}

/*This function merges a file using threads*/
void* parallel_merge(void* arg){
    pthread_t th1, th2;
    int *buffer, left=0, left_read =0;
    thread_argsT* file = (thread_argsT*) arg;
    thread_argsT *file1, *file2; //left half, right half

    if(file->size > MIN_SIZE){
        //Seperate file in half and create threads
        file1 = (thread_argsT*) malloc(sizeof(thread_argsT));
        file2 = (thread_argsT*) malloc(sizeof(thread_argsT));
        if(file1==NULL || file2==NULL){
            perror("malloc");
            return NULL;
        }        
        file1->finish = 0;
        file2->finish = 0;
        file1->file_name = file->file_name;
        file2->file_name = file->file_name;
        
        // file1
        file1->size = file->size/2;
        file1->start_fd = open(file->file_name, O_RDWR);
        file1->end_fd = open(file->file_name, O_RDWR);
        file1->offset = file->offset;

        lseek(file1->start_fd, file1->offset * sizeof(int), SEEK_SET);
        lseek(file1->end_fd, (file1->offset + file1->size) * sizeof(int), SEEK_CUR);

        // file2
        file2->size = file->size - file1->size;
        file2->start_fd = open(file->file_name, O_RDWR);
        file2->end_fd = open(file->file_name, O_RDWR);
        file2->offset = file->offset + file1->size;
        
        lseek(file2->start_fd, (file2->offset) * sizeof(int), SEEK_SET);
        lseek(file2->end_fd, (file->offset + file2->size) * sizeof(int), SEEK_SET);

        //Create threads and busy wait them to finish
        pthread_create(&th1, NULL, parallel_merge, file1);
        pthread_create(&th2, NULL, parallel_merge, file2);

        while(1){
            if(file1->finish && file2->finish){
                break;
            }
        }

        //Merge the sorted pieces in file
        buffer = (int*)malloc(sizeof(int)*(file->size));
        if(buffer == NULL){
            perror("malloc:");
            return(NULL);
        }

        my_read(file1->start_fd, buffer, sizeof(int)*file1->size, &left_read);
        lseek(file1->start_fd, -sizeof(int)*file1->size, SEEK_CUR);

        my_read(file2->start_fd, buffer+file1->size, sizeof(int)*file2->size, &left_read);
        lseek(file2->start_fd, -sizeof(int)*file2->size, SEEK_CUR);

        merge(buffer, left, file->size/2-1, file->size - 1);

        my_write(&file->start_fd, buffer, sizeof(int)*file->size);
        lseek(file->start_fd, -sizeof(int)*file->size, SEEK_CUR);

        //Close fds and free memory to return
        close(file1->start_fd);
        close(file1->end_fd);
        close(file2->start_fd);
        close(file2->end_fd);
        free(buffer);
        free(file1);
        free(file2);
    }else{
        buffer = (int*)malloc(sizeof(int)*file->size);
        if(buffer == NULL){
            perror("malloc:");
            return(NULL);
        }                       

        my_read(file->start_fd, buffer, sizeof(int)*file->size, &left_read);
        lseek(file->start_fd, -sizeof(int)*file->size, SEEK_CUR);

        mergeSort(buffer, left, file->size-1);

        my_write(&file->start_fd, buffer, sizeof(int)*file->size);
        lseek(file->start_fd, -sizeof(int)*file->size, SEEK_CUR);
        free(buffer);
    }
    file->finish = 1;
    return(NULL);
}

/*This function uses read and check if it opperates properly*/
int my_read(int fd, void *buffer, int size, int *left_read){
   int res, read_already=0;

   do{
      res = read(fd, buffer+(read_already*sizeof(int)), size-read_already);
      if(res == -1)
         return(-1);
      else{
         if(res == 0){
            *left_read = size-read_already;
            return(read_already);
         }
      }
      read_already += res;
   }while(read_already < size);
   return(read_already); 
};

/*This function uses write and checks if it operates properly*/
int my_write(int *fd, void *buffer, int size){
	int res, write_already=0;

	do{
	    res = write(*fd, buffer+(write_already*sizeof(buffer)), size-write_already);
		if(res == -1){
			return(-1);
        }else{ 
            if(res == 0)
			   return(0);
        }
		write_already += res;
	}while(write_already != size);
   return(1);
};