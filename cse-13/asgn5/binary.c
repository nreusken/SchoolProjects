#include "binary.h"
#include <stdio.h>
#include <stdlib.h>

//Sorts array using the Binary Insertion algorithm
//array: an array of numbers
//length: the length of an array

//citation: pseudocode from lab manual

void binary_insertion(int *array, int length) {
  //set moves and compares to 0
  int moves = 0;
  int compares = 0;
  //goes through all the elements inside the array
  for (int i = 1; i < length; i += 1) {
    //sets value to element at i
    int value = array[i];
    //set left to 0 and right to i
    int left = 0;
    int right = i;
    //while left is smaller than right
    while (left < right) {
      //increment compare
      compares += 1;
      //finds the middle
      int mid = left + ((right - left) / 2);
      //compares value to middle
      if (value >= array[mid]) {
        left = mid + 1;
      }
      //if not bigger or equal set right to mid
      else {
        right = mid;
      }
    }
    //iterates throuhg j till left is smaller than j
    for (int j = i; j > left; j -= 1) {
      moves += 3;
      //swap array[j] and array[j - 1]
      int temp = array[j - 1];
      array[j - 1] = array[j];
      array[j] = temp;
    }
  }
  //prints stats for binary insertion sort
  printf("Binary Insertion Sort\n");
  printf("%d elements, %d moves, %d compares\n", length, moves, compares);
  return;
}
