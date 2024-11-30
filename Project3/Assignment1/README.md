CONCURRENT PROGRAMING - ASSIGNMENT SERIES 3 - BINARY SEMAPHORES (WITH MONITOR)

DESCRIPTION
This program implements the features of binary semaphores using monitors. The features of binary semaphores that are implemented are Initialization, Increase, Decrease and Destroy. To make this happen pthread library is used and especially the mutexes and conditions (pthread_mutex, pthread_cond). This library can be applied in any program that uses binary semaphores.


DEVELOPMENT ENVIRONMENT
Programming language: C
Operating System: Linux
Compiler: GCC


COMPILATION PROCESS
Ensure GCC is instaled:  sudo apt-get install gcc
Compile the program:
    On a terminal go to the directory named Assignment1.
    Compile with the command "make".
    Then, the executable file named "test" is created.

    To compile without the "make" command it is necessary to:
    1. Compile seperately:  gcc -Wall -fsanitize=address -g -c tast.c
                            gcc -Wall -fsanitize=address -g -c lib1.c
    2. Link: gcc -Wall -fsanitize=address -g test.o lib1.o -o test


USAGE
1. Include the header:

   #include "lib1.h"

2. Initialize the semaphore:

   mysem_t semaphore;
   mysem_init(&semaphore, 1);

3. Use semaphore operations:

   mysem_down(&semaphore);
   mysem_up(&semaphore);

4. Destroy the semaphore:

   mysem_destroy(&semaphore);


AUTHORS AND ACKNOWLEDGEMENT
~ Liapis Evriviadis 03646
~ Plytas Evaggelos 03674
~ Tsiaousi Aikaterini 03626