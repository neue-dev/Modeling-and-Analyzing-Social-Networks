/**
 * @ Author: Mo David
 * @ Create Time: 2024-07-19 10:37:54
 * @ Modified time: 2024-07-19 21:02:46
 * @ Description:
 * 
 * Handles converting the data into the model within memory.
 * Handles serializing and deserializing our data.
 */

#ifndef MODEL_C
#define MODEL_C

#include "../io/file.c"
#include "./utils/hashmap.c"
#include "./record.c"
#include "./node.c"

struct Model {

  // Stores the nodes within the model
  HashMap *nodes;

  // We use this for garbage collection later on
  Node **nodePointers;
  int nodeCount;

} Model;

/**
 * Initializes the model we're going to use.
 * 
 * @param   { Model * }   this  The model to init.
*/
void Model_init() {
  
  // Create a new hashmap
  Model.nodes = HashMap_new();

  // No nodes yet
  Model.nodeCount = 0;
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
  Record *pRecord = Record_new(id, id);

  // Create a new node
  Node *pNode = Node_new(id, pRecord);

  // Save the node in the hashmap
  HashMap_put(Model.nodes, id, pNode);

  // Add the reference to the list of node pointers
  Model.nodePointers[Model.nodeCount++] = pNode;

  // Return the node
  return pNode;
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
  Node *pSourceNode = HashMap_get(Model.nodes, sourceId);
  Node *pTargetNode = HashMap_get(Model.nodes, targetId);

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
 * Clears the contents of the model.
 * Makes sure to perform proper garbage collection.
*/
void Model_clearData() {
  
  // Kill the hashmap first
  // We don't delete the data inside because we clean that up ourselves
  HashMap_kill(Model.nodes, 0);

  // We kill the associated data with each of the nodes
  while(Model.nodeCount--)
    Node_kill(Model.nodePointers[Model.nodeCount], 1);

  // Free the nodePointers
  free(Model.nodePointers);

  // Create a new hashmap
  // Reset node count (although that's a bit redudant)
  Model.nodes = HashMap_new();
  Model.nodeCount = 0;
}

/**
 * Reads the file and converts its data into our model.
 * 
 * @param   { char * }  filepath  The path to the file to read.
*/
void Model_loadData(char *filepath) {
  
  // Create a file to the dataset
  File file;
  File_init(&file, filepath);
  File_open(&file, "r");

  // Temporary variables for holding read data
  char sourceId[32];
  char targetId[32];

  // Metadata
  int nodeCount;
  int edgeCount;

  // The first line only contains metadata
  File_read(&file, "%d %d", &nodeCount, &edgeCount);

  // Init the node pointer array
  Model.nodePointers = calloc(nodeCount, sizeof(Node *));

  // Read the file contents
  // Also generates the model in memory
  while(File_read(&file, "%s %s", &sourceId, &targetId))
    Model_addAdj(sourceId, targetId);

  // Close the file
  File_close(&file);
}

// void Model_

void Model_serializeData() {
  
}

void Model_deserializeData() {
  
}

#endif