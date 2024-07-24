/**
 * @ Author: Mo David
 * @ Create Time: 2024-07-24 22:17:52
 * @ Modified time: 2024-07-24 22:29:47
 * @ Description:
 * 
 * Random number utilities.
 */

#ifndef RAND_C
#define RAND_C

#include <time.h>
#include <stdlib.h> 

// The rand struct
struct Rand {

  // Allows us to init just once
  int isInitted;
  
} Rand;

/**
 * Performs an init
*/
void _Rand_init() {
  
  // If its initted, just skip
  if(Rand.isInitted)
    return;

  // Set the seed
  srand((unsigned) time(NULL));

  // Do the init
  Rand.isInitted = 1;
}

/**
 * Returns a value from 0 to max.
 * 
 * @param   { int }   max   The max value, inclusive.
 * @return  { int }         A random number from 0 - max, inclusive.
*/
int Rand_getMaxxed(int max) {
  
  // Attempt to init
  _Rand_init();

  // Generate the random number
  return rand() % (max + 1);
}

/**
 * Generates a random float from 0 - 1 inclusive.
 * 
 * @return  { double }  The number between 0 and 1.
*/
double Rand_getNormed() {

  // Attempt to init
  _Rand_init();

  // Generate the random float
  return rand() / RAND_MAX;
}

#endif