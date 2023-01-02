#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <semaphore.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include "RequestLog.h"

/**
 * @brief Create a Waitlist object
 * 
 * @param n 
 * @param logfile 
 * @param log 
 * @param offset 
 * @return waitlist 
 */
waitlist createWaitlist(int n, char* logfile, int log, int offset){
  //initializes the struct and allocates memory for it
  waitlist w = (waitlist)calloc(1, sizeof(waitlistObj));
  //initialize struct objects
  w->queue = (int *)calloc(n, sizeof(int));
  w->start = w->end = 0;
  w->size = n;
  w->offset = offset;
  w->log = log;
  sprintf(w->logfile, "%s", logfile);
  sem_init(&w->full, 0, 0);
  sem_init(&w->empty, 0, n);
  sem_init(&w->lock, 0, 1);
  sem_init(&w->logging, 0, 1);
  return w;
}

/**
 * @brief counts the amount of bytes in a file.
 * 
 * @param file 
 * @return int 
 */
int count_bytes(char* file){
  //iniciates lengths, which is to caluclate the length of the file
  int length = 0;
  
  //opens file
  int fd = open(file, O_RDONLY);

  //iniciates buffer and reads the bytes to the buffer
  int* buffer =(int *)calloc(10000, sizeof(int));
  int bytes = read(fd, buffer, 1000);
  //loops till nob bytes are read
  //counts how many bytes read in total
  while(bytes != 0){
    length += bytes;
    bytes = read(fd, buffer, 1000);
  }
  free(buffer);
  //close file
  close(fd);
  return length;
}

/**
 * @brief Log requests file in given in the shared data struct
 * 
 * @param host 
 * @param req 
 * @param file 
 * @param length 
 * @param buffer 
 * @param share 
 */
void log_request(char* host, char* req, char*file, int length, char* buffer, waitlist share){
  //sets struct
  waitlist w = share;
  //creates temporary array to hold the hex conversion of the body
  char hex[2000];
  //allocates space for log
  char* log = (char*)calloc(3000, sizeof(char));
  //if buffer isnt zero than it is a put or get request
  if(buffer != 0){
    //loops through the buffer
    for(int i = 0; i < strlen(buffer); i++){
      //creates temp array to hold each hex value for each byte
      char temp[3];
      //converts one byte to hex and sets it to the tmep array
      snprintf(temp, 3, "%02hhx", buffer[i]);
      //adds the two digit hex to the total hex conversion
      int x = 0;
      //it makes sure that the offset is right within the actual array
      for(int j = i*2; j < (i*2)+3; j++){
        hex[j] = temp[x]; 
        x++;
      }
    }
    //creates the log using sprintf adding the right attributes
    sprintf(log, "%s\t/%s\t%s\t%d\t%s\n", req, file, host, length, hex);
  }
  //if buffer is empty it is a head request, it will create a log head request
  else{
    sprintf(log, "%s\t/%s\t%s\t%d\n", req, file, host, length);
  }
  //opnes file and sets lock to writes to file using pwrite
  int fd = open(w->logfile, O_RDWR);
  sem_wait(&share->logging);
  pwrite(fd, log, strlen(log), w->offset);
  w->offset += strlen(log);
  //releases log, closes file and frees log
  sem_post(&share->logging);
  close(fd);
  free(log);
  return;
}

/**
 * @brief Logs failed rquest or requests that weren't able to be processed by the server 
 * 
 * @param file 
 * @param req 
 * @param code 
 * @param http 
 * @param share 
 */
void log_fail(char* file, char* req, int code, char* http, waitlist share){
  //initializes shared data struct
  waitlist w = share;
  //allocates space for log and sets log using sprintf
  char* log = (char*)calloc(3000, sizeof(char));
  sprintf(log, "FAIL\t%s /%s %s\t%d\n", req, file, http, code);
  //opens file and locks so that it can write to the log file 
  int fd = open(w->logfile, O_RDWR);
  sem_wait(&share->logging);
  pwrite(fd, log, strlen(log), w->offset);
  w->offset += strlen(log);
  //unlocks lock, closes file and frees log
  sem_post(&share->logging);
  close(fd);
  free(log);
  return;
}

