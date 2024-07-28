/**
 * @ Author: Mo David
 * @ Create Time: 2024-07-19 13:49:08
 * @ Modified time: 2024-07-29 02:50:05
 * @ Description:
 * 
 * An entry we use for both hashmaps and queues.
 */

#ifndef ENTRY_C
#define ENTRY_C

#include <stdlib.h>

#define ENTRY_KEY_LENGTH (1 << 6)

typedef struct Entry Entry;

/**
 * Represents an entry in the hashmap.
*/
struct Entry {
  
  // The id of the entry
  char key[ENTRY_KEY_LENGTH + 1];

  // The data associated with the entry
  void *pData;

  // The next entry in the linked list
  // Used to handle collisions in the hashmap
  Entry *pNext;

  // The prev entry in the linked list
  // Useful for the stack
  Entry *pPrev;
};

/**
 * The entry interface.
 */
Entry *_Entry_alloc();
Entry *_Entry_init(Entry *this, char *key, void *pData);
Entry *Entry_new(char *key, void *pData);
void Entry_kill(Entry *this, int bShouldFreeData);

void Entry_chain(Entry *pPrev, Entry *pNext);

/**
 * Allocates space for a new entry instance.
 * 
 * @return  { Entry * }   A pointer to the allocated space.
*/
Entry *_Entry_alloc() {
  Entry *pEntry = calloc(1, sizeof(*pEntry));

  return pEntry;
}

/**
 * Initializes a given entry instance with the provided values.
 * 
 * @param   { Entry * }   this    The entry to initialize.
 * @param   { char * }    key     The key of the given entry.
 * @param   { void * }    pData   The data stored by the entry.
 * @return  { Entry * }           The initialized version of the entry.
*/
Entry *_Entry_init(Entry *this, char *key, void *pData) {
  
  // Save the id and the data
  strncpy(this->key, key, ENTRY_KEY_LENGTH);
  this->pData = pData;

  // Set the next to null by default
  this->pNext = NULL;
  this->pPrev = NULL;

  // Return the new initted entry
  return this;
}

/**
 * Creates a new hashma entry.
 * It initializes the entry with the provided parameters.
 * 
 * @param   { char * }    key     The id of the given entry.
 * @param   { void * }    pData   The data stored by the entry.
 * @return  { Entry * }           The new initialized entry.
*/
Entry *Entry_new(char *key, void *pData) {
  return _Entry_init(_Entry_alloc(), key, pData);
}

/**
 * Deallocates the memory associated with an instance.
 * Performs additional cleanup if needed too.
 * 
 * @param   { Entry * }   this              The entry to kill.
 * @param   { int }       bShouldFreeData   Whether or not to free its associated data.
*/
void Entry_kill(Entry *this, int bShouldFreeData) {
  
  // Free its associated data
  if(bShouldFreeData)
    free(this->pData);

  // Free the instance itself
  free(this);
}

/**
 * Chains the given next entry unto the previous one.
 * 
 * @param   { Entry * }   pPrev   The previous entry.
 * @param   { Entry * }   pNext   The next entry.
 */
void Entry_chain(Entry *pPrev, Entry *pNext) {
  
  // Check if valid assignment
  if(pPrev != NULL)
    pPrev->pNext = pNext;

  // Check if valid assignment
  if(pNext != NULL)
    pNext->pPrev = pPrev;
}

#endif