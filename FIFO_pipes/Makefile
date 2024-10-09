program: program.o library.o
	gcc -Wall -fsanitize=address -g program.o library.o -o program

program.o: program.c library.h
	gcc -Wall -fsanitize=address -g -c program.c
	
library.o: library.c library.h
	gcc -Wall -fsanitize=address -g -c library.c
