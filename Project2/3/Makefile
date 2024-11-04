bridge: bridge.o library.o lib3.o
	gcc -Wall -fsanitize=address -g bridge.o library.o lib3.o -o bridge -lm

bridge.o: bridge.c lib3.h
	gcc -Wall -fsanitize=address -g -c bridge.c
	
lib3.o: lib3.c lib3.h
	gcc -Wall -fsanitize=address -g -c lib3.c -lm

library.o: ../1/library.c ../1/library.h
	gcc -Wall -fsanitize=address -g -c ../1/library.c
