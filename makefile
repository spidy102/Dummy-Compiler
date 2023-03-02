# The compiler
CC = gcc

# The compiler flags
CFLAGS = -g

all: driver

driver:
	$(CC) $(CFLAGS) -o stage1exe driver.c parser.c lexer.c hashmap.c set.c stack.c tree.c twinbuffer.c

clean:
	rm -f *.o stage1exe
