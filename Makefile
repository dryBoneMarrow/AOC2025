CC = cc
CFLAGS = -std=c23 -O3 -march=native
# USE_GLPK = OUI
CHALLENGES =  01 02 03 04 05 06 07 08 09 10 11

all: $(CHALLENGES)

# https://www.gnu.org/software/make/manual/make.html#Static-Pattern
$(CHALLENGES): %: %/input %/main

%/main: %/main.c
	$(CC) $< -o $@ $(CFLAGS)

ifdef USE_GLPK
10/main: 10/main.c
	$(CC) $< -o $@ $(CFLAGS) -DUSE_GLPK=$(USE_GLPK) -lglpk
endif
	
clean:
# https://www.gnu.org/software/make/manual/make.html#Substitution-Refs
	rm -f $(CHALLENGES:%=%/main)
