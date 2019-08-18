CC = gcc
CFLAGS = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean compile compileAssmble compileEmulate

all: assemble emulate;

assemble: assemble.o

emulate: emulate.o
		$(CC) $(CFLAGS) $^ -o $@

clean:
		rm -f $(wildcard *.o)
		rm -f assemble
		rm -f emulate

compile: compileAssmble compileEmulate

compileAssmble:
		gcc -g 32bitProcess.c -o assemble