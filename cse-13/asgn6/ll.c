#include "ll.h"
#include "hs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//amount of times ll_lookup is called
extern int seeks;
//amount of times a node is accesed
extern int links;
//sum of Linked Lists lenghts
extern int sum_ll;
//move_to_front deciding whether or not to use move to front implementation
extern bool move_to_front;
//
//  Constructor  for a ListNode.
//
// gs:   HatterSpeak  struct  containing  oldspeak  and  its  hatterspeaktranslation.
//
ListNode *ll_node_create(HatterSpeak *gs) {
  //allocates memory for the listnode
  ListNode *n = (ListNode *)malloc(sizeof(ListNode));
  if (!n) {
    return 0;
  }
  //sets n->gs to n
  n->gs = gs;
  //sets n->next to NULL
  n->next = NULL;
  //return node
  return n;
}

//
//  Destructor  for a ListNode.
//
// n:   The  ListNode  to free.
//
void ll_node_delete(ListNode *n) {
  //call gs_delete to delete hatterspeak struct
  gs_delete(n->gs);
  //free allocated memory from ll_node_create
  free(n);
  return;
}

//
//  Destructor  for a linked  list of  ListNodes.
//
// head:    The  head of the  linked  list.
//
void ll_delete(ListNode *head) {
  //sets n to head
  ListNode *n = head;
  //goes through every node and clears each node by calling ll_node_delete
  while (n != NULL) {
    //moves pointer to next node
    ListNode *next = n->next;
    //calls ll_node_delete to delete n
    ll_node_delete(n);
    //sets next to n
    n = next;
  }
  return;
}

//
//  Creates  and  inserts a ListNode  into a linked  list.
//
// head:   The  head of the  linked  list to  insert  in.
// gs:     HatterSpeak  struct.
//
// Citation: help from Eugene's Lab section on wednesday
ListNode *ll_insert(ListNode **head, HatterSpeak *gs) {
  //increments sum_ll
  sum_ll += 1;
  //increments seeks
  seeks += 1;
  //if ll_lookup returns null
  if (ll_lookup(head, gs->oldspeak) == NULL) {
    //set n to the node returned from ll_node_create
    ListNode *n = ll_node_create(gs);
    //if n is not initialized return 0
    if (!n) {
      return 0;
    }
    //set n->gs to gs
    n->gs = gs;
    //set n->next to head
    n->next = *head;
    //set head to n
    *head = n;
    //return head
    return n;
  }
  //delet gs structure if head does not equal NULL
  gs_delete(gs);
  //return head
  return *head;
}
//
//  Searches  for a specific  key in a linked  list.
//  Returns  the  ListNode  if found  and  NULL  otherwise.
//
// head:    The  head of the  linked  list to  search  in.
// key:     The key to  search  for.
//
// Citation: help from Eugene's Lab section on wednesday
ListNode *ll_lookup(ListNode **head, char *key) {
  //set n to head
  ListNode *n = *head;
  //set prev to NULL
  ListNode *prev = NULL;
  //loops through the linkedlist
  while (n != NULL) {
    //increment links
    links += 1;
    //sets temp to oldspeak word stored in n->gs hatterspeak struct
    char *temp = n->gs->oldspeak;
    //if temp == key
    if (strcmp(temp, key) == 0) {
      //if move_front impelementation is set to true
      if (move_to_front && (prev != NULL)) {
        //sets prev next to the node after n
        prev->next = n->next;
        //sets n-> to what node is at the head
        n->next = *head;
        //sets n to the head
        *head = n;
        //returns the curren head
        return *head;
      }
      //return the node where the key is equal to the oldspeak word stored in n->gs hatterspeak struct
      return n;
    }
    //set prev to n
    prev = n;
    //set n to n->next
    n = n->next;
  }
  //return NULL if there is no linknode at the the hash index
  return NULL;
}
