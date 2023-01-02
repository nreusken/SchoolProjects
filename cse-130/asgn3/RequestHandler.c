#define _XOPEN_SOURCE 700
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <netinet/in.h>
#include <err.h>
#include <ctype.h>
#include <time.h>

#include "DataStructs.h"

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

/**
   Creates a socket for connecting to a server running on the same
   computer, listening on the specified port number.  Returns the
   socket file descriptor on succes.  On failure, returns -1 and sets
   errno appropriately.
 */
int create_client_socket(uint16_t port) {
  int clientfd = socket(AF_INET, SOCK_STREAM, 0);
  if (clientfd < 0) {
    return -1;
  }

  struct sockaddr_in addr;
  memset(&addr, 0, sizeof addr);
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(port);
  if (connect(clientfd, (struct sockaddr*) &addr, sizeof addr)) {
    return -1;
  }
  return clientfd;
}


//checks request given
int checkRequest(char* req, char* file, char* http_v, char* host){
  //if not get return 501
  char* req_implemented = "GET";
  if(strcmp(req_implemented, req)!=0){
    return 501;
  }

  //if not any of allowed character in file return 400
  for(int i = 1; i < strlen(file); i++){
    if(isalnum(file[i]) == 0 && file[i] != '.' && file[i] != '_'){
        return 400;
      }
  }

  //if string length is longer the 19 return 400
  if(strlen(file) > 20){
    return 400;
  }

  //if not right http version return 400
  char* current_http_v = "HTTP/1.1";
  if(strcmp(current_http_v, http_v)!=0){
    return 400;
  }

  //if host given is not correct return 400
  if(strlen(host)==0){
    return 400;
  }
  return 0;
}

//check last modified dates if most current version in cached data return 1
int checkModified(struct tm* cached_date, struct tm* server_date){
  if(cached_date->tm_sec < server_date->tm_sec){
    return 0;
  }
  else if(cached_date->tm_min < server_date->tm_min){
    return 0;
  }
  else if(cached_date->tm_hour < server_date->tm_hour){
    return 0;
  }
  else if(cached_date->tm_mday < server_date->tm_mday){
    return 0;
  }
  else if(cached_date->tm_mon < server_date->tm_mon){
    return 0;
  }
  else if(cached_date->tm_year < server_date->tm_year){
    return 0;
  }
  return 1;
}

