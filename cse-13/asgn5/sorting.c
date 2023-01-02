#include "binary.h"
#include "bubble.h"
#include "quick.h"
#include "shell.h"
#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//prints given array.
void print_array(int *a, int length) {
  //iterates through each element inside array.
  for (int i = 0; i <= length - 1; i += 1) {
    //prints array following lab printing guidelines.
    printf("%13" PRIu32, a[i]);
    //if 7th element in row is printed, print enter to start a new row.
    if ((i + 1) % 7 == 0) {
      printf("\n");
    }
  }
  //print enter after last element,
  printf("\n");
  return;
}

//Makes a copy of the array, this is created to prevent the actaul random array to be edited.
//Since the array needs to be sorted any amount of times for all requested sorting algorithms.
//It is better to create copies of the actaul array.
void copy_array(int *array, int *new_array, int length) {
  //iterates the the arrays and sets all the numbers from array to new_array each respective position.
  for (int i = 0; i <= (length - 1); i += 1) {
    new_array[i] = array[i];
  }
}

//Main function which mainly calls the other functions.
int main(int argc, char **argv) {
  int c = 0;

  //sets first_elements to 100 which is the default first number of elements that should be printed.
  int first_elements = 100;

  //sets seed to 8222022 which is the default random seed.
  int seed = 8222022;

  //sets size_array to 100 which is the default size of the array.
  int size_array = 100;

  //sets bools for getopt loop.
  bool all = false;
  bool bubble = false;
  bool shell = false;
  bool quick = false;
  bool insertion = false;

  //checks what command line arguments are given.
  while ((c = getopt(argc, argv, "Absqip:r:n:")) != -1) {
    switch (c) {
    //if -p is given, set the n after -p to first_elements.
    case 'p':
      sscanf(optarg, "%d", &first_elements);
      break;
    //if -r is given, set the x after -r to seed.
    case 'r':
      sscanf(optarg, "%d", &seed);
      break;
    //if -n is given, set the x after -n to seed.
    case 'n':
      sscanf(optarg, "%d", &size_array);
      break;
    //if -A is given sets all true.
    case 'A':
      bubble = true;
      shell = true;
      quick = true;
      insertion = true;
      break;
    //if -b is given set bubble true.
    case 'b':
      bubble = true;
      break;
    //if -s is given set shell true.
    case 's':
      shell = true;
      break;
    //if -q is given set quick true.
    case 'q':
      quick = true;
      break;
    //if -i is given set insertion true.
    case 'i':
      insertion = true;
      break;
    }
  }

  //if no arguments given, return an error.
  if (argc == 1) {
    bubble = true;
    shell = true;
    quick = true;
    insertion = true;
  }
  //if given size_array is less than 1 return an error.
  if (size_array < 1) {
    puts("Error: the length of array is too low!");
    return -1;
  }

  //allocates array with length size_array.
  int *array = (int *)calloc(size_array, sizeof(int));

  //sets random seed to seed.
  srand(seed);

  //check if array is actuall allocated
  if (!array) {
    return -1;
  }

  //sets all positions in the array to a random number with 30 bits as a max.
  for (int i = 0; i <= (size_array - 1); i += 1) {
    array[i] = (rand() & 1073741823);
  }

  //if bubble sort algorithm is requested
  if (bubble) {
    //initialize array
    int *bubble_array = (int *)calloc(size_array, sizeof(int));
    //check if bubble array is initialized
    if (!bubble_array) {
      return -1;
    }
    //set bubble array to array
    copy_array(array, bubble_array, size_array);
    //call bubble sort on bubble_array
    bubble_sort(bubble_array, size_array);
    //print bubble array
    print_array(bubble_array, first_elements);
    //free alocated space for bubble array
    free(bubble_array);
  }
  //if shell sort algorithm is requested
  if (shell) {
    //initialize shell array
    int *shell_array = (int *)calloc(size_array, sizeof(int));
    //check if shell array is initialized
    if (!shell_array) {
      return -1;
    }
    //set shell array to array
    copy_array(array, shell_array, size_array);
    //call shell sort on shell_array
    shell_sort(shell_array, size_array);
    //print shell array
    print_array(shell_array, first_elements);
    //free alocated space for shell array
    free(shell_array);
  }
  //if quick sort algorithm is requested
  if (quick) {
    //initialize quick array
    int *quick_array = (int *)calloc(size_array, sizeof(int));
    //check if quick array is initialized
    if (!quick_array) {
      return -1;
    }
    //set quick array to array
    copy_array(array, quick_array, size_array);
    //call quick sort on quick_array
    quick_sort(quick_array, 0, (size_array - 1), size_array);
    //print quick array
    print_array(quick_array, first_elements);
    //free alocated space for quick array
    free(quick_array);
  }
  //if binary insertion sort algorithm is requested
  if (insertion) {
    //initialize insertion array
    int *insertion_array = (int *)calloc(size_array, sizeof(int));
    //check if insertion array is initialize
    if (!insertion_array) {
      return -1;
    }
    //set insertion array to array
    copy_array(array, insertion_array, size_array);
    //call binary insertion sort on insertion_array
    binary_insertion(insertion_array, size_array);
    //print insertion array
    print_array(insertion_array, first_elements);
    //free alocated space for insertion array
    free(insertion_array);
  }
  //free alocated space for array
  free(array);
  return 0;
}