/**
 * @brief Validates the log file
 * 
 * @param file 
 * @return int 
 */
int validate_log(char* file){
  //opens log file and allocates space for buffer
  int fd = open(file, O_RDONLY);
  char* buffer = (char*)calloc(1000, sizeof(char));
  //reads to the buffer
  int bytes =  read(fd, buffer, 1000);
  //if bytes is equal to zero that means the file is empty and return 1
  if(bytes == 0){
    free(buffer);
    close(fd);
    return 1;
  }

  //initialize tabs and new_line variables to count new lines and tabs in a file
  int tabs = 0;
  int new_line = 0;
  
  //loops till bytes equals zero
  while(bytes != 0){
    //loops through buffer which was read from file
    for(int i = 0; i < bytes; i++){
      //if a tab is detected increase tab counter
      if(buffer[i] == '\t'){
        tabs += 1;
      }
      //if a new line was detected meaning it was at the end of a log entry
      else if(buffer[i] == '\n'){
        //if the entry had less than 2 tabs or more than 4 tabs the entry does not follow guidelines
        if(2 > tabs || tabs > 4){
          //if not valid entry free buffer, close file and return 0
          free(buffer);
          close(fd);
          return 0;
        }
        //if new line detected increment new_line and reset tabs for a new entry
        new_line++;
        tabs = 0;
      }
    }
    //clear buffer and read new bytes from file to buffer
    memset(buffer, 0, 1000);
    bytes = read(fd, buffer, 1000);
  }

  //if tabs is not zero return 0 it means that the entry was not complete
  //if new_line equals zero it means that there is no newlines eventhough there are bytes in the file
  if(tabs != 0 || new_line == 0){
    free(buffer);
    close(fd);
    return 0;
  }
  //free buffer and close
  close(fd);
  free(buffer);
  //reutrns 1 because the log file passed all the test to validate the file
  return 1;
}

/**
 * @brief Does a health check on the server given
 * 
 * @param req 
 * @param connfd 
 * @param host 
 * @param share 
 */
void healthcheck(char* req, int connfd, char* host, waitlist share){
  waitlist w = share;
  //checks if log file is valid before doing a health chech
  //if so send a response to the client and log it
  if(validate_log(share->logfile)==0){
    char response[1000];
    memset(response, 0, 1000*sizeof(char));
    sprintf(response, "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 22\r\n\r\nInternal Server Error\n");
    char* http_v = "HTTP/1.1";
    log_fail(share->logfile, req, 500, http_v, share);
    send(connfd, response, strlen(response) ,0);
    return;
  }
  //open file and initialize the buffer
  int fd = open(share->logfile, O_RDONLY);
  char* buffer = (char*)calloc(10000, sizeof(char));
  
  //read bytes and initialize the variables for number of entries, fails and new lines
  int bytes = read(fd, buffer, 10000);
  int entries = 0;
  int fails = 0;
  int new_line = 1;
  //loop till read bytes equal zero
  while(bytes != 0){
    //loops through the buffer
    for(int i = 0; i < strlen(buffer); i++){
      //if buffer is at the end of line increment entries and new line
      if(buffer[i] == '\n'){
        entries += 1;
        new_line++;
      }
      //if start of new line check if it starts with an F meaning a failed log
      else if(new_line == 1){
        //if failed log increment fails
        if(buffer[i] == 'F'){
          fails++;
        }
        //reset new_line after checking the first character of the new line
        new_line = 0;
      }
    }
    //clear buffer and reads new bytes to buffer
    memset(buffer, 0, 10000);
    bytes = read(fd, buffer, 10000);
  }
  //free buffer and close file
  free(buffer);
  close(fd);
  //creates body for the reponse which includes the entries and fails
  char body[100];
  sprintf(body, "%d\n%d\n", fails, entries);
  //creates response using sprintf and sends it to client
  char* response = (char*)calloc(100, sizeof(char));
  sprintf(response, "HTTP/1.1 201 Created\r\nContent-Length: %ld\r\n\r\n%s", strlen(body), body);
  send(connfd, response, strlen(response) ,0);
  free(response);
  //logs request with the body of the healthcheck
  log_request(host, req, share->logfile, strlen(body),  body, share);
  return;
}