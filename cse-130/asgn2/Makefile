httpserver: httpserver.o RequestHandler.o RequestLog.o
	gcc -o httpserver httpserver.o RequestHandler.o RequestLog.o -pthread -Wall -Wextra -Wpedantic -Wshadow

httpserver.o: httpserver.c 
	gcc -c httpserver.c

RequestLog.o: RequestLog.h RequestLog.c
	gcc -c RequestLog.c

RequestHandler.o: RequestHandler.h RequestHandler.c RequestLog.o
	gcc -c RequestHandler.c 

clean:
	rm -f httpserver httpserver.o RequestHandler.o RequestLog.o

valgrind: 
	valgrind --leak-check=full ./httpserver 8080
