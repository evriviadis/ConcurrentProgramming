#ifndef LIBRARY_H
#define LIBRARY_H

#define MIN_SIZE 64

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

/*FILE TO BE SORTED*/
typedef struct {
    char* file_name;
    int size;
    int end_fd;     //file's end index
    int start_fd;   //file's start index
    int finish;
    int offset;
} thread_argsT;


/*FUNCTIONS' PROTOTYPES*/
extern int my_read(int fd, void *buffer, int size, int *left);
extern int my_write(int *fd, void *buffer, int size);
extern void merge(int arr[], int left, int mid, int right);
extern void mergeSort(int arr[], int left, int right);
extern void* parallel_merge(void* arg);
#endif