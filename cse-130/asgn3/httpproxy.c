#include <err.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <netinet/in.h>
#include <getopt.h>
#include <stdbool.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>
#include "DataStructs.h"
#include "RequestHandler.h"




/**
   Adds connfds to the shared data till it is filled and holds
 */
void dispatcher(Shared shared_data, int connfd){
  //sets the shared struct
  Shared s = shared_data;
  //checks if queue has space and if so sets the lock
  sem_wait(&s->empty);
  sem_wait(&s->lock);
  //adds the connfd to the queue and increments the queue
  s->queue[s->end] = connfd;
  s->end = (s->end + 1) % s->size;
  //opens the lock and increments space left on the queue
  sem_post(&s->lock);
  sem_post(&s->full);
  return; 
}

void* healthProber(void* shared_data){
  Shared s = (struct Shared*) shared_data;
  while(1){
   
    sem_wait(&s->check);

    ServersList* servers = s->servers;

    while(servers != NULL){
      check_health(servers);
      

      servers = servers->next;
      
    }
    s->total = 0;
    for(int i = 0; i < s->healthcheck; i++){
      sem_post(&s->healthlock);
    }
    
    
  }
  
}

/**
   Takes the connfd from the queue and adds it to the 
 */
void* worker_threads(void* shared_data){
  //sets the shared struct
  Shared s = (struct Shared*) shared_data;
  //creates and infinite loop for the threads to wait till there is a connection to handle
  while(1){
    //checks if there is something in the queue, if so set the lock
    
    
    sem_wait(&s->full);
    sem_wait(&s->healthlock);
    sem_wait(&s->lock);
    //take the connfd from the queue and increment where the head is at
    int c = s->queue[s->start];
    s->start = (s->start+1) % s->size;
    //opens lock and increments how many empty spaces are there
    int port = best_server(s->servers);
    if(port == 0){
      close(c);
      continue;
    }
    //loops through servers to find available
    int serverfd = create_client_socket(port);
    ServersList* server = findPort(s->servers, port);
    while(serverfd == -1){
      server->disable = 1;
      port = best_server(s->servers);
      if(port == 0){
        close(c);
        sem_post(&s->lock);
        sem_post(&s->empty);
        continue;
      }
      server = findPort(s->servers, port);
      serverfd = create_client_socket(port);

    }
    //incremet requests and increment total requests made
    server->requests++;
    s->total++;
    sem_post(&s->lock);
    sem_post(&s->empty);

    //if server NULL there are no servers available
    if(!server){
      char response[1000];
      memset(response, 0, 1000*sizeof(char));
      sprintf(response, "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 22\r\n\r\nInternal Server Error\n");
      send(c, response, strlen(response) ,0);
    }
    else{
      //handles connection and sends connection fd and the shared data struct
      handle_connection(shared_data, c, serverfd, server);
    }
    //close serverfd and connectionfd
    close(c);
    close(serverfd);
    //increment
    if(s->total == s->healthcheck){
      sem_post(&s->check);
      
    }
  }
}


int main(int argc, char *argv[]) {
  //creates listening port
  int listenfd;
  uint16_t port;

  //initializes variables for arguments
  int num_threads = 5;
  int healthchecks = 5;
  int cache_length = 3;
  int max_size = 1024;
  int c = 0;
  //loops through the args given
  while ((c = getopt(argc, argv, "R:N:s:m:")) != -1) {
    switch (c) {
    //checks if option -N is given
    case 'N':
      //sets number of threads 
      num_threads = atoi(optarg);
      break;
    //checks if option -R is given
    case 'R':
      //sets the file for logging
      healthchecks = atoi(optarg);
      break;
    case 's':
      //sets number of threads 
      cache_length = atoi(optarg);
      break;
    //checks if option -R is given
    case 'm':
      //sets the file for logging
      max_size = atoi(optarg);
      break;
    //if no arg is given exit with error
    }
  }
  
  ServersList* list = NULL;
  //checks if port is given if not, return error.
  if(optind >= argc){
    errx(EXIT_FAILURE, "No port number given");
  }
  else{
    //loops through all inputs that were optional and add them to servers list
    while(optind < argc){
      if(!port){
        port = strtouint16(argv[optind++]);
      }
      else{
        insertServersList(&list, strtouint16(argv[optind++]));
        check_health(list);
      }
    }
      
  }
  
  //creates the shared waitlist data struct, with number of threads log file and the end of file
  Shared shared_data = createSharedData(num_threads, list, max_size, cache_length, healthchecks);
  if(!shared_data){
    return -1;
  }
  //creates list of threads
  pthread_t workers[num_threads];
  //creats each thread and send it to the worker thread function
  for(int i = 0; i < num_threads; i++){
    pthread_create(&workers[i], NULL, &worker_threads, shared_data);
  }
  
  pthread_t healthchecker;
  pthread_create(&healthchecker, NULL, &healthProber, shared_data);

  //checks if port is valid
  if (port == 0) {
    free(shared_data->queue);
    free(shared_data);
    errx(EXIT_FAILURE, "invalid port number: %s", argv[1]);
  }
  listenfd = create_listen_socket(port);

  //loops continously till gets a connection from clietn and sends it to the worker threads
  while(1) {
    int connfd = accept(listenfd, NULL, NULL);
    if (connfd < 0) {
      warn("accept error");
      continue;
    }
    dispatcher(shared_data, connfd);
    
  }
  
  return EXIT_SUCCESS;
}
