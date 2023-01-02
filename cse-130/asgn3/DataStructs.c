#define _XOPEN_SOURCE 700
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "DataStructs.h"

//creates chache entry with given size
Cache* createCacheEntry(int size){
  Cache* c = (Cache*)malloc(sizeof(Cache));
  c->date = (struct tm*)malloc(sizeof(struct tm));
  c->prev = NULL;
  c->next = NULL;
  c->fileName = (char*)calloc(20, sizeof(char));
  c->data = (char*)calloc(size, sizeof(char));
  return c;
}

//add given cache to the front of the linked list
void insertCache(Cache **head, Cache *new){
  //if nothing is in list set next and previous to NULL
  if (*head == NULL){   
    *head = new;
    (*head)->next = NULL;
    (*head)->prev = NULL;
  }
  //else add to the front and change previuos head next to given cache
  else{   
    new->next  = *head;
    (*head)->prev = new;
    new->prev  = NULL;
    *head      = new;
  }
}

//creates a cache of given length 
Cache* createCache(int size, int length){
  Cache* head = NULL;
  //loops till all cache entries are made
  for(int i = 0; i < length; i++){
    Cache* new = createCacheEntry(size);
    insertCache(&head, new);
  }
  return head;
}

//inserts data, file and size into cache
void insertFileToCache(Cache* c, char* file, struct tm date, int size){
  sprintf(c->fileName, "%s",file);
  (c->date)->tm_sec = (&date)->tm_sec;
  (c->date)->tm_min = (&date)->tm_min;
  (c->date)->tm_hour = (&date)->tm_hour;
  (c->date)->tm_mday = (&date)->tm_mday;
  (c->date)->tm_mon = (&date)->tm_mon;
  (c->date)->tm_year = (&date)->tm_year;
  c->file_size = size;
  return;
}

//deletes given cache from the cache
void delCache(Cache **head, Cache *del){
  //if cache is add the head
  if(strcmp((*head)->fileName, del->fileName)==0){

    *head = del->next;
  }
  //if it isnt at the head
  else{
    del->prev->next = del->next;
    //checks if delete next does not equal NULL to set previous
    if (del->next != NULL) del->next->prev = del->prev;
  }

  //frees allocated memory
  free(del->fileName);
  free(del->data);
  free(del);

  return;
}

//find file in cache
Cache* findFileInCache(Shared s, char* file){
  //loops through cache till it finds cache with the same filename
  Cache* ptr = s->cached_files;
  while(ptr != NULL){

    if(strcmp(ptr->fileName, file) == 0){
      return ptr;
    }
    ptr = ptr->next;
  }
  //returns NULL if not found in cache
  return NULL;
}

//finds empty cache if no empty space deletes tail cache of the linked list
Cache* findEmptyCache(Shared s){
  Cache* ptr = s->cached_files;
  Cache* prev;
  //loops to the tail of the list
  while(ptr != NULL){
    prev = ptr;
    ptr = ptr->next;
  }
  //sets tail if no space found the tail can get deleted
  Cache* tail = prev;
  ptr = prev;
  //loops from back to front if there is an empty space return cache
  while(ptr != NULL){
    if(strlen(ptr->fileName) == 0){
      return ptr;
    }
    prev = ptr;
    ptr = ptr->prev;
  }
  //deletes the tail cache if no empty spaces
  delCache(&(s->cached_files), tail);

  //create new cache and insert it at the front of the list of caches
  Cache* new = createCacheEntry(s->cache_size);
  insertCache(&(s->cached_files), new);
  //return new cache
  return new;
}

//creates server list 
ServersList* createServersListEntry(int port){
  ServersList* new =  (ServersList*)malloc(sizeof(ServersList));
	new->next = NULL;
	new->prev = NULL;
	new->port = port;
  new->fails = 0;
  new->requests = 0;
  new->disable = 0;
	return new;
}

//inserts server at the head of the list
void insertServersList(ServersList **head, int port){
  // insert new server as head of list
  ServersList* new =  createServersListEntry(port);
	if (*head == NULL){	
    *head = new;
		(*head)->next = NULL;
		(*head)->prev = NULL;
	}
	else{	
    new->next  = *head;
		(*head)->prev = new;
		new->prev = NULL;
		*head = new;
	}
}

//calculates success rate 
float successRate(int requests, int fails){
  float rate = ((float)requests - (float)fails) / (float)requests; 
  return rate;
}

//finds best server with the least amount of requests or tie breaker on success rate
int best_server(ServersList* head){
  ServersList *ptr;
  ptr = head;
  //holder for best server port number with amount of requests for that server and success rate for that server
  int port = 0;
  int num_of_reqs = -1;
  float success_rate = -1;
  //loops throught list of servers and check server count
  while(ptr != NULL){
    //if requests is less than previous best server set that server to now best server also checks if server isn't disbaled
    if((ptr->requests < num_of_reqs || num_of_reqs == -1) && !ptr->disable){
      port = ptr->port;
      num_of_reqs = ptr->requests;
      success_rate = successRate(ptr->requests, ptr->fails);
    }
    //if it request are equal and isnt disabled do succes rate tie breaker
    else if(ptr->requests == num_of_reqs && !ptr->disable){
      float temp = successRate(ptr->requests, ptr->fails);
      if(temp > success_rate){
        port = ptr->port;
        num_of_reqs = ptr->requests;
        success_rate = temp;
      }
    }
    ptr = ptr->next;
    
  }
  
  return port;
}

//find server with corresponding port number
ServersList *findPort( ServersList *head, int port){
  ServersList *ptr;
  ptr = head;
  //loops through server list till it finds the right server with the same port number else return NULL
  while( ptr != NULL ){
    if (ptr->port == port){
            return ptr;
    }
    else    ptr = ptr->next;
  }

  return( NULL );

}

//creates shared data struct and initializes locks
Shared createSharedData(int n, ServersList* list, int cache_size, int cache_length, int healthcheck){
  //initializes the struct and allocates memory for it
  Shared s = (Shared)calloc(1, sizeof(SharedObj));
  //initialize struct objects
  s->queue = (int *)calloc(n, sizeof(int));
  s->start = s->end = 0;
  s->size = n;
  s->servers = list;
  s->cache_size = cache_size;
  s->cache_length= cache_length;
  s->healthcheck = healthcheck;
  s->total = 0;
  s->cached_files = createCache(s->cache_size, s->cache_length);
  sem_init(&s->full, 0, 0);
  sem_init(&s->empty, 0, n);
  sem_init(&s->lock, 0, 1);
  sem_init(&s->check, 0, 0);
  sem_init(&s->healthlock, 0, healthcheck);
  sem_init(&s->healthlock, 0, healthcheck);
  return s;
}