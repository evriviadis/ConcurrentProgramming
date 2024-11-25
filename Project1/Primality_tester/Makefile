primality_tester: primality_tester.o library.o
	gcc -Wall -fsanitize=address -g primality_tester.o library.o -o primality_tester -lm

primality_tester.o: primality_tester.c library.h
	gcc -Wall -fsanitize=address -g -c primality_tester.c
	
library.o: library.c library.h
	gcc -Wall -fsanitize=address -g -c library.c -lm


