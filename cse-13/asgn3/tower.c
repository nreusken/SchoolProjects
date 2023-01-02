#include "stack.h"
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>

//move counter(static so it doesn't reset every time during recursion).
static int moves = 0;

//recursion implementation.
//Eugene's lab section gave us psuedo code to solving this.
void recursion(
    int number_disks, char starting_peg, char ending_peg, char temp_peg) {
  //if it is the top disk add it to the ending_peg.
  if (number_disks == 1) {
    printf("Move disk %d from peg %c to peg %c\n", number_disks, starting_peg,
        ending_peg);
    moves = moves + 1;
  }
  //if not the last disk.
  else {
    recursion(number_disks - 1, starting_peg, temp_peg, ending_peg);
    printf("Move disk %d from peg %c to peg %c\n", number_disks, starting_peg,
        ending_peg);
    moves = moves + 1;
    recursion(number_disks - 1, temp_peg, ending_peg, starting_peg);
  }
  return;
}

//stack implementation.
//Eugene's Lab section gave me the idea of doing the even or odd.
void stack_func(
    int number_disks, char starting_peg, char ending_peg, char temp_peg) {
  //creates the stacks for starting ending and temp pegs.
  Stack *A = stack_create(number_disks, starting_peg);
  Stack *B = stack_create(number_disks, ending_peg);
  Stack *C = stack_create(number_disks, temp_peg);

  //puts all the disks onto the starting peg.
  for (int i = number_disks; i > 0; i -= 1) {
    stack_push(A, i);
  }

  //if the amount of disks is even.
  if (number_disks % 2 == 0) {
    //till loop breaks, so when stack B is full.
    while (true) {
      //move from A to C or C to A, which ever is legal.
      //if from A to C is legal.
      if ((stack_peek(A) < stack_peek(C) || stack_empty(C))
          && !stack_empty(A)) {
        printf("Move disk %d from peg %c to peg %c\n", stack_peek(A), A->name,
            C->name);
        stack_push(C, stack_peek(A));
        stack_pop(A);
        moves += 1;
      }
      //else move from C to A.
      else if ((stack_peek(C) < stack_peek(A) || stack_empty(A))
               && !stack_empty(C)) {
        printf("Move disk %d from peg %c to peg %c\n", stack_peek(C), C->name,
            A->name);
        stack_push(A, stack_peek(C));
        stack_pop(C);
        moves += 1;
      }
      //move from A to B or B to A, which ever is legal.
      //if from A to B is legal.
      if ((stack_peek(A) < stack_peek(B) || stack_empty(B))
          && !stack_empty(A)) {
        printf("Move disk %d from peg %c to peg %c\n", stack_peek(A), A->name,
            B->name);
        stack_push(B, stack_peek(A));
        stack_pop(A);
        moves += 1;
        //if stack B is full break the loop.
        if (B->top == number_disks) {
          break;
        }
      }
      //else move from B to A
      else if ((stack_peek(B) < stack_peek(A) || stack_empty(A))
               && !stack_empty(B)) {
        printf("Move disk %d from peg %c to peg %c\n", stack_peek(B), B->name,
            A->name);
        stack_push(A, stack_peek(B));
        stack_pop(B);
        moves += 1;
      }
      //move from C to B or B to C, which ever is legal.
      //if from C to B is legal.
      if ((stack_peek(C) < stack_peek(B) || stack_empty(B))
          && !stack_empty(C)) {
        printf("Move disk %d from peg %c to peg %c\n", stack_peek(C), C->name,
            B->name);
        stack_push(B, stack_peek(C));
        stack_pop(C);
        moves += 1;
        //if stack B is full break the loop.
        if (B->top == number_disks) {
          break;
        }
      }
      //else move from B to C.
      else if ((stack_peek(B) < stack_peek(C) || stack_empty(C))
               && !stack_empty(B)) {
        printf("Move disk %d from peg %c to peg %c\n", stack_peek(B), B->name,
            C->name);
        stack_push(C, stack_peek(B));
        stack_pop(B);
        moves += 1;
      }
    }
  }

  //if the amount of disks is odd
  else {
    //till loop breaks, so when stack B is full.
    while (true) {
      //move from A to B or B to A, which ever is legal.
      //if from A to B is legal.
      if ((stack_peek(A) < stack_peek(B) || stack_empty(B))
          && !stack_empty(A)) {
        printf("Move disk %d from peg %c to peg %c\n", stack_peek(A), A->name,
            B->name);
        stack_push(B, stack_peek(A));
        stack_pop(A);
        moves += 1;
        //if stack B is full break the loop
        if (B->top == number_disks) {
          break;
        }
      }
      //else move from B to A
      else if ((stack_peek(B) < stack_peek(A) || stack_empty(A))
               && !stack_empty(B)) {
        printf("Move disk %d from peg %c to peg %c\n", stack_peek(B), B->name,
            A->name);
        stack_push(A, stack_peek(B));
        stack_pop(B);
        moves += 1;
      }
      //move from A to C or C to A, which ever is legal.
      //if from A to C is legal.
      if ((stack_peek(A) < stack_peek(C) || stack_empty(C))
          && !stack_empty(A)) {
        printf("Move disk %d from peg %c to peg %c\n", stack_peek(A), A->name,
            C->name);
        stack_push(C, stack_peek(A));
        stack_pop(A);
        moves += 1;
      }
      //else move from C to A.
      else if ((stack_peek(C) < stack_peek(A) || stack_empty(A))
               && !stack_empty(C)) {
        printf("Move disk %d from peg %c to peg %c\n", stack_peek(C), C->name,
            A->name);
        stack_push(A, stack_peek(C));
        stack_pop(C);
        moves += 1;
      }
      //move from C to B or B to C, which ever is legal.
      //if from C to B is legal.
      if ((stack_peek(C) < stack_peek(B) || stack_empty(B))
          && !stack_empty(C)) {
        printf("Move disk %d from peg %c to peg %c\n", stack_peek(C), C->name,
            B->name);
        stack_push(B, stack_peek(C));
        stack_pop(C);
        moves += 1;
        //if stack B is full break the loop.
        if (B->top == number_disks) {
          break;
        }
      }
      //else move from B to C.
      else if ((stack_peek(B) < stack_peek(C) || stack_empty(C))
               && !stack_empty(B)) {
        printf("Move disk %d from peg %c to peg %c\n", stack_peek(B), B->name,
            C->name);
        stack_push(C, stack_peek(B));
        stack_pop(B);
        moves += 1;
      }
    }
  }
  //when loop is done meaning that all the pegs are on stack B.
  //Delete all the stacks(prevents memory leaks).
  stack_delete(A);
  stack_delete(B);
  stack_delete(C);
  return;
}

