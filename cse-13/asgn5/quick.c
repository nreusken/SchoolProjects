#include "quick.h"
#include <stdio.h>
#include <stdlib.h>

//initialize moves and compares to 0 on global scope so they can be changed in both functions
int moves, compares = 0;

//partitions the array
int partition(int *array, int left, int right) {
  //set pivot to array[left]
  int pivot = array[left];
  //sets lo to left + 1
  int lo = left + 1;
  //set hi to right
  int hi = right;

  while (true) {
    //increment compares by 2
    compares += 2;
    //increase hi while hi is bigger or equal to lo and if array[hi] is bigger or equal to pivot
    while ((lo <= hi) && (array[hi] >= pivot)) {
      hi -= 1;
    }
    //increment compares by 2
    compares += 2;
    //increase hi while hi is bigger or equal to lo and if array[lo] is smaller or equal to pivot
    while ((lo <= hi) && (array[lo] <= pivot)) {
      lo += 1;
    }
    //increment compares by 1
    compares += 1;
    //if hi is bigger or equal to lo swap array[hi] and array[lo]
    if (lo <= hi) {
      //increment moves by 3
      moves += 3;
      //swap array[hi] and array[lo]
      int temp = array[lo];
      array[lo] = array[hi];
      array[hi] = temp;
    }
    //else break the loop
    else {
      break;
    }
  }
  //increment moves by 3
  moves += 3;
  //swap array[hi] and array[left]
  int temp = array[left];
  array[left] = array[hi];
  array[hi] = temp;
  //return hi
  return hi;
}

void quick_sort(int *array, int left, int right, int length) {
  //increase compare by one
  compares += 1;
  //if left is smaller than right
  if (left < right) {
    //set index to what is returned when calling partition
    int index = partition(array, left, right);
    //call quick_sort swapping right with index -1
    quick_sort(array, left, (index - 1), length);
    //call quick_sort swapping left with index + 1
    quick_sort(array, (index + 1), right, length);
  }
  //if left == 0 and right == length -1 meaning that list has been sorted
  if ((left == 0) && (right == (length - 1))) {
    //print statistacal header
    printf("Quick Sort\n");
    printf("%d elements, %d moves, %d compares\n", length, moves, compares);
  }
  return;
}
