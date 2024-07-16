/**
 * @ Author: Mo David
 * @ Create Time: 2024-07-16 17:41:28
 * @ Modified time: 2024-07-16 17:58:35
 * @ Description:
 * 
 * Defines a hashmap class.
 */

#ifndef HASHMAP_C
#define HASHMAP_C

#include <stdlib.h>
#include <string.h>

#define HASHMAP_ID_LENGTH (1 << 6)

/**
 * Represents an entry in the hashmap.
*/
typedef struct Entry {
  
  // The id of the entry
  char id[HASHMAP_ID_LENGTH];

  // The data associated with the entry
  void *data;

} Entry;

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
 * @param   { Entry * }   this  The entry to initialize.
 * @param   { char[] }    id    The id of the given entry.
 * @param   { void * }    data  The data stored by the entry.
 * @return  { Entry * }         The initialized version of the entry.
*/
Entry *_Entry_init(Entry *this, char id[], void *data) {
  
  // Save the id and the data
  strcpy(this->id, id);
  this->data = data;

  // Return the new initted entry
  return this;
}

/**
 * Creates a new hashma entry.
 * It initializes the entry with the provided parameters.
 * 
 * @param   { char[] }    id    The id of the given entry.
 * @param   { void * }    data  The data stored by the entry.
 * @return  { Entry * }         The new initialized entry.
*/
Entry *Entry_new(char id[], void *data) {
  return _Entry_init(_Entry_alloc(), id, data);
}

/**
 * Deallocates the memory associated with an instance.
 * Performs additional cleanup if needed too.
 * 
 * @param   { Entry * }   this  The entry to kill.
*/
void Entry_kill(Entry *this) {
  free(this);
}

/**
 * Represents the actual hashmap data structure.
*/
typedef struct HashMap {

  // The contents of the hashmap
  Entry *entries;

  // The number of entries in the hashmap
  unsigned int size;

} HashMap;

#endif