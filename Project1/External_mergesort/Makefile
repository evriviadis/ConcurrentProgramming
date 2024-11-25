mergeSort: mergeSort.o library.o
	gcc -Wall -fsanitize=address -g mergeSort.o library.o -o mergeSort

mergeSort.o: mergeSort.c library.h
	gcc -Wall -fsanitize=address -g -c mergeSort.c
	
library.o: library.c library.h
	gcc -Wall -fsanitize=address -g -c library.c
