/**
 * @ Author: Mo David
 * @ Create Time: 2024-07-19 13:45:05
 * @ Modified time: 2024-07-19 19:50:30
 * @ Description:
 * 
 * This has a bunch of utilities for displaying stuff on the screen.
 */

#ifndef UI_C
#define UI_C

#include <stdio.h>
#include <string.h>

#define UI_INDENT_SIZE 4

struct UI {
  
  // The size of each of the indents, in spaces
  int indentSize;

} UI;

/**
 * The UI interface.
*/
void UI_init();
void UI_clear();
void UI_indent(char *string);

void UI__();
void UI_s(char *string);
void UI_n(int number);
void UI_f(float number);

/**
 * Initialize the UI object.
*/
void UI_init() {

  // Set the indent size
  UI.indentSize = UI_INDENT_SIZE;

  // Clear the console upon init
  UI_clear();
}

/**
 * Clears the current console.
*/
void UI_clear() {
  printf("\e[H\e[2J\e[3J");
}

/**
 * Use the given string as a lead indent.
 * 
 * @param   { char * }  string  The string to use for the indent.
*/
void UI_indent(char *string) {

  // Create a temp var
  char indent[UI.indentSize + 1];

  // Copy the first four chars of the string
  strncpy(indent, string, UI.indentSize);

  // Print the indent
  printf("%-*s", UI.indentSize, indent);
}

/**
 * Prints the a newline.
*/
void UI__() {
  printf("\n");
}

/**
 * Print a string.
 * 
 * @param   { char * }  string  The string to print out. 
*/
void UI_s(char *string) {
  printf("%s", string);  
}

/**
 * Print a number.
 * 
 * @param   { int }   number  The integer to print out.
*/
void UI_n(int number) {
  printf("%d", number);  
}

/**
 * Print a float.
 * 
 * @param   { float }   number  The floating point to print out.
*/
void UI_f(float number) {
  printf("%f", number);  
}

#endif