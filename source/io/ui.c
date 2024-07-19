/**
 * @ Author: Mo David
 * @ Create Time: 2024-07-19 13:45:05
 * @ Modified time: 2024-07-19 19:21:33
 * @ Description:
 * 
 * This has a bunch of utilities for displaying stuff on the screen.
 */

#ifndef UI_C
#define UI_C

#include <stdio.h>

struct UI {
  
} UI;

/**
 * Initialize the UI object.
*/
void UI_init() {
  
}

/**
 * Prints the given number of tabs.
 * 
 * @param   { int }   numTabs   The number of tabs to print.
*/
void UI_tab(int numTabs) {
  while(numTabs--) printf("\t");  
}

/**
 * Prints the given number of newlines.
 * 
 * @param   { int }   numNewlines   The number of newlines to print.
*/
void UI_newline(int numNewlines) {
  while(numNewlines--) printf("\n");
}

/**
 * Print a string.
 * 
 * @param   { char * }  string  The string to print out. 
*/
void UI_sPrint(char *string) {
  printf("%s", string);  
}

/**
 * Print a number.
 * 
 * @param   { int }   number  The integer to print out.
*/
void UI_nPrint(int number) {
  printf("%d", number);  
}

/**
 * Print a float.
 * 
 * @param   { float }   number  The floating point to print out.
*/
void UI_fPrint(float number) {
  printf("%f", number);  
}

#endif