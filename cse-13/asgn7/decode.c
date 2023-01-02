#include "code.h"
#include "io.h"
#include "trie.h"
#include <fcntl.h>
#include <getopt.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

//initializes the bytes_read and bytes_written to count the size of infile and outfile
int bytes_read = 0;
int bytes_written = 0;

//calculates the lenght of a bit
uint8_t bit_length(uint16_t number) {
  return (uint8_t)log2(number) + 1;
}

//function that contains the LZ78 compression algorithm
//This algorithm is given in the lab manual, not my own code
void decompress(int infile, int outfile) {
  //create the wordtable table
  WordTable *table = wt_create();
  //checks if table is initialized
  if (!table) {
    return;
  }
  //initializes curr_sym, curr_code and next code
  uint8_t curr_sym = 0;
  uint16_t curr_code = 0;
  uint16_t next_code = START_CODE;
  //loops till read_pair returns false
  while (read_pair(infile, &curr_code, &curr_sym, bit_length(next_code))) {
    //sets in table at next_code append the word
    table[next_code] = word_append_sym(table[curr_code], curr_sym);
    //buffer the word in table at next_code
    buffer_word(outfile, table[next_code]);
    //increment next_code
    next_code = next_code + 1;
    //if next_code equal max code, reset table and set next_code to START_CODE
    if (next_code == MAX_CODE) {
      wt_reset(table);
      next_code = START_CODE;
    }
  }
  //flush word to outfile that are remaining inside the buffer
  flush_words(outfile);
  //delete the wordtable table
  wt_delete(table);
  return;
}

int main(int argc, char **argv) {
  int c = 0;
  //set bools for stats stdin and stiout
  bool stats = false;
  bool std_in = true;
  bool std_out = true;

  //set char for input_file
  char input_file[50];
  //set char for output_file
  char output_file[50];
  //goes the set bools for stats stdin and stioutrough what is put in command line argument
  while ((c = getopt(argc, argv, "vi:o:")) != -1) {
    switch (c) {
      //if v is inputted set stats true
    case 'v':
      stats = true;
      break;
    //if i is inputted set std_in flase and set the file inputted to input_file
    case 'i':
      sscanf(optarg, "%s", input_file);
      std_in = false;
      break;
      //if o is inputted set std_out false and set the file inputted to output_file
    case 'o':
      sscanf(optarg, "%s", output_file);
      std_out = false;
      break;
    }
  }
  //sets infile;
  int infile;
  //if std_in true allow for command line input for infile
  if (std_in) {
    infile = STDIN_FILENO;
  }
  //else open the file requested and set it to infile
  else {
    infile = open(input_file, O_RDONLY);
  }
  //sets outfile;
  int outfile;
  //if std_out true allow for command line output for outfile
  if (std_out) {
    outfile = STDOUT_FILENO;
  }
  //else open the file requested and set it to outfile
  else {
    outfile = open(output_file, O_WRONLY | O_CREAT | O_TRUNC);
  }

  //create a header for infile and allocates memory for it
  struct FileHeader *infile_header = (FileHeader *)malloc(sizeof(FileHeader));
  //reads the header from infile and sets it to the header infile
  read_header(infile, infile_header);

  //if the infile header does not have the right magic word give an error
  if (infile_header->magic != MAGIC) {
    puts("Error: the file you are trying to decode does not have the right "
         "magic number");
    return -1;
  }

  //set the protection for outfile to the protection from the header from infile
  fchmod(outfile, infile_header->protection);

  //call decompress on infile to outfile, decompressing infile
  decompress(infile, outfile);

  //if stats is true
  if (stats) {
    //prints the stats of the file
    printf("Compressed file size: %d bytes\n", bytes_read);
    printf("Uncompressed file size: %d bytes\n", bytes_written);
    //calculates the compressed ratio and prints that ratio
    double compressed_ratio
        = 100 * (1 - ((double)bytes_read / (double)bytes_written));
    printf("Compression ratio: %f%%\n", compressed_ratio);
  }

  //fress the allocated memory for infile_header
  free(infile_header);
  //close infile and outfile
  close(infile);
  close(outfile);
  return 0;
}
