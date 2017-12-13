CC = gcc
CFLAGS = -Wall -Wextra -O3

all: build

build: hashtable

hashtable: main.c hashtable.o hash.o
	$(CC) $(CFLAGS) $^ -o $@ 

hashtable.o: hashtable.c hashtable.h
	$(CC) $(CFLAGS) hashtable.c -c


hash.o: hash.c hash.h 
	$(CC) $(CFLAGS) hash.c -c

.PHONY:
clean:
	rm -f ~* *.o *.so hashtable