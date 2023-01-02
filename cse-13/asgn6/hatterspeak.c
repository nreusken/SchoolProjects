#include "bf.h"
#include "hash.h"
#include "hs.h"
#include "parser.h"
#include <ctype.h>
#include <getopt.h>
#include <math.h>
#include <regex.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define WORD "[a-zA-Z0-9]+-[a-zA-Z0-9]+|[a-zA-Z0-9]+'[a-zA-Z0-9]+|[a-zA-Z0-9]+"

//move_to_front deciding whether or not to use move to front implementation
bool move_to_front = false;
//amount of times ll_lookup is called
int seeks = 0;
//amount of times a node is accesed
int links = 0;
//sum of Linked Lists lenghts
int sum_ll = 0;

//linklist struct
struct LinkedList {
  char *word;
  struct LinkedList *next;
};

//looks up if is in the linked list given
int look_up(struct LinkedList **head, char *i) {
  //sets n to head
  struct LinkedList *n = *head;
  //if n does not equal NULL meaning the linkedlist is not empty
  while (n != NULL) {
    //if i equals to n->word return 1 meaning true
    if (strcmp(n->word, i) == 0) {
      return 1;
    }
    //set n to n->next going to the next position
    n = n->next;
  }
  //return 0 if not in linkedlist
  return 0;
}

//adds i to the linked list
void push(struct LinkedList **head, char *i) {
  //checks if i is in the linked list and if so return
  if (look_up(head, i)) {
    return;
  }
  //allocates memory for the new_node in the linked list
  struct LinkedList *node
      = (struct LinkedList *)malloc(sizeof(struct LinkedList));
  //if node is not initialized return
  if (!node) {
    return;
  }
  //set i_len to lenght of i plus to allocate the right amoung of memory of i
  int i_len = strlen(i) + 1;
  //allocate memory for the word i
  node->word = (char *)calloc(i_len, sizeof(char));
  //if node->word is not initialized return
  if (!node->word) {
    return;
  }
  //set n->word to i
  strcpy(node->word, i);
  //set n->next to head
  node->next = *head;
  //set head to node adding it to the front of the list
  *head = node;
  return;
}

//prints linked list given and if there is an translation look it up in the hashtable
void print_linked(struct LinkedList **head, HashTable *hash_table) {
  //sets n to head of the linked list
  struct LinkedList *n = *head;
  //while n is not NULL meaning it is not at the end of the linked list
  while (n != NULL) {
    //lookup the word in the hashtable and set hash_node to the linknode returned from the lookup
    ListNode *hash_node = ht_lookup(hash_table, n->word);
    //if hash_node->gs does not have a hatterspeak translation
    if (hash_node->gs->hatterspeak == NULL) {
      //print the hatterspeak word
      printf("%s\n", n->word);
    }
    //if hash_node->gs has a hatterspeak translation
    else {
      //print the word and its hatterspeak translation
      printf("%s -> %s\n", n->word, hash_node->gs->hatterspeak);
    }
    //goes to the next node in the linkedlist
    n = n->next;
  }
  return;
}

//deletes the linked list
void delete_linked(struct LinkedList **head) {
  //set n to head
  //and creates the linked list next
  struct LinkedList *n = *head;
  struct LinkedList *next;
  //goes through the whole linked list
  while (n != NULL) {
    //sets next to n->next
    next = n->next;
    //frees allocated memory in push
    free(n->word);
    free(n);
    //sets n to n->next
    n = next;
  }
  return;
}

