/**
 * @ Author: Mo David
 * @ Create Time: 2024-07-19 13:47:39
 * @ Modified time: 2024-07-19 14:03:29
 * @ Description:
 * 
 * A queue data structure which comes in handy for traversing our data.
 */

#ifndef QUEUE_C
#define QUEUE_C

#include "./entry.c"
#include <stdint.h>

typedef struct Queue Queue;

struct Queue {

  // Refers to the head and tail of the queue
  // The tail is needed for appending to the queue
  Entry *pHead;
  Entry *pTail;

  // How many items are in the queue
  uint32_t count;
};

/**
 * Allocates memory for a new queue.
 * 
 * @return  { Queue * }   The new queue.
*/
Queue *_Queue_alloc() {
  Queue *pQueue = calloc(1, sizeof(*pQueue));

  return pQueue;
}

/**
 * Initializes the given queue.
 * 
 * @param   { Queue * }   this  The queue to initialize.
 * @return  { Queue * }         The initted queue.
*/
Queue *_Queue_init(Queue *this) {
  
  // Set the pointers to null
  this->pHead = NULL;
  this->pTail = NULL;

  // Set the count to 0
  this->count = 0;
}

/**
 * Creates a new initted queue object.
 * 
 * @return  { Queue * }   A new initted queue object.
*/
Queue *Queue_new() {
  return _Queue_init(_Queue_alloc());
}

/**
 * Frees the memory for a queue object.
 * 
 * @param   { Queue * }   this              The queue object to free.
 * @param   { int }       bShouldFreeData   Whether or not to free the data in the entries.
*/
void Queue_kill(Queue *this, int bShouldFreeData) {

  // Get the head
  Entry *pHead = this->pHead;

  // While not null
  while(pHead != NULL) {
    
    // Keep track of current
    Entry *toFree = pHead;

    // Grab the next
    pHead = pHead->pNext;

    // Free current entry
    Entry_kill(toFree, bShouldFreeData);
  }

  // Free the queue object itself
  free(this);
}

#endif