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
CFLAGS = -Wall -W -Wextra -pedantic

# use of G99 version
G99 = -std=c99

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

MCDA: $(OBJECTS)/main.o $(OBJECTS)/flowsort.o $(OBJECTS)/stoch_prom.o $(OBJECTS)/promethee.o $(OBJECTS)/expert.o
	$(C) $(G99) $(DEBUG) -o $@ $^ $(LDFLAGS) $(CFLAGS)
#	$(C) $(DEBUG) -o MCDA main.o $(CFLAGS) $(OPTIMIZATION)

$(OBJECTS)/expert.o : $(SOURCES)/expert.c $(HEADERS)/expert.h
	$(C) $(G99) $(DEBUG) -o $@ -c $< $(LDFLAGS) $(CFLAGS)
#	$(C) $(DEBUG) -o expert.o -c expert.c $(CFLAGS) $(OPTIMIZATION)

$(OBJECTS)/promethee.o : $(SOURCES)/promethee.c $(HEADERS)/promethee.h $(HEADERS)/expert.h
	$(C) $(G99) $(DEBUG) -o $@ -c $< $(LDFLAGS) $(CFLAGS)
#	$(C) $(DEBUG) -o expert.o -c expert.c $(CFLAGS) $(OPTIMIZATION)

$(OBJECTS)/stoch_prom.o : $(SOURCES)/stoch_prom.c $(HEADERS)/stoch_prom.h $(HEADERS)/promethee.h
	$(C) $(G99) $(DEBUG) -o $@ -c $< $(LDFLAGS) $(CFLAGS)
#	$(C) $(DEBUG) -o expert.o -c expert.c $(CFLAGS) $(OPTIMIZATION)

$(OBJECTS)/flowsort.o : $(SOURCES)/flowsort.c $(HEADERS)/flowsort.h $(HEADERS)/stoch_prom.h
	$(C) $(G99) $(DEBUG) -o $@ -c $< $(LDFLAGS) $(CFLAGS)
#	$(C) $(DEBUG) -o main.o -c main.c $(CFLAGS) $(OPTIMIZATION)

$(OBJECTS)/main.o : $(SOURCES)/main.c $(HEADERS)/flowsort.h $(HEADERS)/expert.h
	$(C) $(G99) $(DEBUG) -o $@ -c $< $(LDFLAGS) $(CFLAGS)
#	$(C) $(DEBUG) -o main.o -c main.c $(CFLAGS) $(OPTIMIZATION)

clean :
	rm -rf $(OBJECTS)/*.o

mrproper: clean
	rm -rf $(AIM)
	
blank:
	tput clear

