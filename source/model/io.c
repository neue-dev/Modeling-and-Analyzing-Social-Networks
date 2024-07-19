/**
 * @ Author: Mo David
 * @ Create Time: 2024-07-19 10:37:54
 * @ Modified time: 2024-07-19 11:47:56
 * @ Description:
 * 
 * Handles serializing and deserializing our data.
 */

#ifndef IO_C
#define IO_C

#include "../io/file.c"

/**
 * Reads the file and converts its data into our model.
 * 
 * @param   { char * }  filepath  The path to the file to read.
*/
void IO_readData(char *filepath) {
  
  // Create a file to the dataset
  File file;
  File_init(&file, filepath);

  
}

void IO_serializeData() {
  
}

void IO_deserializeData() {
  
}

#endif