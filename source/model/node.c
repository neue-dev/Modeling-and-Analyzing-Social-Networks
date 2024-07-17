/**
 * @ Author: Mo David
 * @ Create Time: 2024-07-17 10:27:36
 * @ Modified time: 2024-07-17 10:45:51
 * @ Description:
 * 
 * The node class.
 */

#ifndef NODE_C
#define NODE_C

#include "./hashmap.c"

typedef struct Node Node;

/**
 * Represents a node within our graph.
 * Uses a hashmap to store adjacencies.
 */
struct Node {

  // We use these nodes for directional graphs
  HashMap *prevNodes;
  HashMap *nextNodes;

  // We use this instead when creating undirected graphs
  HashMap *adjNodes;

  // The id and data of the node
  char *id;
  void *pData;
};

/**
 * Allocates memory for a node.
 * 
 * @return  { Node * }  The memory for the new node.
 */
Node *Node_alloc() {
  Node *pNode = calloc(1, sizeof(*pNode));
  
  return pNode; 
}

/**
 * Initializes the given node.
 * 
 * @param   { Node * }  this    The node to initialize.
 * @param   { char * }  id      The id of the node.
 * @param   { void * }  pData   The data stored by the node.
 * @return  { Node * }          The initialized node.
 */
Node *Node_init(Node *this, char *id, void *pData) {

  // Save the id and data
  this->id = id;
  this->pData = pData;

  // Init the hashmaps
  this->nextNodes = HashMap_new();
  this->prevNodes = HashMap_new();
  this->adjNodes = HashMap_new();

  return this;
}

/**
 * Creates a new initialized node.
 * 
 * @param   { char * }  id      The id stored by the node.
 * @param   { void * }  pData   The data stored by the node.
 */
Node *Node_new(char *id, void *pData) {
  return Node_init(Node_alloc(), id, pData);
}

/**
 * Frees the memory associated with the node.
 * 
 * @param   { Node * }  this              The node to free.
 * @param   { int }     bShouldFreeData   Whether or not to free the data in the entries and node.
 */
void Node_kill(Node *this, int bShouldFreeData) {
  
  // Kill the hashmaps BUT don't kill the actual nodes in them 
  // That's why we pass a 0 to the method
  HashMap_kill(this->nextNodes, 0);
  HashMap_kill(this->prevNodes, 0);
  HashMap_kill(this->adjNodes, 0);

  // Free the data associated with the node
  if(bShouldFreeData)
    free(this->pData);

  // Free the instance
  free(this);
}

#endif