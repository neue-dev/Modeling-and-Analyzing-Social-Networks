/**
 * @ Author: Mo David
 * @ Create Time: 2024-07-19 10:37:54
 * @ Modified time: 2024-07-19 11:59:54
 * @ Description:
 * 
 * Handles converting the data into the model within memory.
 * Handles serializing and deserializing our data.
 */

#ifndef MODEL_C
#define MODEL_C

#include "../io/file.c"

typedef struct Model Model;

/**
 * Reads the file and converts its data into our model.
 * 
 * @param   { char * }  filepath  The path to the file to read.
*/
void Model_readData(char *filepath) {
  
  // Create a file to the dataset
  File file;
  File_init(&file, filepath);
  File_open(&file, "r");

  // Temporary variables for holding read data
  char sourceId[32];
  char targetId[32];

  // Read the file contents
  while(File_read(&file, "%s %s", &sourceId, &targetId)) {
    // printf("source: %s, target: %s\n", sourceId, targetId);
  }

  // Close the file
  File_close(&file);
}

void Model_serializeData() {
  
}

void Model_deserializeData() {
  
}

#endif