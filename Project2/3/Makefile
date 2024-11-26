bridge: bridge.o ../../Project3/Assignment1/lib1.o lib3.o
	gcc -Wall -fsanitize=address -g bridge.o ../../Project3/Assignment1/lib1.o lib3.o -o bridge

bridge.o: bridge.c lib3.h
	gcc -Wall -fsanitize=address -g -c bridge.c
	
lib3.o: lib3.c lib3.h
	gcc -Wall -fsanitize=address -g -c lib3.c

lib1.o: ../../Project3/Assignment1/lib1.c ../../Project3/Assignment1/lib1.h
	gcc -Wall -fsanitize=address -g -c ../../Project3/Assignment1/lib1.c 