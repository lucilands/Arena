CFLAGS=-Wall -Wextra -pedantic -std=c99 -g

all: demo.c 
	mkdir -p bin
	$(CC) $< $(CFLAGS) -o bin/demo
