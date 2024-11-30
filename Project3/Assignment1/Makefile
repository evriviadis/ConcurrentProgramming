test: test.o lib1.o
gcc -Wall -fsanitize=address -g test.o lib1.o -o test

test.o: test.c lib1.h
gcc -Wall -fsanitize=address -g -c test.c

lib1.o: lib1.c lib1.h
gcc -Wall -fsanitize=address -g -c lib1.c