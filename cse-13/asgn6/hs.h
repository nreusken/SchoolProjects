#ifndef __HS_H__
#define __HS_H__

typedef struct HatterSpeak {
  char *oldspeak;
  char *hatterspeak;
} HatterSpeak;

//
//creates hatterspeak struct with oldspeak and hatterspeak words
//
//	oldspeak: old speak word
//	hatterspeak: hatterspeak word
HatterSpeak *gs_create(char *oldspeak, char *hatterspeak);

//
//deletes the hatterspeak struct gs
//
//	gs: an hatterspeak struc
//
//
void gs_delete(HatterSpeak *gs);

#endif
