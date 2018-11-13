PRJ=ifj18
#
PROGS=$(PRJ)
CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -lm

all: $(PROGS)

$(PRJ): *.c
	$(CC) $(CFLAGS) -o $@ *.c lib/*.c

clean:
	rm -f *.o *.out $(PROGS)

test:
	make clean; make
	./ifj18 < test.rb
