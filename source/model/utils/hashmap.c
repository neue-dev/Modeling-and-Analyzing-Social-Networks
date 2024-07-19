/**
 * @ Author: Mo David
 * @ Create Time: 2024-07-16 17:41:28
 * @ Modified time: 2024-07-19 13:51:57
 * @ Description:
 * 
 * Defines a hashmap class.
 */

#ifndef HASHMAP_C
#define HASHMAP_C

#include "./entry.c"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define HASHMAP_HASH_SEED (0)
#define HASHMAP_MAX_LOAD (1.1)
#define HASHMAP_MAX_SIZE (1 << 30)

typedef struct HashMap HashMap;

/**
 * Represents the actual hashmap data structure.
*/
struct HashMap {

  // The contents of the hashmap
  Entry **entries;

  // The number of entries in the hashmap
  uint32_t count;

  // The number of occupied slots in the hashmap
  // This is different from the number of entries 
  // ...because slots with collisions produce linked lists
  uint32_t slots; 

  // The maximum number of entries held by the hashmap
  // This changes when the hashmap resizes
  uint32_t limit;

};

/**
 * The hashmap interface.
 */
HashMap *HashMap_alloc();
HashMap *HashMap_init(HashMap *this);
HashMap *HashMap_new();
void HashMap_kill(HashMap *this, int bShouldFreeData);

void _HashMap_attemptResize(HashMap *this);
int _HashMap_put(HashMap *this, Entry *pEntry);
int HashMap_put(HashMap *this, char *key, void *pData);
void *HashMap_get(HashMap *this, char *key);

/**
 * This just jumbles of the value of k and is an arbitrary formula.
 * It's the one used by murmur hash, and is lifted from Wikipedia.
 * 
 * @param   { uint32_t }  k   Some arbitrary integer.
 * @return  { uint32_t }      A jumbled version of that integer.  
*/
static inline uint32_t _HashMap_scramble(uint32_t k) {

  k *= 0xcc9e2d51;
  k = (k << 15) | (k >> 17);
  k *= 0x1b873593;
  
  return k;
}

/**
 * This is an implementation of "murmur hash".
 * The implementation is based on the Wikipedia page of the algorithm.
 * 
 * @param   { char * }    key     The key we wish to hash.
 * @param   { uint32_t }  length  The length of the key to hash.
 * @param   { uint32_t }  seed    The seed to use for hashing.
 * @return  { uint32_t }          The resulting value of the hash.
*/
static inline uint32_t _HashMap_hash(char *key, uint32_t length, uint32_t seed) {
  
  uint32_t h = seed;
  uint32_t k;
  short wordSize = sizeof(uint32_t);

  // We're iterating over 4 characters at a time within the key
  // We do this since an unsigned int has 4 bytes
  for(uint32_t i = length >> 2; i; i--) {

    // We copy 4 bytes of the key unto the location of k
    memcpy(&k, key, wordSize);

    // These are pretty much just arbitrary transformations we perform on the seed
    h ^= _HashMap_scramble(k);
    h = (h << 13) | (h >> 19);
    h = (h << 2) + h + 0xe6546b64;

    // We iterate key for the next iteration
    key += wordSize;
  }

  // In case the key has extra characters after the last batch of four
  // We deal with those and use them to further transform h
  // length & 0b11 makes it clear we're getting the last two bits of the number
  k = 0;
  for(short i = length & 0b11; i; i--) {
    k <<= 8;
    k |= key[i - 1];
  }

  // The final steps of the algorithm
  h ^= _HashMap_scramble(k);
	h ^= length;
	h ^= h >> 16;
	h *= 0x85ebca6b;
	h ^= h >> 13;
	h *= 0xc2b2ae35;
	h ^= h >> 16;

  // Return the generated hash
	return h;
}

/**
 * Resizes the hashmap when space has run out.
 * 
 * @param   { HashMap * }   this  The hashmap to resize.
 */
