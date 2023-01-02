#include "hash.h"
#include "ll.h"
#include "speck.h"
#include <stdio.h>
#include <stdlib.h>

//amount of times ll_lookup is called
extern int seeks;

//
//  Constructor  for a HashTable.
//
// length:   Length  of the  HashTable.
// salt:     Salt  for the  HashTable.
//
HashTable *ht_create(uint32_t length) {
  HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
  if (ht) {
    ht->salt[0] = 0x85ae998311115ae3;
    ht->salt[1] = 0xb6fac2ae33a40089;
    ht->length = length;
    ht->heads = (ListNode **)calloc(length, sizeof(ListNode *));
    return ht;
  }

  return (HashTable *)NIL;
}

//
//
//  Destructor  for a HashTable.
//
// ht:   The  HashTable.
//
void ht_delete(HashTable *ht) {
  //call ll_delete for each LinkNode that is stored at each hash
  for (uint32_t i = 0; i < ht->length; i++) {
    ll_delete(ht->heads[i]);
  }
  //frees up the allocated memory during ht_create
  free(ht->heads);
  free(ht);
}

//
//  Returns  number  of  entries  in hash  table
//
// h:   The  HashTable.
//
uint32_t ht_count(HashTable *ht) {
  //counter for amount of Linked lists in the hash tables
  uint32_t counter = 0;
  //iterates through each hashtable index
  for (uint32_t i = 0; i < ht->length; i++) {
    //if doesn't equal NULL there is an entry
    if (ht->heads[i] != NULL) {
      //increment counter
      counter += 1;
    }
  }
  //return amount of entries
  return counter;
}
//
//  Searches a HashTable  for a key.
//  Returns  the  ListNode  if found  and  returns  NULL  otherwise.
//  This  should  call  the  ll_lookup ()  function.
//
// ht:       The  HashTable.
// key:      The key to  search  for.
//
ListNode *ht_lookup(HashTable *ht, char *key) {
  //creates hash to find at what index the word is located
  uint32_t index = (hash(ht->salt, key)) % ht->length;
  //increase seeks
  seeks += 1;
  //return what returns from callin ll_lookup
  return ll_lookup(&ht->heads[index], key);
}

//  First  creates a new  ListNode  from  HatterSpeak.
//  The  created  ListNode  is then  inserted  into a HashTable.
//  This  should  call  the  ll_insert ()  function.
//
// ht:       The  HashTable.
// gs:       The  HatterSpeak  to add to the  HashTable.
//
void ht_insert(HashTable *ht, HatterSpeak *gs) {
  //creates hash to insert at in the right linked_list
  uint32_t index = (hash(ht->salt, gs->oldspeak)) % ht->length;
  //set the head to the head of the linked list that the hatterspeak struct is inserted in.
  ht->heads[index] = ll_insert((&ht->heads[index]), gs);
  return;
}
