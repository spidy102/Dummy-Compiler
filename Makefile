# The compiler
CC = gcc

# The compiler flags
CFLAGS = -g -Wall 

all: driver

driver:
	$(CC) $(CFLAGS) -o stage1exe driver.c 

parser:
	$(CC) $(CFLAGS) -o parser.o parser.c lexer.c data_structures/hashmap.c data_structures/set.c data_structures/stack.c data_structures/tree.c data_structures/twinbuffer.c

clean:
	rm -f *.o stage1exe
