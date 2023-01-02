CFLAGS=-Wall -Wextra  -Werror  -pedantic 
CC=clang $(CFLAGS)

all: encode decode

encode: encode.o trie.o io.o
	$(CC) -o encode encode.o trie.o io.o -lm
decode: decode.o word.o io.o
	$(CC) -o decode decode.o word.o io.o -lm
decode.o: decode.c
	$(CC) -c decode.c
encode.o: encode.c
	$(CC) -c encode.c
trie.o: trie.c
	$(CC) -c trie.c
word.o: word.c
	$(CC) -c word.c
io.o: io.c
	$(CC) -c io.c
clean: 
	rm -f encode decode encode.o decode.o word.o trie.o io.o
infer:
	make  clean; infer-capture  -- make all; infer-analyze
