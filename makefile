# The compiler
CC = gcc

# The compiler flags
CFLAGS = -g

all: driver

driver:
	$(CC) $(CFLAGS) -o compiler driver.c codegen.c intermedCodeGen.c typeChecker.c symbolTable.c symTableUtil.c ast.c parser.c lexer.c hashmap.c set.c stack.c tree.c twinbuffer.c

clean:
	rm -f *.o compiler
