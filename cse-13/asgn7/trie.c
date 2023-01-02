#include "trie.h"
#include "code.h"
#include <stdio.h>
#include <stdlib.h>

// Constructor for a TrieNode.
//
// index:   Index of the constructed TrieNode.
// returns: Pointer to a TrieNode that has been allocated memory.
//
TrieNode *trie_node_create(uint16_t code) {
  //allocates memory for trie node
  TrieNode *node = (TrieNode *)calloc(1, sizeof(TrieNode));
  //checks if node is created
  if (!node) {
    return NULL;
  }
  //sets node->code to code
  node->code = code;
  //returns the node
  return node;
}

//
// Destructor for a TrieNode.
//
// n:       TrieNode to free allocated memory for.
// returns: Void.
//
void trie_node_delete(TrieNode *n) {
  //frees node n
  free(n);
  return;
}

//
// Initializes a Trie: a root TrieNode with the index EMPTY_CODE.
//
// returns: Pointer to the root of a Trie.
//
TrieNode *trie_create(void) {
  //creats a trie, the root with the code EMPTY_CODE
  return trie_node_create(EMPTY_CODE);
}

//
// Resets a Trie to just the root TrieNode.
//
// root:    Root of the Trie to reset.
// returns: Void.
//
void trie_reset(TrieNode *root) {
  //iterates through the nodes that are connected to the root
  for (int i = 0; i < 256; i++) {
    //deletes the tries connected to the root
    trie_delete(root->children[i]);
  }
  return;
}

//
// Deletes a sub-Trie starting from the sub-Trie's root.
//
// n:       Root of the sub-Trie to delete.
// returns: Void.
//
void trie_delete(TrieNode *n) {
  //checks if n is initialized else return
  if (!n) {
    return;
  }
  //iterates through each trie connected to the node
  for (int i = 0; i < 256; i++) {
    //checks if n->children is not NULL
    if (n->children[i] != NULL) {
      //deletes the trie connected to node n
      trie_delete(n->children[i]);
    }
  }
  //check is n is initialized
  if (n) {
    //deletes node n
    trie_node_delete(n);
  }
  return;
}

//
// Returns a pointer to the child TrieNode reprsenting the symbol sym.
// If the symbol doesn't exist, NULL is returned.
//
// n:       TrieNode to step from.
// sym:     Symbol to check for.
// returns: Pointer to the TrieNode representing the symbol.
//
TrieNode *trie_step(TrieNode *n, uint8_t sym) {
  //returns the pointer to the child TrieNode reprsenting the symbol sym
  return n->children[sym];
}
