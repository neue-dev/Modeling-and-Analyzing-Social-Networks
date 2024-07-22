/**
 * @ Author: Mo David
 * @ Create Time: 2024-07-22 23:08:40
 * @ Modified time: 2024-07-22 23:22:12
 * @ Description:
 * 
 * Our stack implementation.
 * Useful for reversing queue order.
 */

#ifndef STACK_C
#define STACK_C

#include "./entry.c"
#include <stdint.h>

typedef struct Stack Stack;

struct Stack {

  // A pointer to the top of the stack
  Entry *pTop;

  // How many items are in the queue
  uint32_t count;
};

/**
 * Allocates memory for a new stack instance.
 * 
 * @return  { Stack * }   A pointer to the allocated memory.
*/
Stack *_Stack_alloc() {
  Stack *pStack = calloc(1, sizeof(*pStack));

  return pStack;
}

/**
 * Initializes the given stack instance.
 * 
 * @param   { Stack * }   this  The stack to initialize.
 * @return  { Stack * }         A pointer to the initted stack.
*/
Stack *_Stack_init(Stack *this) {
  this->pTop = NULL;
  this->count = 0;

  return this;
}

/**
 * Creates a new initted stack.
 * 
 * @return  { Stack * }   A new initted stack.
*/
Stack *Stack_new() {
  return _Stack_init(_Stack_alloc());
}

/**
 * Deallocates the memory for that stack.
 * 
 * @param   { Stack * }   this              The stack to deallocate.
 * @param   { int }       bShouldFreeData   Whether or not to free the entry data.
*/
void Stack_kill(Stack *this, int bShouldFreeData) {
  
  // Grab the top of the stack
  Entry *pEntry = this->pTop;
  
  // While we have stuff in the stack
  while(pEntry != NULL) {

    // Save a reference to the current top
    Entry *toFree = pEntry;

    // Get the prev entry (the one below the top)
    pEntry = pEntry->pPrev;

    // Kill the entry
    Entry_kill(toFree, bShouldFreeData);
  }

  // Finally, free the stack
  free(this);
}

#endif

