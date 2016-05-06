################################################################
#                  VARIABLES DEFINITIONS                       #
################################################################

# Compilator
CPP = g++
C = gcc

# Options of compilation
CFLAGS = -Wall -Wextra -pedantic -ansi 

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
	$(C) $(DEBUG) -o $@ $^ $(CFLAGS) $(LDFLAGS)
#	$(C) $(DEBUG) -o MCDA main.o $(CFLAGS) $(OPTIMIZATION)

expert.o : expert.c expert.h
	$(C) $(DEBUG) -o $@ -c $< $(CFLAGS) $(LDFLAGS)
#	$(C) $(DEBUG) -o expert.o -c expert.c $(CFLAGS) $(OPTIMIZATION)

main.o : main.cpp expert.h main.h
	$(C) $(DEBUG) -o $@ -c $< $(CFLAGS) $(LDFLAGS)
#	$(C) $(DEBUG) -o main.o -c main.cpp $(CFLAGS) $(OPTIMIZATION)

clean :
	rm -rf *.o

mrproper: clean
	rm -rf $(AIMS)
	
blank:
	tput clear
