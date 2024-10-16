#define LIBRARY_H

#define MIN_SIZE 4

/*ARRAY TO BE SORTED*/
typedef struct {
    int *array;
    int length;
    int left;   //array's start index
} arrayT;

/*FUNCTIONS' PROTOTYPES*/
extern int my_read(int fd, void *buffer, int size, int *left);
extern void merge(int arr[], int left, int mid, int right);
extern void mergeSort(int arr[], int left, int right);
extern void* parallel_merge(void* arg);

#endif
