uint16_t strtouint16(char number[]);

int create_listen_socket(uint16_t port);

int create_client_socket(uint16_t port);

void handle_connection(void* shared_data, int connfd, int serverfd, ServersList* server);

void check_health(ServersList *list);