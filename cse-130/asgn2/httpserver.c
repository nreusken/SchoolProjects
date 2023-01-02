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
#include "RequestHandler.h"
#include "RequestLog.h"


/**
   Adds connfds to the waitlist till it is filled and holds
 */
void dispatcher(waitlist list, int connfd){
  //sets the waitlist struct
  waitlist l = list;
  //checks if queue has space and if so sets the lock
  sem_wait(&l->empty);
  sem_wait(&l->lock);
  //adds the connfd to the queue and increments the queue
  l->queue[l->end] = connfd;
  l->end = (l->end + 1) % l->size;
  //opens the lock and increments space left on the queue
  sem_post(&l->lock);
  sem_post(&l->full);
  return; 
}

/**
   Takes the connfd from the queue and adds it to the 
 */
void* worker_threads(void* list){
  //sets the waitlist struct
  waitlist l = (struct waitlist*) list;
  //creates and infinite loop for the threads to wait till there is a connection to handle
  while(1){
    //checks if there is something in the queue, if so set the lock
    sem_wait(&l->full);
    sem_wait(&l->lock);
    //take the connfd from the queue and increment where the head is at
    int c = l->queue[l->start];
    l->start = (l->start+1) % l->size;
    //opens lock and increments how many empty spaces are there
    sem_post(&l->lock);
    sem_post(&l->empty);
    //handles connection and sends connection fd and the shared data struct
    handle_connection(c, list);
  }
}


int main(int argc, char *argv[]) {
  //creates listening port
  int listenfd;
  uint16_t port;

  //throws an error if no args are given or too many args
  if (argc > 6 || argc == 1) {
    errx(EXIT_FAILURE, "wrong arguments: %s port_num", argv[0]);
  }
  
  //initializes variables for arguments
  int num_threads = 5;
  char log_file[100];
  int log = 0;
  int c = 0;
  //creates temp variables to make sure the args are following spec
  char* temp1 = "-l";
  char* temp2 = "-N";
  //loops through the args given
  while ((c = getopt(argc, argv, "l:N:")) != -1) {
    switch (c) {
    //checks if option -N is given
    case 'N':
      //if the arg is -N ir -l than it will through an error
      if(strcmp(temp1, optarg) == 0 || strcmp(temp2, optarg)==0){
        errx(EXIT_FAILURE, "wrong arguments: %s wrong ", argv[0]);
      }
      //sets number of threads 
      num_threads = strtouint16(optarg);
      break;
    //checks if option -N is given
    case 'l':
      //sets the file for logging
      sscanf(optarg, "%s", log_file);
      //sets log to 1 so logging is enabled
      log = 1;
      //checks if the option given is not an argument if so send an error
      if(strcmp(temp1, log_file) == 0 || strcmp(temp2, log_file)==0){
        errx(EXIT_FAILURE, "wrong arguments: %s wrong ", argv[0]);
      }
      break;
    //if no arg is given exit with error
    case '?':
      errx(EXIT_FAILURE, "wrong arguments: %s wrong ", argv[0]);
    }
  }
  
  //checks if port is given if not, return error.
  if(optind >= argc){
    errx(EXIT_FAILURE, "wrong arguments: %s wrong ", argv[0]);
  }
  else{
    port = strtouint16(argv[optind]);
  }
  
  //checks if logging is enabled and creates file
  //if file is already there validates the logfile.
  int offset = 0;
  if(access(log_file, F_OK) == 0){
    if(validate_log(log_file) == 0){
      errx(EXIT_FAILURE, "Invalid Log File");
      return 0;
    }
    offset = count_bytes(log_file);
  }
  else if(log == 1){
    int fd = open(log_file, O_CREAT, 0666);
    close(fd);
  }

  //creates the shared waitlist data struct, with number of threads log file and the end of file
  waitlist connfd_list = createWaitlist(num_threads, log_file, log, offset);
  //creates list of threads
  pthread_t workers[num_threads];
  //creats each thread and send it to the worker thread function
  for(int i = 0; i < num_threads; i++){
    pthread_create(&workers[i], NULL, &worker_threads, connfd_list);
  }
  
  //checks if port is valid
  if (port == 0) {
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
    dispatcher(connfd_list, connfd);
    
  }
  return EXIT_SUCCESS;
}
