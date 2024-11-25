binary_sems: binary_sems.o library.o
	gcc -Wall -fsanitize=address -g binary_sems.o library.o -o binary_sems

binary_sems.o: binary_sems.c library.h
	gcc -Wall -fsanitize=address -g -c binary_sems.c
	
library.o: library.c library.h
	gcc -Wall -fsanitize=address -g -c library.c
