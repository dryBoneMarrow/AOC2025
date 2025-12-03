all: 01 02

01: 01/input 01/main
02: 02/input 02/main

01/main: 01/main.c
	cc 01/main.c -o 01/main
02/main: 02/main.c
	cc 02/main.c -o 02/main
	
clean:
	rm 01/main
	rm 02/main
