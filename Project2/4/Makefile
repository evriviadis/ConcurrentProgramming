train: train.o library.o lib4.o
	gcc -Wall -fsanitize=address -g train.o library.o lib4.o -o train -lm

train.o: train.c lib4.h
	gcc -Wall -fsanitize=address -g -c train.c
	
lib4.o: lib4.c lib4.h
	gcc -Wall -fsanitize=address -g -c lib4.c -lm

library.o: ../1/library.c ../1/library.h
	gcc -Wall -fsanitize=address -g -c ../1/library.c