//handles connection accepted by proxy
void handle_connection(Shared shared_data, int connfd, int serverfd, ServersList* server){
  //recieves request made by client
  char* buff = (char*)calloc(10000, sizeof(char));
  memset(buff, 0, 10000*sizeof(char));
  int r = recv(connfd, buff, 10000, 0);
  
  //initializes string variables for header
  char* req_type = NULL;
  char* file = NULL;
  char* host = NULL;
  char* http_v = NULL;

  //parses string to get all header parts
  char* copy = (char*)calloc(10000, sizeof(char));
  sprintf(copy, "%s", buff);
  char *temp1 = NULL;
  char* header_line = strtok_r(copy, "\r\n", &temp1);
  while(header_line != NULL){
    char* temp2 = NULL;
    char* word = strtok_r(header_line, " ", &temp2);
    while(word != NULL){
      if(!req_type){
        req_type = word;
      }
      else if(!file){
        file = word;
      }
      else if(!http_v){
        http_v = word;
      }
      char* temp = "Host:";
      if(strcmp(word, temp) == 0){
        host = temp2;
        word = strtok_r(NULL, " ", &temp2);
        if(strlen(temp2) != 0){
          host = "";
        }
      }
      word = strtok_r(NULL, " ", &temp2);
    }
    header_line = strtok_r(NULL, "\r\n", &temp1);
  }
  
  //checks parsed request
  int req_code = checkRequest(req_type, file, http_v, host);
  //returns error is any found
  if(req_code == 400){
    memset(buff, 0, 10000*sizeof(char));
    sprintf(buff, "HTTP/1.1 400 Bad Request\r\nContent-Length: 12\r\n\r\nBad Request\n");
    send(connfd, buff, strlen(buff), 0);
    free(buff);
    free(copy);
    return;
  }
  else if(req_code == 501){
    memset(buff, 0, 10000*sizeof(char));
    sprintf(buff, "HTTP/1.1 501 Not Implemented\r\nContent-Length: 16\r\n\r\nNot Implemented\n");
    send(connfd, buff, strlen(buff) ,0);
    free(buff);
    free(copy);
    return;
  }
  //allocates space for file name
  char* f = (char*)calloc(20, sizeof(char));
  sprintf(f, "%s", file);
  
  //checks if file is in cache
  Cache* ptr = findFileInCache(shared_data, f);

  //file is not in chache
  if(ptr==NULL){
    //sends request to server and recieves data from server
    send(serverfd, buff, r, 0);
    memset(buff, 0, 10000*sizeof(char));
    r = recv(serverfd, buff, 10000, 0);
    send(connfd, buff, r, 0);
    //initializes request type integer and parse header to make sure that 200 response
    int request_type = 0;
    sscanf(buff,"HTTP/1.1 %d", &request_type);
    
    //if request type is 200 
    if(request_type == 200){
      //parse response header to get file length and Last modified date
      memset(copy, 0, 10000*sizeof(char));
      sprintf(copy, "%s", buff);
      char *temp = NULL;
      char* header_line = strtok_r(copy, "\r\n", &temp);
      header_line = strtok_r(NULL, "\r\n", &temp);
      int length = 0;
      sscanf(header_line, "Content-Length: %d\r\n", &length);
      header_line = strtok_r(NULL, "\r\n", &temp);
      char* date;
      char* last = strtok_r(header_line, " ", &date);
      

      //finds the index at the end of the header in the buffer
      int index = 0;
      for(int i = 0; i < r; i++){
        if(buff[i] == '\r' && buff[i-2] == '\r'){
          index = i+2;
          break;
        }
      }
      //if file is smaller than set cache size cache file
      if(length <= shared_data->cache_size){
        //using tm data struct and strptime get date times
        struct tm time;
        strptime(date,"%a, %d %B %Y %H:%M:%S %Z", &time);
        //finds empty cache
        ptr = findEmptyCache(shared_data);
        //call insert file into cache to insert the length file name and time 
        insertFileToCache(ptr, f, time, length);
        //copy the data of file given in the first recv in to the cache
        int i = 0;
        while(i+index < 10000 && i<ptr->file_size){
          ptr->data[i] = buff[i+index];
          i++;
        }
        //calculates if all bytes from the file have been recieved 
        int bytes = length - (r + index);
        //loop if there are remaining bytes to be recieved 
        while(bytes>0){
          memset(buff, 0, 10000*sizeof(char));
          r = recv(serverfd, buff, 10000, 0);
          send(connfd, buff, r, 0);
          bytes -= r;
          //copy that data into the cache
          for(int j = 0; i < length && j < 10000; j++){
            ptr->data[i] = buff[j];
            i++;
          }
        }
      }
      //if not in cache recv more data from server and don't cache data
      else{
        
        int bytes = length - (r + index);
        while(bytes>0){
          memset(buff, 0, 10000*sizeof(char));
          r = recv(serverfd, buff, 10000, 0);
          send(connfd, buff, r, 0);
          bytes -= r;
        }
      }
      
    }
    //if not 200 ok repsonse given increment server request counter
    else{

      server->fails++;
    }

  }
  //file is in chache
  else{
    //create header request to get last modifed from file on server and recv it in req buffer
    char* req = (char*)calloc(10000, sizeof(char));
    sprintf(req, "HEAD %s HTTP/1.1\r\nHost: localhost:8080\r\n\r\n", ptr->fileName);
    send(serverfd, req, strlen(req), 0);
    memset(req, 0, 10000*sizeof(char));
    recv(serverfd, req, 10000*sizeof(char), 0);
    //parse recieved header file to find the last modifed
    char *temp = NULL;
    char* header_line = strtok_r(req, "\r\n", &temp);
    header_line = strtok_r(NULL, "\r\n", &temp);
    header_line = strtok_r(NULL, "\r\n", &temp);
    char* date;
    char* last = strtok_r(header_line, " ", &date);
    struct tm time;
    strptime(date,"%a, %d %B %Y %H:%M:%S %Z", &time);
    //check modified file if file in cache is most up to date send file from cache
    if(checkModified(ptr->date, &time)){
      memset(req, 0, 10000*sizeof(char));
      sprintf(req, "HTTP/1.1 200 Ok\r\nContent-Length: %d\r\n\r\n", ptr->file_size);
      send(connfd, req, strlen(req),0);
      send(connfd, ptr->data, ptr->file_size, 0);   
    }
    //else send get request to server and handle connection regularly
    else{
      //sends request to server and recieves data from server
      send(serverfd, buff, r, 0);
      memset(buff, 0, 10000*sizeof(char));
      r = recv(serverfd, buff, 10000, 0);
      send(connfd, buff, r, 0);
      //initializes request type integer and parse header to make sure that 200 response
      int request_type = 0;
      sscanf(buff,"HTTP/1.1 %d", &request_type);
      
      //if request type is 200 
      if(request_type == 200){
        //parse response header to get file length and Last modified date
        memset(copy, 0, 10000*sizeof(char));
        sprintf(copy, "%s", buff);
        char *temp = NULL;
        char* header_line = strtok_r(copy, "\r\n", &temp);
        header_line = strtok_r(NULL, "\r\n", &temp);
        int length = 0;
        sscanf(header_line, "Content-Length: %d\r\n", &length);
        header_line = strtok_r(NULL, "\r\n", &temp);
        char* date;
        char* last = strtok_r(header_line, " ", &date);
        

        //finds the index at the end of the header in the buffer
        int index = 0;
        for(int i = 0; i < r; i++){
          if(buff[i] == '\r' && buff[i-2] == '\r'){
            index = i+2;
            break;
          }
        }
        //if file is smaller than set cache size cache file
        if(length <= shared_data->cache_size){
          //using tm data struct and strptime get date times
          struct tm time;
          strptime(date,"%a, %d %B %Y %H:%M:%S %Z", &time);
          //finds empty cache
          ptr = findEmptyCache(shared_data);
          //call insert file into cache to insert the length file name and time 
          insertFileToCache(ptr, f, time, length);
          //copy the data of file given in the first recv in to the cache
          int i = 0;
          while(i+index < 10000 && i<ptr->file_size){
            ptr->data[i] = buff[i+index];
            i++;
          }
          //calculates if all bytes from the file have been recieved 
          int bytes = length - (r + index);
          //loop if there are remaining bytes to be recieved 
          while(bytes>0){
            memset(buff, 0, 10000*sizeof(char));
            r = recv(serverfd, buff, 10000, 0);
            send(connfd, buff, r, 0);
            bytes -= r;
            //copy that data into the cache
            for(int j = 0; i < length && j < 10000; j++){
              ptr->data[i] = buff[j];
              i++;
            }
          }
        }
        //if not in cache recv more data from server and don't cache data
        else{
          
          int bytes = length - (r + index);
          while(bytes>0){
            memset(buff, 0, 10000*sizeof(char));
            r = recv(serverfd, buff, 10000, 0);
            send(connfd, buff, r, 0);
            bytes -= r;
          }
        }
        
      }
      //if not 200 ok repsonse given increment server request counter
      else{

        server->fails++;
      }
    }
    //free request
    free(req);
  }
  //free allocated memory
  free(f);
  free(buff);
  free(copy);  
  return;
}

