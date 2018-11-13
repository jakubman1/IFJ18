PRJ=ifj18
#
PROGS=$(PRJ)
CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -lm

all: $(PROGS)

$(PRJ): *.c lib/*.c
	$(CC) $(CFLAGS) -o $@ *.c lib/*.c

clean:
	rm -f *.o *.out $(PROGS) debug

debug: *.c lib/*.c
	$(CC) $(CFLAGS) -o $@ *.c lib/*.c -DDEBUG

test:
	make clean; make debug
	./debug < test.rb

fulltest:
	ssh -t xmanja00@merlin.fit.vutbr.cz 'git clone https://github.com/jakubman1/IFJ18.git; cd IFJ18; make debug;chmod +x test/runtest.sh;test/runtest.sh;cd ..; rm -rf IFJ18'
