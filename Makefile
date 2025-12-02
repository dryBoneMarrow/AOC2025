all: 01 02
01: 01/input 01/main
02: 02/input 02/main

01/main:
	cc 01/main.c -o 01/main
02/main:
	cc -lm 02/main.c -o 02/main
	