int main(int argc, char **argv) {
  int c = 0;

  //sets bloom filter size to 2^20 which is the default length for the bloom filter.
  uint32_t bloom_size = pow(2, 20);

  //sets hash_size to 10000 which is the default length for hash table size.
  uint32_t hash_size = 10000;

  //sets bool for getopt loop.
  bool stats = false;

  //checks what command line arguments are given.
  while ((c = getopt(argc, argv, "smbh:f:")) != -1) {
    switch (c) {
    //if -h is given, set the x after -h to hash_size.
    case 'h':
      sscanf(optarg, "%d", &hash_size);
      break;
    //if -f is given, set the x after -f to bloom_size.
    case 'f':
      sscanf(optarg, "%d", &bloom_size);
      break;
    //if -s is given set statistics true.
    case 's':
      stats = true;
      break;
    //if -m is gives set move_to_front true.
    case 'm':
      move_to_front = true;
      break;
    //if -b is gives set move_to_front false.
    case 'b':
      move_to_front = false;
      break;
    }
  }

  //creates hash table with size hash_size
  HashTable *hash_table = ht_create(hash_size);

  //create bloom filter with size bloom_size
  BloomFilter *bloom_filter = bf_create(bloom_size);

  //opens oldspeak.txt file and reads the file and sets each word within the file in hash table and bloomfilter.
  char word[4046] = { 0 };
  FILE *old_speak_txt = fopen("oldspeak.txt", "r");
  while (fscanf(old_speak_txt, "%s\n", word) != EOF) {
    //inserts the oldspeak in bloom_filter
    bf_insert(bloom_filter, word);
    //inserts hatterspeak struct with oldpseak and NULL in hash_table
    HatterSpeak *temp = gs_create(word, NULL);
    //inserts hatterspeak struct with oldpseak in hash_table
    ht_insert(hash_table, temp);
  }
  fclose(old_speak_txt);

  //opens hatterspeak.txt file and reads the file and sets each word within the file in hash table and bloomfilter.
  char word1[4046] = { 0 };
  char word2[4046] = { 0 };
  FILE *hatter_speak_txt = fopen("hatterspeak.txt", "r");
  while (fscanf(hatter_speak_txt, "%s %s\n", word1, word2) != EOF) {
    //inserts the oldspeak in bloom_filter
    bf_insert(bloom_filter, word1);
    //inserts hatterspeak struct with oldpseak and hatterspeak in hash_filter
    HatterSpeak *temp = gs_create(word1, word2);
    //inserts hatterspeak struct with oldpseak and hatterspeak in hash_table
    ht_insert(hash_table, temp);
  }
  fclose(hatter_speak_txt);

  c = 0;

  //takes in words inputed by the user.
  FILE *infile = stdin;

  regex_t re;

  if (regcomp(&re, WORD, REG_EXTENDED)) {
    perror("regomp");
    exit(1);
  }

  //creates linked list for non_talk and hatter_speak
  struct LinkedList *non_talk = NULL;
  struct LinkedList *hatter_speak = NULL;
  char *new_word;
  while ((new_word = next_word(infile, &re))) {
    for (int i = 0; i <= strlen(new_word); i += 1) {
      new_word[i] = tolower(new_word[i]);
    }
    //if new_word in bloom fitler
    if (bf_probe(bloom_filter, new_word)) { //if new_word in hash table
      if (ht_lookup(hash_table, new_word)) {
        //if the node returned from ht_lookup has hatterspeak set to NULL
        if (ht_lookup(hash_table, new_word)->gs->hatterspeak == NULL) {
          //add the word to the non_talk linkedlist
          push(&non_talk, new_word);
        }
        //if it does have a hatterspeak translation
        else {
          //add the word to the hatter_speak linked list
          push(&hatter_speak, new_word);
        }
      }
    }
  }
  //close infile
  fclose(infile);
  clear_words();
  regfree(&re);

  //if statistics if requested will only print the statistics
  if (stats) {
    printf("Seeks: %d\n", seeks);
    //calculates avergae links for each seek
    float avg_links = (float)links / seeks;
    printf("Average seek length: %f\n", avg_links);
    //caculates average linked list length
    float avg_ll = (float)sum_ll / hash_size;
    printf("Average Linked List Length: %f\n", avg_ll);
    //calculates the percentage of the hash_table that is used
    float ht_load_ratio = ((float)ht_count(hash_table) / hash_size) * 100;
    printf("Hash table load: %f%%\n", ht_load_ratio);
    //goes through the bloom_filter and checks how many bits are set within the filter
    int bf_counter = 0;
    for (int i = 0; i <= bloom_size; i++) {
      if (bv_get_bit(bloom_filter->filter, i)) {
        bf_counter += 1;
      }
    }
    //calculates the percentage of the bloom_filter that is used
    float bf_load_ratio = ((float)bf_counter / bloom_size) * 100;
    printf("Bloom filter load: %f%%\n", bf_load_ratio);
  } else {
    //if only non_talks word are given
    if (non_talk && hatter_speak == NULL) {
      //print error message
      printf("Dear Wonderlander,\n\n");
      printf("You have chosen to use words that the queen has decreed "
             "oldspeak.\n");
      printf("Due to your infraction you will be sent to the dungeon where you "
             "will be taught hatterspeak.\n\n");
      printf("Your errors:\n\n");
      //prints the nontalk word used by user
      print_linked(&non_talk, hash_table);
    }
    //if there is hatterspeak given by the user
    else if (hatter_speak && non_talk == NULL) {
      //print error message
      printf("Dear Wonderlander,\n\n");
      printf("The decree for hatterspeak finds your message lacking. Some of "
             "thewords that you used are not hatterspeak.\n");
      printf("The list shows how to turn the oldspeak words into "
             "hatterspeak.\n\n");
      //print the hatterspeak words and its hatterspeak translation
      print_linked(&hatter_speak, hash_table);
    }
    //if both hatterspeak words and nontalk word are given by user
    else if (non_talk && hatter_speak) {
      //print error message
      printf("Dear Comrade,\n\n");
      printf("You have chosen to use words that the queen has decreed "
             "oldspeak.\n");
      printf("Due to your infraction you will be sent to the dungeon where you "
             "will be taught hatterspeak.\n\n");
      printf("Your errors:\n\n");
      //print nontalk words give by user
      print_linked(&non_talk, hash_table);
      printf("\nAppropriate hatterspeak translations.\n\n");
      //print hatterspeak given by userand it hatterspeak translation
      print_linked(&hatter_speak, hash_table);
    }
  }

  //frees hatter_speak and non_talk linkedlists
  delete_linked(&hatter_speak);
  delete_linked(&non_talk);

  //frees hastable and bloomfilter
  bf_delete(bloom_filter);
  ht_delete(hash_table);
  return 0;
}
