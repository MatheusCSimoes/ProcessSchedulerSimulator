CC=gcc

CFLAGS=-g -Wall -pedantic -D_GNU_SOURCE=1 -D_REENTRANT
#CFLAGS=-g -Wall -D_REENTRANT
MATH_FLAGS=-lm
SOURCES=main.c iodevice.c queue.c process.c
OBJECTS=main.o iodevice.h queue.o process.h
BINARIES=main

all: $(BINARIES)

main: main.o iodevice.o queue.o process.o
	$(CC) -o simulator main.o iodevice.o queue.o process.o $(MATH_FLAGS)

main.o: main.c
	$(CC) -c main.c $(CFLAGS) $(MATH_FLAGS)

iodevice.o: iodevice.c
	$(CC) -c iodevice.c $(CFLAGS) $(MATH_FLAGS)

queue.o: queue.c
	$(CC) -c queue.c $(CFLAGS) $(MATH_FLAGS)

process.o: process.c
	$(CC) -c process.c $(CFLAGS) $(MATH_FLAGS)

clean:
	rm -rf *.o *.exe *.bak *.c~ $(BINARIES) core simulator

limpa:
	del *.o *.exe *.bak *.c~ $(BINARIES) core
