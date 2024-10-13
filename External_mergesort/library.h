#ifndef LIBRARY_H
#define LIBRARY_H

extern int* merge(int arr[], int left, int mid, int right); 
extern void* thread(void* arg);
extern int my_read(int fd, void *buffer, int size, int *left);

#endif