static inline void _HashMap_resize(HashMap *this) {

  // Get a reference to the old entries
  Entry **pOldEntries = this->entries;
  
  // Set the slots to 0 
  // We have to recompute this value
  this->slots = 0;
  
  // Compute the new size
  // The way we have this implemented means the limit will always be 2^n - 1
  this->limit <<= 1;
  this->limit += 1;

  // Reallocate the entries variable
  this->entries = calloc(this->limit, sizeof(Entry *));

  // Pointers we use to traverse our arrays
  Entry *pOldEntry;
  Entry *pNewEntry;

  // Remap each of the old entries
  for(uint32_t i = 0; i < this->limit >> 1; i++) {
    
    // Grab the current entry
    pOldEntry = pOldEntries[i];

    // Skip the null entries
    if(pOldEntry == NULL)
      continue;

    // Insert the entries into the new space
    do {

      // A temp pointer 
      Entry *pPrevEntry;

      // Insert the old entry
      _HashMap_put(this, pOldEntry);

      // Grab the entry that was adjacent to pOldEntry in old hashmap
      pPrevEntry = pOldEntry;
      pOldEntry = pPrevEntry->pNext;

      // Set the next of the prev entry to null, since it's been remapped
      Entry_chain(pPrevEntry, NULL);

    } while(pOldEntry != NULL);
  }

  // Get rid of the old array of pointers
  free(pOldEntries);
}

/**
 * Allocates space for a new hashmap.
 * 
 * @return  { HashMap * }   The pointer to the allocated space.
 */
HashMap *_HashMap_alloc() {
  HashMap *pHashMap = calloc(1, sizeof(*pHashMap));

  return pHashMap;
}

/**
 * Initializes the provided hashmap instance.
 * 
 * @param   { HashMap * }   this  The pointer to the hashmap to initialize.
 * @return  { HashMap * }         The pointer to the initialized hashmap.
 */
HashMap *_HashMap_init(HashMap *this) {

  uint32_t initialLimit = (1 << 8) - 1;

  // We start with 16 slots
  this->limit = initialLimit;
  this->slots = 0;
  this->count = 0;

  // Init the entrie pointer array
  this->entries = calloc(initialLimit, sizeof(Entry *));
  
  return this;
}

/**
 * Creates a new hashmap initialized with the given values.
 * 
 * @return  { HashMap * }   A new hashmap that's been initted.
 */
HashMap *HashMap_new() {
  return _HashMap_init(_HashMap_alloc());
}

/**
 * Deallocates the memory associated with a hashmap.
 * 
 * @param   { HashMap * }   this              The memory to deallocate.
 * @param   { int }         bShouldFreeData   Whether or not to free the data in the entries.
 */
void HashMap_kill(HashMap *this, int bShouldFreeData) {

  // Make sure we free all the entires too
  for(uint32_t i = 0; i < this->limit; i++) {
    
    Entry *pEntry = this->entries[i];
    Entry *pNext = NULL;

    // If there's something at this slot
    while(pEntry != NULL) {

      // Grab the pointer to next entry
      pNext = pEntry->pNext;

      // Free the current entry
      Entry_kill(pEntry, bShouldFreeData);
      
      // Go to next entry
      pEntry = pNext;
    }
  }

  // Free the main memory
  free(this);
}

/**
 * Attempts to resize the hashmap IF needed.
 * If none of the conditions are satisfied, it does nothing.
 * 
 * @param   { HashMap * }   this  The hashmap to resize.
 */
void _HashMap_attemptResize(HashMap *this) {
  
  // Compute the load factor
  // It's just the average length of our linked lists
  double loadFactor = ((double) this->count) / ((double) this->slots);

  // We can't keep resizing indefinitely because we'll run out of memory smh
  if(this->limit > HASHMAP_MAX_SIZE)
    return;

  // If the max load is reached, we attempt a resize
  // Even if the entire hashmap fills up, collisons will just start happening
  // That will eventually trigger the loadFactor to go above the max load allowed
  // Also, we only resize if the hashmap is at least half full
  if(loadFactor > HASHMAP_MAX_LOAD && this->slots >= this->limit * 0.5)
    _HashMap_resize(this);
}

