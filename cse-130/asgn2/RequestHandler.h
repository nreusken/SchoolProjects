//-------------------------------
// Structs and function headers for
// http requests
//-------------------------------

void head(int connfd, char* host, char* req, char* file, void* list);

void get(int connfd, char* file, void* list);

void put(int connfd, char* file, char* length, char* buff, void* list);

uint16_t strtouint16(char number[]);

void handle_connection(int connfd, void* list);

int create_listen_socket(uint16_t port);