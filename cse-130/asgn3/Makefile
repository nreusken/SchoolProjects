httpproxy: httpproxy.o DataStructs.o RequestHandler.o
	gcc -o httpproxy httpproxy.o DataStructs.o RequestHandler.o -pthread -Wall -Wextra -Wpedantic -Wshadow

httpproxy.o: httpproxy.c 
	gcc -c httpproxy.c

DataStructs.o: DataStructs.h DataStructs.c 
	gcc -c DataStructs.c

RequestHandler.o: RequestHandler.h RequestHandler.c
	gcc -c RequestHandler.c 

clean:
	rm -f httpproxy httpproxy.o DataStructs.o RequestHandler.o

build:
		rm -f httpproxy httpproxy.o DataStructs.o RequestHandler.o 
		make

valgrind: 
	valgrind --leak-check=full ./httpproxy 8080 8081
