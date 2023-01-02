#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

//Creates the stack.
Stack *stack_create(int capacity, char name) {
  Stack *s = (struct Stack *)malloc(sizeof(Stack));
  //checks if stack is true.
  if (!s) {
    return 0;
  }
  //if capcity is set to less 1 it will round the capacity up to 1.
  if (capacity < 1) {
    capacity = 1;
  }
  //sets capacity to what was given and set the pointer to 1 and allocates the right memory for the size of the stack.
  s->capacity = capacity;
  s->top = 0;
  s->name = name;
  s->items = (int *)malloc(sizeof(int) * capacity);
  //if items is empty than it will return 0
  if (!s->items) {
    return 0;
  }
  //returns the stack.
  return s;
}

//frees up the memerory and clears the stack, preventing memory leaks.
void stack_delete(Stack *s) {
  free(s->items);
  s->items = NULL;
  free(s);
  return;
}

//pops the top position of the stack and sets the pointer one position back.
int stack_pop(Stack *s) {
  //check if stack is true.
  if (!s) {
    return -1;
  }
  //if stack has at least one item it will pop an item off the stack.
  if (s->top > 0) {
    s->top -= 1;
    return s->items[s->top];
  }
  //if the stack is empt it will return -1.
  else {
    return -1;
  }
}

//adds the item given to the stack and increases the pointer.
void stack_push(Stack *s, int item) {
  // checks if stack is true.
  if (!s) {
    return;
  }
  //checks if stack is full, when it is it wil increase the memory allocated.
  if (s->top == s->capacity) {
    s->capacity *= 2;
    s->items = (int *)realloc(s->items, s->capacity * sizeof(int));
  }
  //if there is an item given than it will add it to the stack and increase the pointer.
  if (s->items) {
    s->items[s->top] = item;
    s->top += 1;
  }
  return;
}

//checks if stack is empty, returns true if empty.
bool stack_empty(Stack *s) {
  return s->top == 0;
}

//returns the item that is on top of the stack but doesn't edit the stack in any way.
int stack_peek(Stack *s) {
  //checks if stack is true.
  if (!s) {
    return -1;
  }
  //if stack has atleast one item it will return that item.
  if (s->top > 0) {
    return s->items[s->top - 1];
  }
  //if stack is empty it will return -1
  else {
    return -1;
  }
}
