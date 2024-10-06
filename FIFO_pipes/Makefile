program: program.o library.o
	gcc -Wall -g program.o library.o -o program

program.o: program.c library.h
	gcc -Wall -g -c program.c
	
library.o: library.c library.h
	gcc -Wall -g -c library.c
