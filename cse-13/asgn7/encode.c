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
#include <sys/uio.h>
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
void compress(int infile, int outfile) {
  //initializes the root
  TrieNode *root = trie_create();
  //the current node is pointing at the rood
  TrieNode *curr_node = root;
  //checks if curr_node does not equal NULL
  if (!curr_node) {
    return;
  }
  //initializes prev_node
  TrieNode *prev_node = NULL;
  //initializes curr_sym, prev_sym and next code
  uint8_t curr_sym = 0;
  uint8_t prev_sym = 0;
  uint16_t next_code = START_CODE;
  //iterates till read_sym returns false
  while (read_sym(infile, &curr_sym)) {
    //sets next_code to what the code of the curr node is with current sym
    TrieNode *next_node = trie_step(curr_node, curr_sym);
    //if node does not equal null
    if (next_node != NULL) {
      //set prev_node to curr_node
      prev_node = curr_node;
      //set curr_node to next_node
      curr_node = next_node;
    }
    //if next_node equall NULL
    else {
      //buffer the pair
      buffer_pair(outfile, curr_node->code, curr_sym, bit_length(next_code));
      //create new node and seeting it to one of the children
      curr_node->children[curr_sym] = trie_node_create(next_code);
      //set curr_node to root
      curr_node = root;
      //set next_code to next_code plus one
      next_code = next_code + 1;
    }
    //if next_code equal MAX_CODE
    if (next_code == MAX_CODE) {
      //reset the root and set curr_node to root and reset next_code
      trie_reset(root);
      curr_node = root;
      next_code = START_CODE;
    }
    //set prev_sym to curr_sym
    prev_sym = curr_sym;
  }
  //if curr_node does not equal root
  if (curr_node != root) {
    //buffer the rest of the trie
    buffer_pair(outfile, prev_node->code, prev_sym, bit_length(next_code));
    //next_code = next_code + 1 modulo max_code to make sure it isn't bigger than max_code
    next_code = (next_code + 1) % MAX_CODE;
  }
  //deletes the whole trie
  trie_delete(root);
  //buffers the pair
  buffer_pair(outfile, STOP_CODE, 0, bit_length(next_code));
  //flushesh what is left in pairs
  flush_pairs(outfile);
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
  //goes through what is put in command line argument
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

  //create the stat struct to get the protection from infile
  struct stat infile_stats;
  //get the file stats for infile
  fstat(infile, &infile_stats);
  //gives the outfile the same protection as outfile
  fchmod(outfile, infile_stats.st_mode);

  //creates outfile header and allocates memory for it
  struct FileHeader *outfile_header
      = (FileHeader *)calloc(1, sizeof(FileHeader));
  //checks if outfile_header is initialized
  if (!outfile_header) {
    return 0;
  }
  //sets header magic to magic
  outfile_header->magic = MAGIC;
  //sets header protection to the protection of infile
  outfile_header->protection = infile_stats.st_mode;
  //writes the header to the outfile
  write_header(outfile, outfile_header);

  //call compress on infile to outfile, compressing the file
  compress(infile, outfile);

  //if stats is true
  if (stats) {
    //prints the stats of the file
    printf("Compressed file size: %d bytes\n", bytes_written);
    printf("Uncompressed file size: %d bytes\n", bytes_read);
    //calculates the compressed ratio and prints that ratio
    double compressed_ratio
        = 100 * (1 - ((double)bytes_written / (double)bytes_read));
    printf("Compression ratio: %f%%\n", compressed_ratio);
  }
  //fress the allocated memory for outfile_header
  free(outfile_header);
  //close infile and outfile
  close(infile);
  close(outfile);

  return 0;
}
