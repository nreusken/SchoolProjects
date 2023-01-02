FLAGS=-Wall -Wextra  -Werror  -pedantic -std=c99
CC=clang $(CFLAGS)

all: sorting

sorting: sorting.o bubble.o shell.o quick.o binary.o
	$(CC) -o sorting sorting.o bubble.o shell.o quick.o binary.o
sorting.o: sorting.c
	$(CC) -c sorting.c
bubble.o: bubble.c
	$(CC) -c bubble.c
shell.o: shell.c
	$(CC) -c shell.c
quick.o: quick.c
	$(CC) -c quick.c
binary.o: binary.c
	$(CC) -c binary.c
clean:
	rm -f sorting sorting.o bubble.o shell.o quick.o binary.o
infer:
	make  clean; infer-capture  -- make; infer-analyze
valgrind: 
	make clean; make; valgrind sorting