/**
 * Inserts a new element into the hashmap.
 * However, this takes as input the actual entry object.
 * Unlike HashMap_put, _HashMap_put does not instantiate the entry object within its body.
 * NOTE THAT THIS DOES NOT INCREMENT THE COUNT OF THE HASHMAP.
 * Fails on duplicate keys.
 * 
 * @param   { HashMap * }   this    The hashmap to update.
 * @param   { Entry * }     pEntry  The entry to put.
 * @return  { int }                 Whether or not the insertion was successful.
 */
int _HashMap_put(HashMap *this, Entry *pEntry) {
  
  // Grab the key of the entry
  char *key = pEntry->key;
  uint32_t hash = _HashMap_hash(key, strlen(key), HASHMAP_HASH_SEED);
  uint32_t slot = hash % this->limit;

  // The slot we wish to insert the entry
  Entry *pSlot = this->entries[slot];

  // There's nothing there
  if(pSlot == NULL) {
    
    // Insert the entry into the slot
    this->entries[slot] = pEntry;
    this->slots++;

    // Return
    _HashMap_attemptResize(this);
    return 1;
  }

  // If there is a collision, however...
  while(1) {
    
    // Check for duplicate key
    if(!strcmp(pSlot->key, pEntry->key))
      return 0;

    // Break out of loop if null
    if(pSlot->pNext == NULL)
      break;

    // Grab the next entry in the linked list
    pSlot = pSlot->pNext;
  }
  
  // Finally, chain the current entry to the last one
  Entry_chain(pSlot, pEntry);

  // Success
  _HashMap_attemptResize(this);
  return 1;
}

/**
 * Inserts a new element into the hashmap.
 * 
 * @param   { HashMap * }   this    The hashmap to update.
 * @param   { char * }      key     The key of the entry to insert.
 * @param   { void * }      pData   The data of the entry to insert.
 * @return  { int }                 Indicates whether or not the operation was successful.
 */
int HashMap_put(HashMap *this, char *key, void *pData) {

  // Grab the slot of the entry
  uint32_t hash = _HashMap_hash(key, strlen(key), HASHMAP_HASH_SEED);
  uint32_t slot = hash % this->limit;

  // The slot we wish to insert the entry
  Entry *pSlot = this->entries[slot];

  // Create the entry too
  Entry *pEntry = Entry_new(key, pData);

  // There's nothing there
  if(pSlot == NULL) {
    
    // Insert the entry into the slot
    this->entries[slot] = pEntry;
    this->slots++;
    this->count++;

    // Return
    _HashMap_attemptResize(this);
    return 1;
  }

  // If there is a collision, however...
  while(1) {
    
    // Check for duplicate key
    if(!strcmp(pSlot->key, key)) {
      
      // Free the created entry
      free(pEntry);

      // Return failure
      return 0;
    }

    // Get out of loop if NULL
    if(pSlot->pNext == NULL)
      break;
      
    // Grab the next entry in the linked list
    pSlot = pSlot->pNext;
  }
  
  // Finally, chain the current entry to the last one
  // Increment the count too
  Entry_chain(pSlot, pEntry);
  this->count++;

  // Success
  _HashMap_attemptResize(this);
  return 1;
}

/**
 * Returns the data stored at the given key for the hashmap.
 * 
 * @param   { HashMap * }   this  The hashmap to read.
 * @param   { char * }      key   The key of the data.
 * @return  { void * }            A pointer to the data stored there.
 */
void *HashMap_get(HashMap *this, char *key) {
  
  // Grab the slot of the entry
  uint32_t hash = _HashMap_hash(key, strlen(key), HASHMAP_HASH_SEED);
  uint32_t slot = hash % this->limit;

  // The slot we wish to insert the entry
  Entry *pSlot = this->entries[slot];

  // The key was not found
  if(pSlot == NULL)
    return NULL;
 
  // Traverse the linked list
  while(strcmp(pSlot->key, key)) {

    // Go to next in list
    pSlot = pSlot->pNext;

    // The key was not found
    if(pSlot == NULL)
      return NULL;
  }

  // Return the associated data
  return pSlot->pData;
}

#endif