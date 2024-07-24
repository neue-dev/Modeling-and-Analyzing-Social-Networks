/**
 * @ Author: Mo David
 * @ Create Time: 2024-07-17 10:27:36
 * @ Modified time: 2024-07-24 22:16:33
 * @ Description:
 * 
 * The node class.
 */

#ifndef NODE_C
#define NODE_C

#include "./structs/hashmap.c"

#define NODE_ID_LENGTH (1 << 6)

typedef struct Node Node;

/**
 * Represents a node within our graph.
 * Uses a hashmap to store adjacencies.
 */
struct Node {

  // We use these variables for pathfinding
  Node *pPrevNode;
  Node *pNextNode;

  // We use this instead when creating undirected graphs
  HashMap *adjNodes;

  // The id and data of the node
  char id[NODE_ID_LENGTH + 1];
  void *pData;
};

/**
 * Allocates memory for a node.
 * 
 * @return  { Node * }  The memory for the new node.
 */
Node *_Node_alloc() {
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
Node *_Node_init(Node *this, char *id, void *pData) {

  // Save the id and data
  strncpy(this->id, id, NODE_ID_LENGTH);
  this->pData = pData;

  // Set the pointer to null
  this->pPrevNode = NULL;
  this->pNextNode = NULL;

  // Init the hashmaps
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
  return _Node_init(_Node_alloc(), id, pData);
}

/**
 * Frees the memory associated with the node.
 * 
 * @param   { Node * }  this              The node to free.
 * @param   { int }     bShouldFreeData   Whether or not to free the data in the entries and node.
 */
void Node_kill(Node *this, int bShouldFreeData) {
  
  // Kill the hashmap BUT don't kill the actual nodes in it 
  // That's why we pass a 0 to the method
  HashMap_kill(this->adjNodes, 0);

  // Free the data associated with the node
  if(bShouldFreeData)
    free(this->pData);

  // Free the instance
  free(this);
}

/**
 * Sets the next node of the given node.
 * 
 * @param   { Node * }  this    The node to modify.
 * @param   { Node * }  pNext   A pointer to the next node.
*/
void Node_setNext(Node *this, Node *pNext) {
  this->pNextNode = pNext;
}

/**
 * Sets the prev node of the given node.
 * 
 * @param   { Node * }  this    The node to modify.
 * @param   { Node * }  pPrev   A pointer to the prev node.
*/
void Node_setPrev(Node *this, Node *pPrev) {
  this->pPrevNode = pPrev;
}

/**
 * Adds an adjacent node to the given node.
 * This method is used exclusively when addNext() and addPrev() are not.
 * In other words, we either use addNext() and addPrev() OR addAdj() based on whether we're constructing a un/directed graph.
 * 
 * @param   { Node * }  this  The first node in the adjacency.
 * @param   { Node * }  pAdj  The second node in the adjacency.
*/
void Node_addAdj(Node *this, Node *pAdj) {
  HashMap_put(this->adjNodes, pAdj->id, pAdj);
  HashMap_put(pAdj->adjNodes, this->id, this);
}

#endif