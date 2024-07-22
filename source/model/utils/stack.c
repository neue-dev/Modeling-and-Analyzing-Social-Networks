/**
 * @ Author: Mo David
 * @ Create Time: 2024-07-22 23:08:40
 * @ Modified time: 2024-07-22 23:12:13
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

#endif

