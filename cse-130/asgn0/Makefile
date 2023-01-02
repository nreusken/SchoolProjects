shoulders: shoulders.o
	cc -o shoulders shoulders.o

shoulders.o: shoulders.c
	cc -c shoulders.c

clean:
	rm -f shoulders shoulders.o

valgrind: 
	valgrind --leak-check=full ./shoulders