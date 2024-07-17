/**
 * @ Author: Mo David
 * @ Create Time: 2024-07-16 17:41:28
 * @ Modified time: 2024-07-17 08:57:05
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

/**
 * Represents an entry in the hashmap.
*/
typedef struct Entry {
  
  // The id of the entry
  char key[HASHMAP_KEY_LENGTH + 1];

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
 * @param   { char * }    key   The key of the given entry.
 * @param   { void * }    data  The data stored by the entry.
 * @return  { Entry * }         The initialized version of the entry.
*/
Entry *_Entry_init(Entry *this, char *key, void *data) {
  
  // Save the id and the data
  strncpy(this->key, key, HASHMAP_KEY_LENGTH);
  this->data = data;

  // Return the new initted entry
  return this;
}

/**
 * Creates a new hashma entry.
 * It initializes the entry with the provided parameters.
 * 
 * @param   { char * }    key   The id of the given entry.
 * @param   { void * }    data  The data stored by the entry.
 * @return  { Entry * }         The new initialized entry.
*/
Entry *Entry_new(char *key, void *data) {
  return _Entry_init(_Entry_alloc(), key, data);
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
  Entry **entries;

  // The number of entries in the hashmap
  uint32_t size;

  // The maximum number of entries held by the hashmap
  // This changes when the hashmap resizes
  uint32_t limit;

} HashMap;

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
  int wordSize = sizeof(uint32_t);

  // We're iterating over 4 characters at a time within the key
  // We do this since an unsigned int has 4 bytes
  for(int i = length >> 2; i; i--) {

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
  for(int i = length & 0b11; i; i--) {
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
  this->size = 0;

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
 * Inserts a new element into the hashmap.
 * 
 * @param   { HashMap * }   this  The hashmap to update.
 * @param   { char * }      key   The key of the entry to insert.
 * @param   { void * }      data  The data of the entry to insert.
 */
void HashMap_put(HashMap *this, char *key, void *data) {
  printf("hash: %u", _HashMap_hash(key, strlen(key), 0));
}

#endif