main: main.o lib2.o
	gcc -Wall  -g main.o lib2.o  -o main

main.o: main.c lib2.h
	gcc -Wall -g  -c main.c
	
lib2.o: lib2.c lib2.h
	gcc -Wall -g  -c lib2.c

clean:
	rm -f *.o  main