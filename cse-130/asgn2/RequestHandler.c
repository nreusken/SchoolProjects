#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <stdint.h>
#include <ctype.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include "RequestLog.h"

#define HTTP "HTTP/1.1"

/**
   Handles HEAD Requests
 */
void head(int connfd, char* host, char* req, char* file, waitlist list){
  //checks if requested file is in the folder 
  if(access(file, F_OK) != 0){
    char* response = (char*)calloc(1000, sizeof(char));
    sprintf(response, "HTTP/1.1 404 File Not Found\r\nContent-Length: 15\r\n\r\nFile Not Found\n");
    log_fail(file, req, 404, HTTP, list);
    send(connfd, response, strlen(response) ,0);
    free(response);
    return;
  }
  //checks if the given file has permissions set to be read
  if(access(file, R_OK) != 0){
    char* response = (char*)calloc(1000, sizeof(char));
    sprintf(response, "HTTP/1.1 403 Forbidden\r\nContent-Length: 10\r\n\r\nForbidden\n");
    log_fail(file, req, 403, HTTP, list);
    send(connfd, response, strlen(response) ,0);
    free(response);
    return;
  }

  //iniciates lengths, which is to caluclate the length of the file
  int length = 0;
  
  //opens file
  int fd = open(file, O_RDONLY);

  //locks file with shared lock so people with a shared lock can access
  flock(fd, LOCK_SH);


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
  
  //if logging is enabled send request to log request
  if(list->log == 1){
    char* temp = 0;
    log_request(host, req, file, length, temp, list);
    
  }

  //closes file and and unlock file
  flock(fd, LOCK_UN);
  close(fd);
 

  //allocates space for response heaader, and sends it to clients
  char* response = (char*)calloc(1000, sizeof(char));  
  sprintf(response, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n", length);
  send(connfd, response, strlen(response) ,0);
  free(response);  
}


/**
   Handles GET Requests
 */
void get(int connfd, char* host, char* req, char* file, waitlist list){
  //checks if requested file is in the folder 
  if(access(file, F_OK) != 0){
    char* response = (char*)calloc(1000, sizeof(char));
    sprintf(response, "HTTP/1.1 404 File Not Found\r\nContent-Length: 15\r\n\r\nFile Not Found\n");
    log_fail(file, req, 404, HTTP, list);
    send(connfd, response, strlen(response) ,0);
    free(response);
    return;
  }
  //checks if the given file has permissions set to be read 
  if(access(file, R_OK) != 0){
    char* response = (char*)calloc(1000, sizeof(char));
    sprintf(response, "HTTP/1.1 403 Forbidden\r\nContent-Length: 10\r\n\r\nForbidden\n");
    log_fail(file, req, 403, HTTP, list);
    send(connfd, response, strlen(response) ,0);
    free(response);
    return;
  }
  //iniciates lengths, which is to caluclate the length of the file
  int length = 0;
  
  //opens file
  int fd = open(file, O_RDONLY);

  //locks file with shared lock so other get and head requests can still access file
  flock(fd, LOCK_SH);
  //iniciates buffer and reads the bytes to the buffer
  int* buffer =(int *)calloc(10000, sizeof(int));
  int bytes = read(fd, buffer, 10000);
  //loops till nob bytes are read
  //counts how many bytes read in total
  while(bytes != 0){
    length += bytes;
    bytes = read(fd, buffer, 10000);
  }
  //close file
  close(fd);
  free(buffer);

  //allocates space for response heaader, and sends it to clients
  char* response = (char*)calloc(1000, sizeof(char));  
  sprintf(response, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n", length);
  send(connfd, response, strlen(response) ,0);
  free(response);

  //opens file again to read and send the bytes
  fd = open(file, O_RDONLY);
  int* buff =(int *)calloc(10000, sizeof(int));
  //reads the first and loops till buffer reaches 0
  //while everytime sending the buffer to client
  int rd = read(fd, buff, 10000);
  while(rd != 0){
    
    send(connfd, buff, 10000, 0);
    //resets buff before reading new bytes from file
    memset(buff, 0, 10000*sizeof(int));
    rd = read(fd, buff, 10000);
    
  }
  if(list->log == 1){
    char* temp = (char*)calloc(1000, sizeof(char));
    pread(fd, temp, 1000, 0);
    log_request(host, req, file, length, temp, list);
    free(temp);
  }

  free(buff);
  //close file and unlock file
  flock(fd, LOCK_UN);
  close(fd);
}


/**
   Handles PUT Requests
 */
void put(int connfd, char* host, char* req, char* file, char* length, char* buff, waitlist list){
  //checks if the given file has permissions set to be read 
  if(access(file, W_OK) != 0 && access(file, F_OK) == 0){
    char* response = (char*)calloc(1000, sizeof(char));
    sprintf(response, "HTTP/1.1 403 Forbidden\r\nContent-Length: 10\r\n\r\nForbidden\n");
    log_fail(file, req, 403, HTTP, list);
    send(connfd, response, strlen(response) ,0);
    free(response);
    return;
  }
  //checks if file is in directory, if so delte the file since when openning a new file will be created allowing for overwriting.
  if(access(file, F_OK) == 0){
    remove(file);
  }
  
  //chagne the length in to a integer assigning it to bytes
  //which is the amount of bytes writing to the new file
  int bytes = atoi(length);
  int len = bytes;
  //opens file, creating a new one allowing for reading and writing premissions
  int des = open(file, O_RDWR | O_CREAT, 0666);
  
  //finds the index at the end of the header in the buffer
  int index = 0;
  for(int i = 0; i < strlen(buff); i++){
    if(buff[i] == '\r' && buff[i-2] == '\r'){
      index = i+2;
    }
  }
  
  //allocates space for a buffer
  //get everything after the header and stores into buffer
  char* buffer = (char *)calloc(10000, sizeof(char));
  for(int i = index; i < strlen(buff); i++){
    buffer[i-index] = buff[i];
  }
  

  //locks file
  flock(des, LOCK_EX);
  //writes to file if buffer isnt empty and deecreases bytes
  if(strlen(buffer) != 0){
    write(des, buffer, strlen(buffer));
    bytes -= strlen(buffer);
  }

  //initiates r value
  
  int r;
  //loops till all bytes are written to the file
  while(bytes > 0){
    //resets buff before writing to it
    memset(buffer, 0, 10000*sizeof(char));
    //read the bytes sent by clienht
    r = recv(connfd, buffer, 10000, 0);
    //write the bytes to the file
    if(r <= 0){
      flock(des, LOCK_UN);
      close(des);
      free(buffer);
      remove(file);
      return; 
    }
    write(des, buffer, r);
    //subtract the written bytes from the total bytes that need to be written
    bytes -= r;
    
  }

  //free bufffer and unlock file
  free(buffer);
  char* temp;
  if(list->log == 1){
    temp = (char*)calloc(1000, sizeof(char));
    pread(des, temp, 1000, 0);
  }

  flock(des, LOCK_UN);

  //close file
  close(des);

  if(list->log == 1){
    log_request(host, req, file, len, temp, list);
    free(temp);
  }
  

  //allocate space and seet header response for client, and send that to the client 
  char* response = (char*)calloc(100, sizeof(char));
  sprintf(response, "HTTP/1.1 201 Created\r\nContent-Length: 8\r\n\r\nCreated\n");
  send(connfd, response, strlen(response) ,0);
  free(response);
}

/**
   Converts a string to an 16 bits unsigned integer.
   Returns 0 if the string is malformed or out of the range.
 */
uint16_t strtouint16(char number[]) {
  char *last;
  long num = strtol(number, &last, 10);
  if (num <= 0 || num > UINT16_MAX || *last != '\0') {
    return 0;
  }
  return num;
}

void handle_connection(int connfd, waitlist list) {
  // initialized the variables to read the header incoming from the client
  char* file = (char*)calloc(21, sizeof(char));
  char* http_v = (char*)calloc(8, sizeof(char));
  char* command = (char*)calloc(6, sizeof(char));
  char* host = (char*)calloc(21, sizeof(char));
  char* f = (char*)calloc(20, sizeof(char));
  char* buff = (char*)calloc(10000, sizeof(char));
  while(1){
    //read client heeader
    int r = recv(connfd, buff, 10000, 0);

    //set variables from client header
    sscanf(buff, "%s %s %s \r\nHost: %s\r\n", command, file, http_v, host);
    
    //checks if the file given has allowed characters
    int check_file = 0;
    for(int i = 1; i < strlen(file); i++){
      if(isalnum(file[i]) == 0 && file[i] != '.' && file[i] != '_'){
        check_file++;
      }
      if(check_file){
        break;
      }
    }
    
    
    //checks if there are no spaces in given host
    int check_host = 0;
    for(int i = 0; i < strlen(host); i++){
      if(isspace(host[i])){
        check_host++;
      }
      if(check_host){
        break;
      }
    }

    //creates right HTTP version to check with given version
    char* h_check = (char*)calloc(9, sizeof(char));
    sprintf(h_check, "%s", "HTTP/1.1");
    
    memset(f, 0, 19*sizeof(char));
    for(int i = 0; i < 19; i++){
      f[i] = file[i+1];
    }
    
    //checks if get is used for health check and if logging is enabled
    //if so call healthcheck function else send responses and maybe log
    char* health = "healthcheck";
    char* g = "GET";
    if(strcmp(health, f) == 0){
      if(strcmp(command, g) != 0){
        char* response = (char*)calloc(1000, sizeof(char));
        sprintf(response, "HTTP/1.1 403 Forbidden\r\nContent-Length: 10\r\n\r\nForbidden\n");
        log_fail(file, command, 403, HTTP, list);
        send(connfd, response, strlen(response) ,0);
        free(response);
        break;
      }
      else if(!list->log){
        char* response = (char*)calloc(1000, sizeof(char));
        sprintf(response, "HTTP/1.1 404 File Not Found\r\nContent-Length: 15\r\n\r\nFile Not Found\n");
        send(connfd, response, strlen(response) ,0);
        free(response);
        break;
      }
      else{
        healthcheck(command, connfd, host, list);
        break;
      }
    }

    //checks if given file name is right length and if there are any forbidden characters used
    //check if given host does not have any spaces 
    //checks if given http version is correct
    //if not return header response with code 400 Bad Request
    if(strcmp(h_check, http_v) != 0 || 1 >= strlen(file) || strlen(file) > 20 || file[0]!='/' || check_file || check_host || strlen(host) < 12){
      char response[1000];
      memset(response, 0, 1000*sizeof(char));
      sprintf(response, "HTTP/1.1 400 Bad Request\r\nContent-Length: 12\r\n\r\nBad Request\n");
      log_fail(f, command, 400, http_v, list);
      free(h_check);
      send(connfd, response, strlen(response) ,0);
      break;
    }
    free(h_check);

    //removes / from given file, this is so that when opening file it opens the right file
   
    //set correct strings to compare to the given request type which is assigned to command
    char* p = "PUT";
    char* h = "HEAD";
    //checks if request type is GET and if so call get function to start process for GET request 
    if(strcmp(g, command) == 0){
      get(connfd, host, command, f, list);
    }
    else if(strcmp(h, command) == 0){
      head(connfd, host, command, f, list);
    }
    //checks if request type is PUT and if so call get function to start process for PUT request 
    else if(strcmp(p, command) == 0){
      char length[1000];
      sscanf(buff, "%s %s %s \r\nHost: %s \r\nUser-Agent: curl/7.58.0\r\nAccept: */*\r\nContent-Length: %s", command, file, http_v, host, length);
      for(int i = 0; i < strlen(length); i++){
        if(isdigit(length[i]) == 0){
          char response[1000];
          memset(response, 0, 1000*sizeof(char));
          sprintf(response, "HTTP/1.1 400 Bad Request\r\nContent-Length: 12\r\n\r\nBad Request\n");
          send(connfd, response, strlen(response) ,0);
          close(connfd);
          return;
        }
      }
      put(connfd, host, command, f, length, buff, list);
    }
    //if neither call not implemented response
    else{
      char response[1000];
      memset(response, 0, 1000*sizeof(char));
      sprintf(response, "HTTP/1.1 501 Not Implemented\r\nContent-Length: 16\r\n\r\nNot Implemented\n");
      send(connfd, response, strlen(response) ,0);
    }
    break;
  }
  //free allocated memory
  free(f);
  free(file);
  free(http_v);
  free(command);
  free(host);
  free(buff);
  // when done, close socket
  close(connfd);
}

/**
   Creates a socket for listening for connections.
   Closes the program and prints an error message on error.
 */
int create_listen_socket(uint16_t port) {
  struct sockaddr_in addr;
  int listenfd = socket(AF_INET, SOCK_STREAM, 0);
  if (listenfd < 0) {
    err(EXIT_FAILURE, "socket error");
  }

  memset(&addr, 0, sizeof addr);
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htons(INADDR_ANY);
  addr.sin_port = htons(port);
  if (bind(listenfd, (struct sockaddr*)&addr, sizeof addr) < 0) {
    err(EXIT_FAILURE, "bind error");
  }

  if (listen(listenfd, 500) < 0) {
    err(EXIT_FAILURE, "listen error");
  }

  return listenfd;
}