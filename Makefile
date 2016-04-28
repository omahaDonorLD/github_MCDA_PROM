################################################################
#                  VARIABLES DEFINITIONS                       #
################################################################

# Compilator
CPP = g++

# Options of compilation
CFLAGS = -Wall -Wextra -pedantic -ansi

# Options of compilation for performances' optimisation
OPTIMIZATION = -O3

# Folder who contained files to include
INCLUDE = -I .

# Finals executables
AIMS = MAIN

################################################################
#                  COMMANDS OF COMPILATION                     #
################################################################

all: $(AIMS)

MAIN: main.o
	$(CPP) -o PROM main.o $(CFLAGS) $(OPTIMIZATION)

main.o: main.cpp
	$(CPP) -o main.o -c main.cpp $(CFLAGS) $(OPTIMIZATION)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(AIMS)
	
blank:
	tput clear
