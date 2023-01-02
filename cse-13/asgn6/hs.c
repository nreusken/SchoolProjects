#include "hs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

HatterSpeak *gs_create(char *oldspeak, char *hatterspeak) {
  //allocated memory for gs structure
  HatterSpeak *gs = (HatterSpeak *)malloc(sizeof(HatterSpeak));
  //if gs is not initialized return 0
  if (!gs) {
    return 0;
  }
  //old_len to the length of old speak plus one, to allocate the right amount of space for the string
  int old_len = strlen(oldspeak) + 1;
  //allocates memory for oldspeak word
  gs->oldspeak = (char *)calloc(old_len, sizeof(char));
  //if gs->oldspeak is not initialized return 0
  if (!gs->oldspeak) {
    return 0;
  }
  //sets gs->oldspeak to oldspeak
  strcpy(gs->oldspeak, oldspeak);

  //if hatterspeak does not equal word meaning it has a translation
  if (hatterspeak != NULL) {
    ////hatter_len to the length of hatter speak plus one, to allocate the right amount of space for the string
    int hatter_len = strlen(hatterspeak) + 1;
    //allocates memory for hatterspeak word
    gs->hatterspeak = (char *)calloc(hatter_len, sizeof(char));
    //if gs->hatterspeak is not initialized return 0
    if (!gs->hatterspeak) {
      return 0;
    }
    //sets gs->hatterspeak to hatterspeak
    strcpy(gs->hatterspeak, hatterspeak);
  }
  //sets gs->hatterspeak to NULL if it does not have a translation
  else {
    gs->hatterspeak = NULL;
  }
  //return hatterspeak structure gs
  return gs;
}

void gs_delete(HatterSpeak *gs) {
  //frees memory allocated at gs_create
  free(gs->hatterspeak);
  free(gs->oldspeak);
  free(gs);
  return;
}
