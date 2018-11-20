PRJ=ifj18
PROGS=$(PRJ)
CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -lm

# For testing on merlin.fit.vutbr.cz
XLOGIN=xmanja00

all: $(PROGS)

$(PRJ): *.c lib/*.c
	$(CC) $(CFLAGS) -o $@ *.c lib/*.c

clean:
	rm -f *.o *.out $(PROGS) lexical-tests *.exe *.tmp *.stackdump

tests: lexical-test

lexical-test: *.c lib/*.c
	$(CC) $(CFLAGS) -o $@ *.c lib/*.c -DDEBUG

test: tests
	test/runtest.sh

fulltest:
	ssh -t $(XLOGIN)@merlin.fit.vutbr.cz 'git clone https://github.com/jakubman1/IFJ18.git; cd IFJ18; make tests;chmod +x test/runtest.sh;test/runtest.sh;cd ..;echo -e "\n\nCleaning up after tests..."; rm -rf IFJ18;echo "Cleanup done."'