int main(int argc, char **argv) {
  int c = 0;

  //defines the starting, ending, and temp pegs.
  char starting_peg = 'A';
  char ending_peg = 'B';
  char temp_peg = 'C';

  //sets num_disks to 5 which is the default.
  int num_disks = 5;

  //sets bools for getopt loop.
  bool recur = false;
  bool stack = false;

  //checks what command line arguments are given
  while ((c = getopt(argc, argv, "n:sr")) != -1) {
    switch (c) {
    //if -n is given, set the x after -n to num_disks.
    case 'n':
      sscanf(optarg, "%d", &num_disks);
      break;
    //if -s is given set stack true.
    case 's':
      stack = true;
      break;
    case 'r':
      recur = true;
      break;
    }
  }
  //if no arguments given, return an error.
  if (argc == 1) {
    puts("Error: no  arguments  supplied!");
    return -1;
  }
  //if give number of disks is less than 1, return an error.
  if (num_disks < 1) {
    puts("Error: the number of disks is too low!");
    return -1;
  }
  //if both stack and recur are false meaning no implementation is requested, return an error.
  if (!stack && !recur) {
    puts("Error: no implementation given!");
    return -1;
  }
  //if stack is true meaning stack implementation is requested, print the header and call stack_func.
  if (stack) {
    moves = 0;
    printf("================================\n");
    printf("----------   STACKS   ----------\n");
    printf("================================\n");
    stack_func(num_disks, starting_peg, ending_peg, temp_peg);
    printf("\nNumber of moves: %d\n", moves);
  }
  //if both implementions are called print an enter so it is spaced correctly.
  if (stack && recur) {
    printf("\n");
  }
  //if recur is true meaning recursion implementation is requested, print header and call recusion.
  if (recur) {
    moves = 0;
    printf("================================\n");
    printf("--------   RECURSION   ---------\n");
    printf("================================\n");
    recursion(num_disks, starting_peg, ending_peg, temp_peg);
    printf("\nNumber of moves: %d\n", moves);
  }

  return 0;
}
