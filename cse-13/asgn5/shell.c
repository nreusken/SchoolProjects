#include "shell.h"
#include <stdio.h>
#include <stdlib.h>

//calculates the next gap using the given previous gap
int next_gap(int n) {
  //if n is smaller or equal to 2  set n to 1
  if (n <= 2) {
    n = 1;
  }
  //else multply 5 * n and divide by 11 rounding the number
  else {
    n = 5 * n / 11;
  }
  //return n
  return n;
}

//shell_sort sort array using shell sort algorithm
void shell_sort(int *array, int length) {
  //initialize moves and compares and set to 0
  int moves = 0;
  int compares = 0;
  //set gap to length
  int gap = length;
  //while gap is bigger than 1
  while (gap > 1) {
    //set gap to next_gap of gap
    gap = next_gap(gap);
    //iterate through the list starting at the gap position
    for (int i = gap; i < length; i += 1) {
      //itarate through the list till gap - 1 and starting at i
      for (int j = i; j > (gap - 1); j -= gap) {
        //increment compares by 1
        compares += 1;
        // if array[j] is less than array[j - gap] swap array[j] and  array[j - gap]
        if (array[j] < array[j - gap]) {
          //increment moves by 3
          moves += 3;
          //swap array[j] and  array[j - gap]
          int temp = array[j];
          array[j] = array[j - gap];
          array[j - gap] = temp;
        }
      }
    }
  }
  //print statistical header
  printf("Shell Sort\n");
  printf("%d elements, %d moves, %d compares\n", length, moves, compares);
  return;
}
