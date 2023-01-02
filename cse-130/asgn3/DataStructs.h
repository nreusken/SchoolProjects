#define _XOPEN_SOURCE 700
#include <time.h>


typedef struct Cache{
  struct tm* date;
  char* fileName;
  char* data;
  int file_size;
  struct Cache *next;
  struct Cache *prev;
}Cache;

typedef struct ServersList{
  int requests, fails, disable;
  u_int16_t port;
  struct ServersList *next;
  struct ServersList *prev;
}ServersList;


typedef struct Shared{
  int* queue;
  int start, end, size, cache_size, cache_length, healthcheck, total;
  ServersList* servers;
  Cache* cached_files;
  sem_t full, empty, lock, healthlock, check;
}SharedObj;
typedef struct Shared* Shared;

Cache* createCacheEntry(int size);

Cache* createCache(int size, int length);

void insertFileToCache(Cache* c, char* file, struct tm date, int size);

void delCache(Cache** head, Cache* ptr);

Cache* findFileInCache(Shared s, char* file);

Cache* findEmptyCache(Shared s);

ServersList* createServersListEntry(int port);

void insertServersList(ServersList **head, int port);

int best_server(ServersList* head);

ServersList *findPort( ServersList *head, int port);

Shared createSharedData(int n, ServersList* list, int cache_size, int cach_length, int healthcheck);