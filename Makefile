# KnapSack Makefile
#
# Usage :
#  make		 : Build the program and tests
#  make tests    : Only build the tests
#  make knapsack : Only build the program
#  make clean	 : Remove all object files
#  make clobber	 : Remove all object files and programs (tests and program)
#  make install  : Install knapsack
#

# Configuration
## Compiler
CC=clang++
# CC=/usr/local/Cellar/gcc/4.9.2_1/bin/g++-4.9
ifdef DEBUG
CFLAGS=-g -DDEBUG -Wall
# CFLAGS=-g -DDEBUG -Wall -fsanitize=address
else
CFLAGS=-O3 -msse2 -msse3 -Wall
endif

## Directories
DOBJ=obj
DSRC=src
DTEST=test

#Targets
## Public
all: tests

tests: BigNatural.test

clean:
	rm -f $(DOBJ)/*.o

clobber: clean
	rm -f test/*.test

## Tests
BigNatural.test: $(DSRC)/BigNatural.cpp
	$(CC) $(CFLAGS) -DTEST $^ -o $(DTEST)/$@ $(LDFLAGS)


## Templates

$(DOBJ)/%.o: $(DSRC)/%.cpp
	$(CC) $(CFLAGS) -c $^ -o $@


