main: main.o coroutines.o
	gcc -Wall -g main.o coroutines.o -o main

main.o: main.c coroutines.h
	gcc -Wall -g -c main.c
	
coroutines.o: coroutines.c coroutines.h
	gcc -Wall -g -c coroutines.c