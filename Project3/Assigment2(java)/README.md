CONCURRENT PROGRAMING - ASSIGNMENT SERIES 3 - PRIMALITY TESTER

DESCRIPTION
This program takes positive integers as input and checks if they are prime or not. What's special about this, is that main doesn't execute the checks. Instead, it assigns them to N worker threads that have been created to check the primality. In that way, the analysis becomes faster. Threads can check none, one or many numbers for primality. This depends on main's assignments, on how fast a thread finishes and if there are
many threads available to test numbers concurrently.


DEVELOPMENT ENVIRONMENT
Programming language: C
Operating System: Linux
Compiler: GCC


COMPILATION PROCESS
Ensure Java is instaled!
Compile the program:
    On a terminal go to the directory named Assignment2.
    Compile with the command: javac App.java
    Then, the executable file named "App.class" is created.
    Run with the command: java App


RUNNING PROCESS
Create a file in the same directory named "input.txt" 
that contains positive integers seperated with ' ' or '\n'. 
Output can be displayed on screen (stdout).
The output depicts if a number is prime or if it is not.
Run the program with the command: java App N, N equals to worker threads.


AUTHORS AND ACKNOWLEDGEMENT
~ Liapis Evriviadis 03646
~ Plytas Evaggelos 03674
~ Tsiaousi Aikaterini 03626
