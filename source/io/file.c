/**
 * @ Author: Mo David
 * @ Create Time: 2024-07-19 10:14:59
 * @ Modified time: 2024-07-19 11:57:36
 * @ Description:
 */

#ifndef FILE_C
#define FILE_C

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

typedef struct File File;

struct File {

  // The path to the file itself
  char filepath[256];

  // The pointer to the file
  FILE *pFile; 
};

/**
 * Initiailizes the file with the given values.
 * 
 * @param   { File * }  this      The file object to init.
 * @param   { char * }  filepath  The path to the file to link.
*/
void File_init(File *this, char *filepath) {
  strcpy(this->filepath, filepath);
}

/**
 * Opens the file with the given mode.
 * 
 * @param   { File * }  this  The file to open.
 * @param   { char * }  mode  The mode to open the file.
 * @return  { int }           A boolean; whether or not the file was opened.
*/
int File_open(File *this, char *mode) {

  // Open the file first
  this->pFile = fopen(this->filepath, mode);

  // Failure to open file
  if(this->pFile == NULL)
    return 0;

  // File was opened
  return 1;
}

/**
 * Read the file with the given format.
 * 
 * @param   { File * }    this    The file object to use.
 * @param   { char * }    format  The format of the tokens to read.
 * @param   { void *[] }  ...     The pointers to the memory to save the tokens in.    
 * @return  { int }               How many tokens were consumed.
*/
int File_read(File *this, char *format, ...) {

  // Pointer to the argument list
  va_list va_ptr;
  va_start(va_ptr, format);

  // If the file pointer is NULL
  if(this->pFile == NULL)
    return 0;

  // Call fscanf with our args
  int result = vfscanf(this->pFile, format, va_ptr);

  // Return 0 for EOF
  if(result == EOF) 
    return 0;

  // Otherwise, return result
  return result;
}

/**
 * Close the associated file.
 * 
 * @param   { File * }  this  The file to close.
*/
void File_close(File *this) {
  fclose(this->pFile);
}

#endif