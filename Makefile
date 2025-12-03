CC = gcc
CFLAGS = -std=c23

all: 01 02 03

01: 01/input 01/main
02: 02/input 02/main
03: 03/input 03/main

01/main: 01/main.c
	$(CC) 01/main.c -o 01/main $(CFLAGS)
02/main: 02/main.c
	$(CC) 02/main.c -o 02/main $(CFLAGS)
03/main: 03/main.c
	$(CC) 03/main.c -o 03/main $(CFLAGS)
	
clean:
	rm 01/main
	rm 02/main
	rm 03/main
