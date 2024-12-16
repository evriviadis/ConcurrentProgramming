program: program.o library.o coroutines.o
	gcc -Wall -g program.o library.o coroutines.o -o program

program.o: program.c library.h ../coroutines.h
	gcc -Wall -g -c program.c
	
library.o: library.c library.h
	gcc -Wall -g -c library.c

coroutines.o: ../coroutines.c ../coroutines.h
	gcc -Wall -g -c ../coroutines.c