/**
 * @ Author: Mo David
 * @ Create Time: 2024-07-19 10:37:54
 * @ Modified time: 2024-07-19 12:55:39
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

} *MODEL;

/**
 * Initializes the model we're going to use.
 * 
 * @param   { Model * }   this  The model to init.
*/
void Model_init() {

  // Allocate the model
  MODEL = calloc(1, sizeof(*MODEL));
  
  // Create a new hashmap
  MODEL->nodes = HashMap_new();
}

/**
 * Adds a new node to the model.
 * The function also returns a reference to the created node.
 * 
 * @param   { char * }  id  The id of the node.
 * @return  { Node * }      The created node.
*/
Node *Model_addNode(char *id) {
  
  // Create a record for the source node
  Record *pSourceRecord = Record_new(id, id);

  // Create a new node
  Node *pSourceNode = Node_new(id, pSourceRecord);

  // Save the node in the hashmap
  HashMap_put(MODEL->nodes, id, pSourceNode);

  // Return the node
  return pSourceNode;
}

/**
 * Adds an adjacency to the model.
 * Creates the nodes if they dont exist.
 * Note that although our graph is undirected, we label our nodes "source" and "target" for the sake of clarity.
 * 
 * @param   { char * }  sourceId  The source id of the adjacency.  
 * @param   { char * }  targetId  The target id of the adjacency.
*/
void Model_addAdj(char *sourceId, char *targetId) {

  // The source and target nodes
  Node *pSourceNode = HashMap_get(MODEL->nodes, sourceId);
  Node *pTargetNode = HashMap_get(MODEL->nodes, targetId);

  // If the source node does not exist in the hashmap yet
  if(pSourceNode == NULL) 
    pSourceNode = Model_addNode(sourceId);

  // If the target node does not exist in the hashmap yet
  if(pTargetNode == NULL)
    pTargetNode = Model_addNode(targetId);

  // Add the target node to the source node as an adjacency
  Node_addAdj(pSourceNode, pTargetNode);
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
  // Also generates the model in memory
  while(File_read(&file, "%s %s", &sourceId, &targetId)) {
    
    
  }

  // Close the file
  File_close(&file);
}

void Model_serializeData() {
  
}

void Model_deserializeData() {
  
}

#endif