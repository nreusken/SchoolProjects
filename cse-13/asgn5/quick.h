#ifndef __QUICK_H__
#define __QUICK_H__

#include <stdbool.h>

//Partitions through the array
//array: give array that needs to be partitioned
//left: left bound
//right: right bound

int partition(int *array, int left, int right);

//quick_sort sorts an given list and sorts it usinng quick sort algorithm.
//array: any given array that needs to be sorted
//left: when calling first left is 0
//right: when calling first right is length of the array - 1
//length: lenght of an array.

void quick_sort(int *array, int left, int right, int length);

#endif
