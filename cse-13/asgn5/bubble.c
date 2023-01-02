#include "bubble.h"
#include <stdio.h>
#include <stdlib.h>

//Citation: Psuedocode is from the lab

//Sorts array using bubble sort implementation
//
//array: the array being sorted

void bubble_sort(int *array, int length) {
  //inistializes moves and compares and sets them 0
  int moves = 0;
  int compares = 0;
  //iterates through array
  for (int i = 0; i < length - 1; i += 1) {
    //sets j to lenght - 1
    int j = length - 1;
    while (j > i) {
      //increment compare by 1
      compares += 1;
      //if array[j] is smaller than array[j -1]
      if (array[j] < array[j - 1]) {
        moves += 3;
        //swap array[j] and array[j - 1]
        int temp = array[j];
        array[j] = array[j - 1];
        array[j - 1] = temp;
      }
      //j minus - 1
      j -= 1;
    }
  }
  //print statistical header
  printf("Bubble Sort\n");
  printf("%d elements, %d moves, %d compares\n", length, moves, compares);
  return;
}
