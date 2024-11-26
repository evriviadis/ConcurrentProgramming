primality_tester: primality_tester.o library.o lib2.o
	gcc -Wall -fsanitize=address -g primality_tester.o library.o lib2.o -o primality_tester -lm

primality_tester.o: primality_tester.c lib2.h
	gcc -Wall -fsanitize=address -g -c primality_tester.c
	
lib2.o: lib2.c lib2.h
	gcc -Wall -fsanitize=address -g -c lib2.c -lm

library.o: ../1/library.c ../1/library.h
	gcc -Wall -fsanitize=address -g -c ../1/library.c
