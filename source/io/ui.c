/**
 * @ Author: Mo David
 * @ Create Time: 2024-07-19 13:45:05
 * @ Modified time: 2024-07-20 15:56:58
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

/**
 * Stores the text output of an input prompt in the provided out variable. 
 * 
 * @param   { char * }  indent  The indent for the input prompt.
 * @param   { char * }  out     The output variable.
*/
void UI_input(char *indent, char *out) {

  // Print the cursor prompt
  UI__();
  UI_indent(indent);

  // Wait for user input
  char input[256];
  scanf("%s", input);
  UI__();

  // Return the output
  strcpy(out, input);
}

/**
 * Grabs a yes or no response from the user.
 * Does not print a prompt for it.
 * 
 * @param   { char * }  indent  The indent to use.
 * @return  { int }             Whether or not the user agreed / disagreed.
*/
int UI_response(char *indent) {

  // Print the prompt indent
  UI__();
  UI_indent(indent);
  
  // Grab the response
  char response;
  scanf(" %c", &response);
  
  // Whether or not the response was a y
  return response == 'Y' || response == 'y';
}

#endif