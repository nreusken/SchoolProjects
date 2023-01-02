#include "bf.h"
#include "bv.h"
#include "speck.h"
#include <stdio.h>
#include <stdlib.h>

//
//  Constructor  for a BloomFilter.
//
// size:   The  number  of  entries  in the  BloomFilter.
//
BloomFilter *bf_create(uint32_t size) {
  BloomFilter *bf = (BloomFilter *)malloc(sizeof(BloomFilter));
  if (bf) {
    bf->primary[0] = 0xfc28ca6885711cf7;
    bf->primary[1] = 0x2841af568222f773;
    bf->secondary[0] = 0x85ae998311115ae3;
    bf->secondary[1] = 0xb6fac2ae33a40089;
    bf->tertiary[0] = 0xd37b01df0ae8f8d0;
    bf->tertiary[1] = 0x911d454886ca7cf7;
    bf->filter = bv_create(size);
    return bf;
  }
  return (BloomFilter *)NIL;
}

//
//  Destructor  for a BloomFilter.
//
// bf:   The  BloomFilter.
//
void bf_delete(BloomFilter *bf) {
  //free allocated memory
  bv_delete(bf->filter);
  free(bf);
  return;
}

//
//  Inserts a new  key  into  the  BloomFilter.
//  Indices  to set  bits  are  given by the   hash  functions.
//
// bf:     The  BloomFilter.
// key:    The  key to  insert  into  the  BloomFilter.
//
void bf_insert(BloomFilter *bf, char *key) {
  //creates hash index for key.
  uint32_t hash_num = (hash(bf->primary, key)) % bv_get_len(bf->filter);
  //sets bit at hash_num
  bv_set_bit(bf->filter, hash_num);
  //creates new hash index
  hash_num = (hash(bf->secondary, key))
             % bv_get_len(bf->filter); //sets bit at hash_num
  bv_set_bit(bf->filter, hash_num);
  //creates new hash index
  hash_num = (hash(bf->tertiary, key)) % bv_get_len(bf->filter);
  //sets bit at hash_num
  bv_set_bit(bf->filter, hash_num);
}

//
//  Probes a BloomFilter  to check if a key has  been  inserted.
//
// bf:     The  BloomFilter.
// key:    The  key in which to check  membership.
//
bool bf_probe(BloomFilter *bf, char *key) {
  //creates hash index for key.
  uint32_t hash_num = (hash(bf->primary, key)) % bv_get_len(bf->filter);
  //checks if bit is set at primary hash
  if (bv_get_bit(bf->filter, hash_num)) {
    //creates new hash index for key
    hash_num = (hash(bf->secondary, key)) % bv_get_len(bf->filter);
    //checks if bit is set at secondary hash
    if (bv_get_bit(bf->filter, hash_num)) {
      //creates new hash index for key
      hash_num = (hash(bf->tertiary, key)) % bv_get_len(bf->filter);
      //check if bit is set at tertiary hash
      if (bv_get_bit(bf->filter, hash_num)) {
        //if set at all hashes return trues
        return true;
      }
    }
  }
  //else return false
  return false;
}
