/**
 * @ Author: Mo David
 * @ Create Time: 2024-07-16 17:41:28
 * @ Modified time: 2024-07-17 09:39:45
 * @ Description:
 * 
 * Defines a hashmap class.
 */

#ifndef HASHMAP_C
#define HASHMAP_C

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// ! remove
#include <stdio.h>

#define HASHMAP_KEY_LENGTH (1 << 6)
#define HASHMAP_HASH_SEED (0)

typedef struct Entry Entry;
typedef struct HashMap HashMap;

/**
 * Represents an entry in the hashmap.
*/
struct Entry {
  
  // The id of the entry
  char key[HASHMAP_KEY_LENGTH + 1];

  // The data associated with the entry
  void *pData;

  // The next entry in the linked list
  // Used to handle collisions in the hashmap
  Entry *pNext;

};

/**
 * The entry interface.
 */
Entry *_Entry_alloc();
Entry *_Entry_init(Entry *this, char *key, void *pData);
Entry *_Entry_new(char *key, void *pData);
void _Entry_kill(Entry *this);

void _Entry_chain(Entry *this, Entry *pNext);

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
  strncpy(this->key, key, HASHMAP_KEY_LENGTH);
  this->pData = pData;

  // Set the next to null by default
  this->pNext = NULL;

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
Entry *_Entry_new(char *key, void *pData) {
  return _Entry_init(_Entry_alloc(), key, pData);
}

/**
 * Deallocates the memory associated with an instance.
 * Performs additional cleanup if needed too.
 * 
 * @param   { Entry * }   this  The entry to kill.
*/
void _Entry_kill(Entry *this) {
  free(this);
}

/**
 * Chains the given next entry unto the current one.
 * 
 * @param   { Entry * }   this    The entry to modify.
 * @param   { Entry * }   pNext   The entry to chain.
 */
void _Entry_chain(Entry *this, Entry *pNext) {
  this->pNext = pNext;
}

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
void HashMap_kill(HashMap *this);

int _HashMap_put(HashMap *this, Entry *pEntry);
int HashMap_put(HashMap *this, char *key, void *pData);

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
      _Entry_chain(pPrevEntry, NULL);

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

  uint32_t initialLimit = (1 << 4) - 1;

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
 * @param   { HashMap * }   this  The memory to deallocate.
 */
void HashMap_kill(HashMap *this) {
  free(this);
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
    return 1;
  }

  // If there is a collision, however...
  while(pSlot->pNext != NULL) {
    
    // Check for duplicate key
    if(!strcmp(pSlot->key, pEntry->key))
      return 0;

    // Grab the next entry in the linked list
    pSlot = pSlot->pNext;
  }
  
  // Finally, chain the current entry to the last one
  _Entry_chain(pSlot, pEntry);

  // Success
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

  // Grab the key of the entry
  uint32_t hash = _HashMap_hash(key, strlen(key), HASHMAP_HASH_SEED);
  uint32_t slot = hash % this->limit;

  // The slot we wish to insert the entry
  Entry *pSlot = this->entries[slot];

  // Create the entry too
  Entry *pEntry = _Entry_new(key, pData);

  // There's nothing there
  if(pSlot == NULL) {
    
    // Insert the entry into the slot
    this->entries[slot] = pEntry;
    this->slots++;
    this->count++;

    // Return
    return 1;
  }

  // If there is a collision, however...
  while(pSlot->pNext != NULL) {
    
    // Check for duplicate key
    if(!strcmp(pSlot->key, pEntry->key))
      return 0;

    // Grab the next entry in the linked list
    pSlot = pSlot->pNext;
  }
  
  // Finally, chain the current entry to the last one
  // Increment the count too
  _Entry_chain(pSlot, pEntry);
  this->count++;

  // Success
  return 1;
}

#endif