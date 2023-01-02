#include "io.h"
#include "code.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BLOCK 4096

//citation: Went to Eugenes lab section on wednesday 12/2 and 12/9 where most of the design for this code is from.

//bytes_read and bytes_written from deocde.c and encode.c
extern int bytes_read;
extern int bytes_written;

//
// Wrapper for the read() syscall.
// Loops to read the specified number of bytes, or until input is exhausted.
// Returns the number of bytes read.
//
// infile:  File descriptor of the input file to read from.
// buf:     Buffer to store read bytes into.
// to_read: Number of bytes to read.
// returns: Number of bytes read.
//
int read_bytes(int infile, uint8_t *buf, int to_read) {
  //initiallizes total_read and bytes
  int total_read = 0;
  int bytes = 0;
  //loops till all bytes are read from the file in to the buffer
  do {
    //call read, reading from infile to buf and setting the amount read to bytes
    bytes = read(infile, buf + total_read, to_read - total_read);
    //incrementing total_read by the amounto of bytes read
    total_read += bytes;
  } while (bytes > 0 && total_read != to_read);
  //incremenets bytes_read by total_read fromm function
  bytes_read += total_read;
  //returns total_read
  return total_read;
}

// Wrapper for the write() syscall.
// Loops to write the specified number of bytes, or until nothing is written.
// Returns the number of bytes written.
//
// outfile:   File descriptor of the output file to write to.
// buf:       Buffer that stores the bytes to write out.
// to_write:  Number of bytes to write.
// returns:   Number of bytes written.
//
int write_bytes(int outfile, uint8_t *buf, int to_write) {
  //initiallizes total_written and bytes
  int total_written = 0;
  int bytes = 0;
  //loops till all bytes are written from buffer and in to outfile
  do {
    //call write, writing in to outfile from buf and setting the amount written to bytes
    bytes = write(outfile, buf + total_written, to_write - total_written);
    //incrementing total_written by the amounto of bytes written
    total_written += bytes;
  } while (bytes > 0 && total_written != to_write);
  //incremenets bytes_written by total_written fromm function
  bytes_written += total_written;
  //returns total_read
  return total_written;
}

//
// Reads in a FileHeader from the input file.
//
// infile:  File descriptor of input file to read header from.
// header:  Pointer to memory where the bytes of the read header should go.
// returns: Void.
//
void read_header(int infile, FileHeader *header) {
  //calls read_bytes to read header from infile
  read_bytes(infile, (uint8_t *)header, sizeof(FileHeader));
  return;
}

//
// Writes a FileHeader to the output file.
//
// outfile: File descriptor of output file to write header to.
// header:  Pointer to the header to write out.
// returns: Void.
//
void write_header(int outfile, FileHeader *header) {
  //calls write_bytes to write header into infile
  write_bytes(outfile, (uint8_t *)header, sizeof(FileHeader));
  return;
}

//
// "Reads" a symbol from the input file.
// The "read" symbol is placed into the pointer to sym (pass by reference).
// In reality, a block of symbols is read into a buffer.
// An index keeps track of the currently read symbol in the buffer.
// Once all symbols are processed, another block is read.
// If less than a block is read, the end of the buffer is updated.
// Returns true if there are symbols to be read, false otherwise.
//
// infile:  File descriptor of input file to read symbols from.
// sym:     Pointer to memory which stores the read symbol.
// returns: True if there are symbols to be read, false otherwise.
//

//buffers used to buffer symbols
static uint8_t symbuf[BLOCK];
//index used to know where at inside of symbuf
static int sym_index = 0;

bool read_sym(int infile, uint8_t *sym) {
  //sets end
  static int end = 0;
  //sets end if index is 0
  if (sym_index == 0) {
    //reads the bytes into symbuf from infile and sets number of bytes read to end
    end = read_bytes(infile, symbuf, BLOCK);
  }
  //sets sym to symbol at symbuf[index];
  *sym = symbuf[sym_index++];
  //if sym_index == BLOCK resest sym_index
  if (sym_index == BLOCK) {
    sym_index = 0;
  }
  //if end == block return true
  if (end == BLOCK) {
    return true;
  }
  //else check if sym_index == end + 1 because then return false else return true
  else {
    if (sym_index == end + 1) {
      return false;
    } else {
      return true;
    }
  }
}

//
// Buffers a pair. A pair is comprised of a symbol and an index.
// The bits of the symbol are buffered first, starting from the LSB.
// The bits of the index are buffered next, also starting from the LSB.
// bit_len bits of the index are buffered to provide a minimal representation.
// The buffer is written out whenever it is filled.
//
// outfile: File descriptor of the output file to write to.
// sym:     Symbol of the pair to buffer.
// index:   Index of the pair to buffer.
// bit_len: Number of bits of the index to buffer.
// returns: Void.
//

//bitbuf buffers the pairs of bits
static uint8_t bitbuf[BLOCK];
//bit_index keeps count of where inside of bitbuf we are
static int bit_index = 0;

