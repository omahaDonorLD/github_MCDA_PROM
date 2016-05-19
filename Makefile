################################################################
#                  VARIABLES DEFINITIONS                       #
################################################################

# Compilator
CPP = g++
C = gcc

# Options of compilation
CFLAGS = -Wall -Wextra -pedantic -ansi

# use of c99 version
C99 = -std=c99

# include standard math library
LDFLAGS += -lm

# Options of compilation for performances' optimisation
OPTIMIZATION = -O3
DEBUG = -g

# Folder who contained files to include
INCLUDE = -I .

# Finals executables
AIMS = MCDA

################################################################
#                  COMMANDS OF COMPILATION                     #
################################################################

all: $(AIMS)

MCDA: main.o expert.o
	$(C) $(C99) $(DEBUG) -o $@ $^ $(LDFLAGS) $(CFLAGS)
#	$(C) $(DEBUG) -o MCDA main.o $(CFLAGS) $(OPTIMIZATION)

expert.o : expert.c expert.h
	$(C) $(C99) $(DEBUG) -o $@ -c $< $(LDFLAGS) $(CFLAGS)
#	$(C) $(DEBUG) -o expert.o -c expert.c $(CFLAGS) $(OPTIMIZATION)

main.o : main.c expert.h main.h
	$(C) $(C99) $(DEBUG) -o $@ -c $< $(LDFLAGS) $(CFLAGS)
#	$(C) $(DEBUG) -o main.o -c main.c $(CFLAGS) $(OPTIMIZATION)

clean :
	rm -rf *.o

mrproper: clean
	rm -rf $(AIMS)
	
blank:
	tput clear
