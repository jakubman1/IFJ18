PRJ=ifj18
#
PROGS=$(PRJ)
CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -lm

all: $(PROGS)

$(PRJ): *.c
	$(CC) $(CFLAGS) -o $@ *.c

clean:
	rm -f *.o *.out $(PROGS)
