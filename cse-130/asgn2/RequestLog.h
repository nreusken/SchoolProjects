typedef struct waitlist{
  int* queue;
  int start, end, size, log;
  char logfile[100];
  off_t offset;
  sem_t full, empty, lock, logging;
}waitlistObj;
typedef struct waitlist* waitlist;

waitlist createWaitlist(int n, char* fd, int log, int offset);

int count_bytes(char* file);

void log_request(char* host, char* req, char*file, int length, char* buffer, waitlist w);

void log_fail(char* file, char* req, int code, char* http, waitlist share);

int validate_log(char* file);

void healthcheck(char* req, int connfd, char* host, waitlist share);