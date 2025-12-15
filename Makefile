CC = gcc
CFLAGS = -std=c23 -O3 -march=native

CHALLENGES =  01 02 03 04 05 06 07 08
all: $(CHALLENGES)

# https://www.gnu.org/software/make/manual/make.html#Static-Pattern
$(CHALLENGES): %: %/input %/main

%/main: %/main.c
	$(CC) $< -o $@ $(CFLAGS)


clean:
#https://www.gnu.org/software/make/manual/make.html#Substitution-Refs
	rm -f $(CHALLENGES:%=%/main)
