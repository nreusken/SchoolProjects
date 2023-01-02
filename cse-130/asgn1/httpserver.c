#include <err.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <ctype.h>


/**
   Handles HEAD Requests
 */
void head(int connfd, char* file){
  //checks if requested file is in the folder 
  if(access(file, F_OK) != 0){
    char* response = (char*)calloc(1000, sizeof(char));
    sprintf(response, "HTTP/1.1 404 File Not Found\r\nContent-Length: 15\r\n\r\nFile Not Found\n");
    send(connfd, response, strlen(response) ,0);
    free(response);
    return;
  }
  //checks if the given file has permissions set to be read
  if(access(file, R_OK) != 0){
    char* response = (char*)calloc(1000, sizeof(char));
    sprintf(response, "HTTP/1.1 403 Forbidden\r\nContent-Length: 10\r\n\r\nForbidden\n");
    send(connfd, response, strlen(response) ,0);
    free(response);
    return;
  }

  //iniciates lengths, which is to caluclate the length of the file
  int length = 0;
  
  //opens file
  int fd = open(file, O_RDONLY);

  //iniciates buffer and reads the bytes to the buffer
  int buffer[1000];
  int bytes = read(fd, buffer, 1000);
  //loops till nob bytes are read
  //counts how many bytes read in total
  while(bytes != 0){
    length += bytes;
    bytes = read(fd, buffer, 1000);
  }
  //close file
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
void get(int connfd, char* file){
  //checks if requested file is in the folder 
  if(access(file, F_OK) != 0){
    char* response = (char*)calloc(1000, sizeof(char));
    sprintf(response, "HTTP/1.1 404 File Not Found\r\nContent-Length: 15\r\n\r\nFile Not Found\n");
    send(connfd, response, strlen(response) ,0);
    free(response);
    return;
  }
  //checks if the given file has permissions set to be read 
  if(access(file, R_OK) != 0){
    char* response = (char*)calloc(1000, sizeof(char));
    sprintf(response, "HTTP/1.1 403 Forbidden\r\nContent-Length: 10\r\n\r\nForbidden\n");
    send(connfd, response, strlen(response) ,0);
    free(response);
    return;
  }
  //iniciates lengths, which is to caluclate the length of the file
  int length = 0;
  
  //opens file
  int fd = open(file, O_RDONLY);
  
  //iniciates buffer and reads the bytes to the buffer
  int buffer[10000];
  int bytes = read(fd, buffer, 10000);
  //loops till nob bytes are read
  //counts how many bytes read in total
  while(bytes != 0){
    length += bytes;
    bytes = read(fd, buffer, 10000);
  }
  //close file
  close(fd);

  //allocates space for response heaader, and sends it to clients
  char* response = (char*)calloc(1000, sizeof(char));  
  sprintf(response, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n", length);
  send(connfd, response, strlen(response) ,0);
  free(response);

  //opens file again to read and send the bytes
  fd = open(file, O_RDONLY);
  memset(buffer, 0, 10000*sizeof(int));
  //reads the first and loops till buffer reaches 0
  //while everytime sending the buffer to client
  int rd = read(fd, buffer, 10000);
  while(rd != 0){
    send(connfd, buffer, 10000, 0);
    //resets buff before reading new bytes from file
    memset(buffer, 0, 10000*sizeof(int));
    rd = read(fd, buffer, 10000);
  }
  //close file
  close(fd);
}


/**
   Handles PUT Requests
 */
void put(int connfd, char* file, char* length){
  //checks if the given file has permissions set to be read 
  if(access(file, W_OK) != 0 && access(file, F_OK) == 0){
    char* response = (char*)calloc(1000, sizeof(char));
    sprintf(response, "HTTP/1.1 403 Forbidden\r\nContent-Length: 10\r\n\r\nForbidden\n");
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

  //opens file, creating a new one allowing for reading and writing premissions
  int des = open(file, O_RDWR | O_CREAT, 0666);
  
  //allocates space for a buffer
  int* buff = (int *)calloc(10000, sizeof(int));
  //initiates r value
  int r;
  //loops till all bytes are written to the file
  while(bytes > 0){
    //resets buff before writing to it
    memset(buff, 0, 10000*sizeof(char));
    //read the bytes sent by clienht
    r = recv(connfd, buff, 10000, 0);
    //write the bytes to the file
    if(r <= 0){
      close(des);
      free(buff);
      remove(file);
      return; 
    }
    write(des, buff, r);
    //subtract the written bytes from the total bytes that need to be written
    bytes -= r;
    
  }
  //free bufffer
  free(buff);
  //close file
  close(des);

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



void handle_connection(int connfd) {
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
    
    //checks if given file name is right length and if there are any forbidden characters used
    //check if given host does not have any spaces 
    //checks if given http version is correct
    //if not return header response with code 400 Bad Request
    if(strcmp(h_check, http_v) != 0 || 1 >= strlen(file) || strlen(file) > 20 || file[0]!='/' || check_file || check_host || strlen(host) < 12){
      char response[1000];
      memset(response, 0, 1000*sizeof(char));
      sprintf(response, "HTTP/1.1 400 Bad Request\r\nContent-Length: 12\r\n\r\nBad Request\n");
      free(h_check);
      send(connfd, response, strlen(response) ,0);
      break;
    }
    free(h_check);

    //removes / from given file, this is so that when opening file it opens the right file
    memset(f, 0, 19*sizeof(char));
    for(int i = 0; i < 19; i++){
      f[i] = file[i+1];
    }
    
    //set correct strings to compare to the given request type which is assigned to command
    char g[5];
    memset(g , 0, 5*sizeof(char));
    sprintf(g, "%s", "GET");
    char p[5];
    memset(p , 0, 5*sizeof(char));
    sprintf(p, "%s", "PUT");
    char h[5];
    memset(h , 0, 5*sizeof(char));
    sprintf(h, "%s", "HEAD");
    //checks if request type is GET and if so call get function to start process for GET request 
    if(strcmp(g, command) == 0){
      get(connfd, f);
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
      put(connfd, f, length);
    }
    //checks if request type is HEAD and if so call get function to start process for HEAD request 
    else if(strcmp(h, command) == 0){
      head(connfd, f);
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


int main(int argc, char *argv[]) {
  int listenfd;
  uint16_t port;

  if (argc != 2) {
    errx(EXIT_FAILURE, "wrong arguments: %s port_num", argv[0]);
  }
  port = strtouint16(argv[1]);
  if (port == 0) {
    errx(EXIT_FAILURE, "invalid port number: %s", argv[1]);
  }
  listenfd = create_listen_socket(port);

  while(1) {
    int connfd = accept(listenfd, NULL, NULL);
    if (connfd < 0) {
      warn("accept error");
      continue;
    }
    handle_connection(connfd);
  }
  return EXIT_SUCCESS;
}