void buffer_pair(int outfile, uint16_t code, uint8_t sym, uint8_t bit_len) {
  //loops through code
  for (int bit = 0; bit < bit_len; bit += 1) {
    //if LSB in code = 1 set bit within bifbuf
    if ((code & 1) == 1) {
      //sets bit at bit_index
      bitbuf[bit_index / 8] |= (1 << (bit_index % 8));
    }
    //else clear bit within bitbuf
    else {
      //clear bit at bit_index
      bitbuf[bit_index / 8] &= ~(1 << (bit_index % 8));
    }
    //incerment bit index to go to next bit
    bit_index += 1;
    //got to next bit within code
    code >>= 1;
    //if bit_index == block * 8
    if (bit_index == (BLOCK * 8)) {
      //write out the bitbuf to outfile
      write_bytes(outfile, bitbuf, BLOCK);
      //reset bit_index
      bit_index = 0;
    }
  }
  //iterates through sym
  for (int bit = 0; bit < 8; bit += 1) {
    //if lsb in sym = 1 set bit in bitbuf
    if ((sym & 1) == 1) {
      //sets bit at bit_index
      bitbuf[bit_index / 8] |= (1 << (bit_index % 8));
    }
    //else clear bit within bitbuf
    else {
      //clear bit at bit_index
      bitbuf[bit_index / 8] &= ~(1 << (bit_index % 8));
    }
    //incerment bit index to go to next bit
    bit_index += 1;
    //got to next bit within sym
    sym >>= 1;
    //if bit_index == block * 8
    if (bit_index == (BLOCK * 8)) {
      //write out the bitbuf to outfile
      write_bytes(outfile, bitbuf, BLOCK);
      //reset bit_index
      bit_index = 0;
    }
  }
  return;
}

//
// Writes out any remaining pairs of symbols and indexes to the output file.
//
// outfile: File descriptor of the output file to write to.
// returns: Void.
//
void flush_pairs(int outfile) {
  //sets bytes to 0
  int bytes = 0;
  //checks if bitbuf is not empty
  if (bit_index != 0) {
    //if bit index does not have a remainder when divided by 8
    if ((bit_index % 8) == 0) {
      //set bytes to bitindex divide by 8
      bytes = bit_index / 8;
    }
    //if bit index does have a remainded
    else {
      //set bytes to bitindex divide by 8 + 1
      bytes = (bit_index / 8) + 1;
    }
    //write bitbuf to outfile
    write_bytes(outfile, bitbuf, bytes);
  }
  return;
}

//
// "Reads" a pair (symbol and index) from the input file.
// The "read" symbol is placed in the pointer to sym (pass by reference).
// The "read" index is placed in the pointer to index (pass by reference).
// In reality, a block of pairs is read into a buffer.
// An index keeps track of the current bit in the buffer.
// Once all bits have been processed, another block is read.
// The first 8 bits of the pair constitute the symbol, starting from the LSB.
// The next bit_len bits constitutes the index, starting from the the LSB.
// Returns true if there are pairs left to read in the buffer, else false.
// There are pairs left to read if the read index is not STOP_INDEX.
//
// infile:  File descriptor of the input file to read from.
// sym:     Pointer to memory which stores the read symbol.
// index:   Pointer to memory which stores the read index.
// bit_len: Length in bits of the index to read.
// returns: True if there are pairs left to read, false otherwise.
//
bool read_pair(int infile, uint16_t *code, uint8_t *sym, uint8_t bit_len) {
  //sets code to 0 making sure it is reset
  *code = 0;
  //loops through code
  for (int bit = 0; bit < bit_len; bit += 1) {
    //if bit_index == 0
    if (bit_index == 0) {
      //read from infile to bitbuf
      read_bytes(infile, bitbuf, BLOCK);
    }
    //if bit is set at bit_index in bitbuf
    if (((bitbuf[bit_index / 8] & (1 << (bit_index % 8))) >> (bit_index % 8))
        == 1) {
      //set bit at bit in code
      *code |= (1 << (bit % 16));
    }
    //if bit is not set at bit_index in bitbuf
    else {
      //clear bit at bit in code
      *code &= ~(1 << (bit % 16));
    }
    //incremenet bit_index to go to next bit in bitbuf
    bit_index += 1;
    //if bit_index == BLOCK * 8 reset bit_index
    if (bit_index == (BLOCK * 8)) {
      bit_index = 0;
    }
  }
  //sets sym to 0 making sure it is reset
  *sym = 0;
  //loops through sym
  for (int bit = 0; bit < 8; bit += 1) {
    //if bit_index == 0
    if (bit_index == 0) {
      //read from infile to bitbuf
      read_bytes(infile, bitbuf, BLOCK);
    }
    //if bit is set at bit_index in bitbuf
    if (((bitbuf[bit_index / 8] & (1 << (bit_index % 8))) >> (bit_index % 8))
        == 1) {
      //set bit at bit in code
      *sym |= (1 << (bit));
    }
    //if bit is not set at bit_index in bitbuf
    else {
      //clear bit at bit in sym
      *sym &= ~(1 << (bit));
    }
    //incremenet bit_index to go to next bit in bitbuf
    bit_index += 1;
    //if bit_index == BLOCK * 8 reset bit_index
    if (bit_index == (BLOCK * 8)) {
      bit_index = 0;
    }
  }
  //return true if code does not equal stop_code
  return *code != STOP_CODE;
}

//
// Buffers a Word, or more specifically, the symbols of a Word.
// Each symbol of the Word is placed into a buffer.
// The buffer is written out when it is filled.
//
// outfile: File descriptor of the output file to write to.
// w:       Word to buffer.
// returns: Void.
//
void buffer_word(int outfile, Word *w) {
  //goes through word w
  for (uint32_t i = 0; i < w->len; i += 1) {
    //sets the word in to the symbuf
    symbuf[sym_index++] = w->syms[i];
    //if symbuf is full write and reset sym_index
    if (sym_index == BLOCK) {
      //writes symbuf to outfile
      write_bytes(outfile, symbuf, BLOCK);
      //resets sym_index
      sym_index = 0;
    }
  }
  return;
}
//
// Writes out any remaining symbols in the buffer.
//
// outfile: File descriptor of the output file to write to.
// returns: Void.
//
void flush_words(int outfile) {
  //checks if sym_index isn't empty
  if (sym_index != 0) {
    //writes the left over symbols in symbuf
    write_bytes(outfile, symbuf, sym_index);
  }
  return;
}