//checks heath at given server
void check_health(ServersList* list){
  //open connection if not available set disable and return
  int serverfd = create_client_socket(list->port);
  if(serverfd == -1){
    (list)->disable = 1;
    close(serverfd);
    return;
  }
  //else set disable to 0
  else{
    (list)->disable = 0;
  }
  //create get request and store recieved data in buff
  char response[1000];
  char buff[1000];
  memset(response, 0, 1000*sizeof(char));
  memset(buff, 0, 1000*sizeof(char));
  sprintf(response, "GET /healthcheck HTTP/1.1\r\nHost: localhost:%d\r\n\r\n", list->port);
  send(serverfd, response, 1000 ,0);
  recv(serverfd, buff, 1000, 0);
  int request_type = 0;
  //checks if 200 response is sent else disable server
  sscanf(buff,"HTTP/1.1 %d", &request_type);
  if(request_type != 200){
    (list)->disable = 1;
    close(serverfd);
    return;
  }
  //parse through header and get fail and requests from response
  int reqs = -1;
  int i = 0;
  char *temp = NULL;
  char *parse;
  
  parse = strtok_r(buff, "\r\n", &temp);
  while(parse != NULL){
    i = i + sscanf(parse, "%d", &reqs);
    if(i == 1){
      (list)->fails = reqs;
    }
    else if(i == 2){
      (list)->requests = reqs+1;
      break;
    }
    parse = strtok_r(NULL, "\r\n", &temp);
  }
  //close connections
  close(serverfd);
  return;
}