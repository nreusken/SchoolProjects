FLAGS=-Wall -Wextra  -Werror  -pedantic -std=c99
CC=clang $(CFLAGS)

all: hatterspeak 

hatterspeak: hatterspeak.o hash.o bf.o bv.o speck.o ll.o parser.o hs.o
	$(CC) -o hatterspeak hatterspeak.o hash.o ll.o bf.o bv.o speck.o parser.o hs.o -lm
hatterspeak.o: hatterspeak.c
	$(CC) -c hatterspeak.c
hash.o: hash.c
	$(CC) -c hash.c
ll.o: ll.c
	$(CC) -c ll.c
bf.o: bf.c
	$(CC) -c bf.c
bv.o: bv.c
	$(CC) -c bv.c
speck.o: speck.c
	$(CC) -c speck.c
parser.o: parser.c
	$(CC) -c parser.c
hs.o: hs.c
	$(CC) -c hs.c
clean:
	rm -f hatterspeak hatterspeak.o hash.o bf.o bv.o ll.o speck.o parser.o hs.o
infer:
	make  clean; infer-capture  -- make; infer-analyze
valgrind:
	make clean; make; valgrind hatterspeak
