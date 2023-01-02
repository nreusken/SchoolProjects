httpserver: httpserver.o
	cc -o httpserver httpserver.o

httpserver.o: httpserver.c
	cc -c httpserver.c

clean:
	rm -f httpserver httpserver.o

valgrind: 
	valgrind --leak-check=full ./httpserver 8080
