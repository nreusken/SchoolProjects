#include "word.h"
#include "code.h"
#include <stdio.h>
#include <stdlib.h>

//
// Constructor for a word.
//
// syms:    Array of symbols a Word represents.
// len:     Length of the array of symbols.
// returns: Pointer to a Word that has been allocated memory.
//
Word *word_create(uint8_t *syms, uint64_t len) {
  //allocate memory for word
  Word *w = (Word *)malloc(sizeof(Word));
  //if word isn't set return 0
  if (!w) {
    return 0;
  }
  //allocates memory for w->sysm
  w->syms = (uint8_t *)malloc(sizeof(uint8_t));
  //if w->sysm isnt set return 0;
  if (!w->syms) {
    return 0;
  }
  //sets w->len to len
  w->len = len;
  //goes through syms
  for (uint64_t i = 0; i < len; i++) {
    //sets w->syms[i] to syms[i]
    w->syms[i] = syms[i];
  }
  //return w
  return w;
}

//
// Constructs a new Word from the specified Word appended with a symbol.
// The Word specified to append to may be empty.
// If the above is the case, the new Word should contain only the symbol.
//
// w:       Word to append to.
// sym:     Symbol to append.
// returns: New Word which represents the result of appending.
//
Word *word_append_sym(Word *w, uint8_t sym) {
  //if w == NULL meaning it isn't set
  if (w == NULL) {
    //sets syms array to sym
    uint8_t *syms = { &sym };
    //creates a word adt for new_word
    Word *new_word = word_create(syms, 1);
    //return new_word
    return new_word;
  }
  //if word is set append the word
  else {
    //creates a copy of w and sets it to new_word
    Word *new_word = word_create(w->syms, w->len);
    //reallocates spaces for the extra symbol
    new_word->syms = realloc(new_word->syms, w->len + 1);
    //checks if new_word is set
    if (!new_word->syms) {
      return 0;
    }
    //increments new_word->len
    new_word->len += 1;
    //sets the new space allocated for an extra sym to sym
    new_word->syms[new_word->len - 1] = sym;
    //returns new_word
    return new_word;
  }
}

//
// Destructor for a Word.
//
// w:       Word to free memory for.
// returns: Void.
//
void word_delete(Word *w) {
  //frees w->sysm and w
  free(w->syms);
  free(w);
  return;
}

//
// Creates a new WordTable, which is an array of Words.
// A WordTable has a pre-defined size of MAX_CODE (UINT16_MAX - 1).
// This is because codes are 16-bit integers.
// A WordTable is initialized with a single Word at index EMPTY_CODE.
// This Word represents the empty word, a string of length of zero.
//
// returns: Initialized WordTable.
//
WordTable *wt_create(void) {
  //allocates memory for wordtable wt
  WordTable *wt = calloc(MAX_CODE, sizeof(Word));
  //return wt
  return wt;
}

//
// Resets a WordTable to having just the empty Word.
//
// wt:      WordTable to reset.
// returns: Void.
//
void wt_reset(WordTable *wt) {
  //goes through the word table skipping EMPY_CODE
  for (int i = START_CODE; START_CODE < MAX_CODE; i++) {
    //deletes word
    word_delete(wt[i]);
  }
  return;
}
//
// Deletes an entire WordTable.
// All Words in the WordTable must be deleted as well.
//
// wt:      WordTable to free memory for.
// returns: Void.
//
void wt_delete(WordTable *wt) {
  //goes through the whole wordtable
  for (int i = 0; i <= MAX_CODE; i++) {
    //checks if wordtable is set at index i
    if (wt[i] != NULL) {
      //delete word at wt[i];
      word_delete(wt[i]);
    }
  }
  //free wordtable wt
  free(wt);
  return;
}
