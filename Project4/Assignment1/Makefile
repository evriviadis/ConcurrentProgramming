test: test.o coroutines.o
	gcc -Wall -g test.o coroutines.o -o test

test.o: test.c coroutines.h
	gcc -Wall -g -c test.c
	
coroutines.o: coroutines.c coroutines.h
	gcc -Wall -g -c coroutines.c
