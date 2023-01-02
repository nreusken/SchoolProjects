CFLAGS=-Wall -Wextra  -Werror  -pedantic
CC=clang $(CFLAGS)

all: tower

tower: tower.o stack.o
	$(CC) -o tower tower.o stack.o
tower.o: tower.c
	$(CC) -c tower.c
stack.o: stack.c
	$(CC) -c stack.c 
clean:
	rm -f tower tower.o stack.o
infer:
	make  clean; infer-capture  -- make; infer-analyze  -- make
