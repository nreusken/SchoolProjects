#include "bv.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

//CITATION: functions: bv_create, bv_set_bit, bv_clr_bit, bv_get_bit, and bv_set_bit do not all have my own code
//	Code was given during Eugene's Lab Section on wednesday, 11/4/2020.
//

//  Creates a new  BitVector  of  specified  length.
//  bit_len : The  length  in bits of the  BitVector.
BitVector *bv_create(uint32_t bit_len) {
  //allocates memory for Bitvector structure
  BitVector *v = (BitVector *)malloc(sizeof(BitVector));
  //checks if bitvector is true.
  if (!v) {
    return 0;
  }
  //sets bitvector to one if less than 1.
  if (bit_len < 1) {
    bit_len = 1;
  }
  //set bitvector lenght and allocates that lenght in memory for the bitvector.
  v->length = bit_len;
  v->vector = (uint8_t *)calloc(bit_len / 8 + 1, sizeof(uint8_t));
  //checks if vector is true.
  if (!v->vector) {
    return 0;
  }
  //return bitvector.
  return v;
}

// Frees  memory  allocated  for a BitVector.
// v : The  BitVector.
void bv_delete(BitVector *v) {
  free(v->vector);
  v->vector = NULL;
  free(v);
  return;
}

//  Returns  the  length  in bits of the  BitVector.
//v : The  BitVector.
uint32_t bv_get_len(BitVector *v) {
  return v->length;
}

// Sets  the  bit at index in the  BitVector.
// v : The  BitVector.
// i : Index of the bit to set.
void bv_set_bit(BitVector *v, uint32_t i) {
  //checks if bitvector is true.
  if (!v) {
    return;
  }
  //checks if i is within the length.
  if (i > v->length) {
    return;
  }

  i = i % v->length;
  //sets byte to which byte i is located in.
  uint8_t byte = v->vector[i / 8];
  //sets position to where i is with in the bit.
  uint8_t position = i % 8;
  //sets the byte where i is located to byte and mask.
  v->vector[i / 8] = byte | (1 << position);
  return;
}

// Clears  the bit at index in the  BitVector.
// v : The  BitVector.
// i : Index of the bit to  clear.
void bv_clr_bit(BitVector *v, uint32_t i) {
  //checks if bitvector is true.
  if (!v) {
    return;
  }
  //checks if i is within the length.
  if (i > v->length) {
    return;
  }

  i = i % v->length;
  //sets byte to which byte i is located in.
  uint8_t byte = v->vector[i / 8];
  //sets position to where i is with in the bit.
  uint8_t position = i % 8;
  //sets mask to which bit need to stay using not bitwise operation.
  uint8_t mask = ~(1 << position);
  //sets the byte where i is located to byte and mask.
  v->vector[i / 8] = byte & mask;
  return;
}

// Gets a bit  from a BitVector.
// v : The  BitVector.
// i : Index of the bit to get.
uint8_t bv_get_bit(BitVector *v, uint32_t i) {
  //checks if bitvector is true.
  if (!v) {
    return 0;
  }
  //checks if i is within the length
  if (i > v->length) {
    return 0;
  }
  //sets position to where i is with in the bit.
  uint8_t position = i % 8;
  //returns the bit at location i using shift and & bitwise operation.
  return (v->vector[i / 8] & (1 << position)) >> position;
}

// Sets  all  bits in a BitVector.
// v : The  BitVector.
void bv_set_all_bits(BitVector *v) {
  //checks if bitvector is true.
  if (!v) {
    return;
  }
  //parses through the bigtvector and calls bv_set_bit to set all bits to 1.
  for (uint32_t i = 0; i < v->length; i += 1) {
    bv_set_bit(v, i);
  }
  return;
}
