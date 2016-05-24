################################################################
#                  VARIABLES DEFINITIONS                       #
################################################################

# Compilator
CPP = g++
C = gcc

# Directories
SOURCES = srcs
HEADERS = heads
OBJECTS = objs

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
AIM = MCDA

################################################################
#                  COMMANDS OF COMPILATION                     #
################################################################

all: $(AIM)

MCDA: $(OBJECTS)/main.o $(OBJECTS)/expert.o
	$(C) $(C99) $(DEBUG) -o $@ $^ $(LDFLAGS) $(CFLAGS)
#	$(C) $(DEBUG) -o MCDA main.o $(CFLAGS) $(OPTIMIZATION)

$(OBJECTS)/expert.o : $(SOURCES)/expert.c $(HEADERS)/expert.h
	$(C) $(C99) $(DEBUG) -o $@ -c $< $(LDFLAGS) $(CFLAGS)
#	$(C) $(DEBUG) -o expert.o -c expert.c $(CFLAGS) $(OPTIMIZATION)

$(OBJECTS)/main.o : $(SOURCES)/main.c $(HEADERS)/expert.h $(HEADERS)/main.h
	$(C) $(C99) $(DEBUG) -o $@ -c $< $(LDFLAGS) $(CFLAGS)
#	$(C) $(DEBUG) -o main.o -c main.c $(CFLAGS) $(OPTIMIZATION)

clean :
	rm -rf $(OBJECTS)/*.o

mrproper: clean
	rm -rf $(AIM)
	
blank:
	tput clear
