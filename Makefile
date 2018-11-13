PRJ=ifj18
#
PROGS=$(PRJ)
CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -lm

all: $(PROGS)

$(PRJ): *.c lib/*.c
	$(CC) $(CFLAGS) -o $@ *.c lib/*.c

clean:
	rm -f *.o *.out $(PROGS)

debug: *.c lib/*.c
	$(CC) $(CFLAGS) -o $@ *.c lib/*.c -DDEBUG

test:
	make clean; make debug
	./debyg < test.rb

fulltest:
	ssh -t xmanja00@merlin.fit.vutbr.cz 'git clone https://github.com/jakubman1/IFJ18.git; cd IFJ18; make debug;./debug < test/test.rb;cd ..; rm -rf IFJ18'
