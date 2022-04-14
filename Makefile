CC=gcc

all: main.o
	$(CC) -o output main.o -lpthread

main.o: main.c
	$(CC) -c main.c -lpthread

.PHONY: clean all

clean:
	rm -f *.o *.a