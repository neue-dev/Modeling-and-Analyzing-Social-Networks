/**
 * @ Author: Mo David
 * @ Create Time: 2024-07-19 10:37:54
 * @ Modified time: 2024-07-19 12:32:34
 * @ Description:
 * 
 * Handles converting the data into the model within memory.
 * Handles serializing and deserializing our data.
 */

#ifndef MODEL_C
#define MODEL_C

#include "../io/file.c"
#include "./hashmap.c"
#include "./record.c"
#include "./node.c"

typedef struct Model Model;

struct Model {

  // Stores the nodes within the model
  HashMap *nodes;

} MODEL;

/**
 * Initializes the model we're going to use.
 * 
 * @param   { Model * }   this  The model to init.
*/
void Model_init(Model *this) {
  
  // Create a new hashmap
  this->nodes = HashMap_new();
}

/**
 * Adds a node to the model.
*/
void Model_addNode() {

}

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
    // Record_new(sourceId, sourceId);
  }

  // Close the file
  File_close(&file);
}

void Model_serializeData() {
  
}

void Model_deserializeData() {
  
}

#endif