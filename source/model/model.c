/**
 * @ Author: Mo David
 * @ Create Time: 2024-07-19 10:37:54
 * @ Modified time: 2024-07-25 13:36:05
 * @ Description:
 * 
 * Handles converting the data into the model within memory.
 * Handles serializing and deserializing our data.
 */

#ifndef MODEL_C
#define MODEL_C

#include "../utils/bmp.c"
#include "../utils/color.c"
#include "../utils/rand.c"
#include "../utils/point.c"

#include "./structs/hashmap.c"
#include "./structs/stack.c"
#include "./structs/queue.c"

#include "../io/file.c"
#include "./record.c"
#include "./node.c"

#define MODEL_EMPTY "no model"
struct Model {

  // The path to the active dataset
  char activeDataset[256];

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
 * "Generates" the connection between two nodes.
 * By this, we mean that it initializes the "prev" variables of the nodes to the represent a connection between the nodes.
 * Returns whether or not a connection between the two nodes was found.
 * 
 * @param   { Node * }  pSourceNode   The source node of the connection.
 * @param   { Node * }  pTargetNode   The target node of the connection.
 * @return  { int }                   Whether or not a connection could be found.
*/
int Model_generateConnection(Node *pSourceNode, Node *pTargetNode) {

  // We proceed to traverse the dataset if both nodes were fine
  Queue *nodeQueue = Queue_new();
  HashMap *visited = HashMap_new();

  // A constant we use to know something has been visited
  char *VISITED = "VISITED";
  int success = 0;

  // Push the source node unto the queue
  Queue_add(nodeQueue, pSourceNode);

  // Clear the prev of the node in case it was set in a previous traversal
  Node_setPrev(pSourceNode, NULL);

  // While the queue isn't empty
  while(Queue_getCount(nodeQueue) && !success) {

    // Grab the head and its details
    Node *pHead = Queue_remove(nodeQueue);
    HashMap *adjNodes = pHead->adjNodes;
    
    // Grab the keys we need to iterate over
    char **nextNodeKeys = adjNodes->keys;

    // Check if we've reached the destination
    if(pHead == pTargetNode) {
      success = 1;
      break;
    }

    // For each of the adjacent nodes
    for(int i = 0; i < adjNodes->count; i++) {

      // Grab the key
      char *key = nextNodeKeys[i];

      // Next node
      Node *pNextNode = HashMap_get(adjNodes, key);

      // Check if visited
      if(HashMap_get(visited, pNextNode->id) == VISITED)
        continue;

      // Add the next node to visited
      HashMap_put(visited, pNextNode->id, VISITED);

      // Set the prev of the node
      Node_setPrev(pNextNode, pHead);

      // Append the node to the queue
      Queue_add(nodeQueue, pNextNode);
    }

    // Add the head to visited
    HashMap_put(visited, pHead->id, VISITED);
  }

  // Garbage collection
  // We're only deleting the data structures, so the data itself should be safe
  HashMap_kill(visited, 0);
  Queue_kill(nodeQueue, 0);

  // Return whether or not it succeeded
  return success;
}

/**
 * Gets the adjacencies to a particular node.
 * 
 * @param   { char * }  id    The id of the node to inspect.
 * @param   { int }     cols  The number of cols for formatting data.
*/
void Model_printFriendList(char *id, int cols) {

  // Grab the node we want
  Node *pNode = HashMap_get(Model.nodes, id);

  // The id was invalid
  if(pNode == NULL) {
    printf("\tInvalid id.\n");
    return;
  }

  // Grab the details of the adjacency
  char **pNodeAdjs = HashMap_getKeys(pNode->adjNodes);
  int count = HashMap_getCount(pNode->adjNodes);

  // List all the friends of that node
  printf("\tFriends (%d): \n", count);
  
  // Print the adjacencies
  for(int i = 0; i < count; i++) {
    
    // Four columns only
    if(i % cols == 0)
      printf("\n\t");
    
    // Data print
    printf("%s,\t", pNodeAdjs[i]);
  }

  // Last newline
  printf("\n");
}

/**
 * Displays whether or not there's a connection between the two nodes.
 * Prints the appropriate message when none exists, or when one of the nodes are invalid.
 * 
 * @param   { char * }  sourceId  The id of the source node.
 * @param   { char * }  targetId  The id of the target node.
 * @param   { int }     cols      The number of cols for the formatting.
*/
void Model_printConnection(char *sourceId, char *targetId, int cols) {
  
  // Grab the nodes we want
  Node *pSourceNode = HashMap_get(Model.nodes, sourceId);
  Node *pTargetNode = HashMap_get(Model.nodes, targetId);

  // If either id was invalid
  if(pSourceNode == NULL || pTargetNode == NULL) {
    printf("\tAt least one of the ids was invalid.\n");
    return;
  }

  // Look for a connection
  int success = Model_generateConnection(pSourceNode, pTargetNode);   

  // No path could be found
  if(!success) {
    printf("\tA path could not be found.\n");
    return;
  }

  // Create a new stack so we can reverse the order
  Stack *pPathStack = Stack_new();
  Node *pNode = pTargetNode; 

  // Put the nodes unto the stack
  while(pNode != NULL) {

    // Push the current node
    Stack_push(pPathStack, pNode);

    // Go to adjacent node
    pNode = pNode->pPrevNode;
  }

  // Print that a path was found, reassign pNode to stack top
  printf("\tThe following path was found.\n\n");
  pNode = Stack_pop(pPathStack);

  // Very unconverntional for loop
  for(int i = 0; pNode != NULL; i++) {
    
    // Column formatting
    if(i % cols == 0)
      printf("\n\t");

    // Print the ids
    printf("=> %s\t", pNode->id);

    // Go to next in chain
    pNode = Stack_pop(pPathStack);
  }

  // Cleaner printing
  printf("\n");

  // Free the memory
  // Don't free the data cuz we're editing live nodes
  Stack_kill(pPathStack, 0);
}

/**
 * Clears the contents of the model.
 * Makes sure to perform proper garbage collection.
*/
void Model_clearData() {

  // If it's already cleared or smth
  if(!strcmp(Model.activeDataset, MODEL_EMPTY))
    return;
  
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

  // Empty the activeDataset string
  strcpy(Model.activeDataset, MODEL_EMPTY);
}

/**
 * Reads the file and converts its data into our model.
 * 
 * @param   { char * }  filepath  The path to the file to read.
 * @return  { int }               Whether or not the data was loaded.
*/
int Model_loadData(char *filepath) {
  
  // Create a file to the dataset
  File file;
  File_init(&file, filepath);
  
  // Try to open the file
  if(!File_open(&file, "r"))
    return 0;

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

  // Set the active dataset
  strcpy(Model.activeDataset, filepath);

  // Close the file
  File_close(&file);

  // Success
  return 1;
}

/**
 * Configures the data for drawing.
*/
void Model_drawData(char *filename) {

  // ! move this elsewhere later
  int width = 4096;
  int height = 4096;
  int minPointRadius = 2;
  int maxPointRadius = 10;
  int minPointBuffer = 20;
  int centerX = width / 2;
  int centerY = height / 2;
  
  // The number of nodes we're going to plot
  // The number of iterations to run the force sim
  int size = Model.nodeCount;
  int iterations = 60000;
  
  // The coordinates and weight of the nodes
  Point points[size];
  HashMap *pointMap = HashMap_new();
  double maxW = 0;

  // Init the nodes with random values and appropriate weights
  for(int i = 0; i < size; i++) {
    
    // Grab the node
    Node *pNode = Model.nodePointers[i];
    int angle = Rand_getMaxxed(360);
    int radius = (width + height) / 2 / 2;

    // Encode the point too
    HashMap_put(pointMap, pNode->id, &points[i]);

    // Init the point data
    Point_init(&points[i], 
      centerX + cos(angle) * radius,      // x coord 
      centerY + sin(angle) * radius,      // y coord
      pNode->adjNodes->count);            // weight
    
    // Grab the largest weight
    maxW = maxW < points[i].w ? points[i].w : maxW;
  }

  // Normalize the weights
  for(int i = 0; i < size; i++)
    points[i].w /= maxW;

  // For each of the simulation iterations
  for(int i = 0; i < iterations; i++) {
    
    // Progress update
    printf("%lf%%\n,", ((double) (i)) / iterations * 100);
    
    // For each of the nodes
    for(int j = 0; j < size; j++) {
      
      // Grab the point
      Point *pPoint = &points[j];
      Node *pPointNode = Model.nodePointers[j];

      // Compute dist from center
      double distSquareCenter = Point_getDistSquare(pPoint, centerX, centerY);
      distSquareCenter = distSquareCenter < 0.1 ? 0.1 : distSquareCenter;

      // Compute forces 
      Point_setForce(pPoint, 0, 0);
      Point_addForce(pPoint, 
        Point_getDistX(pPoint, centerX) / distSquareCenter * 1000.0 * pPoint->w * pPoint->w, 
        Point_getDistY(pPoint, centerY) / distSquareCenter * 1000.0 * pPoint->w * pPoint->w);

      // For all the other points
      for(int k = 0; k < size; k++) {
        
        // Grab the other point
        Point *pOther = &points[k];
        Node *pOtherNode = Model.nodePointers[k];
        int mult = 1;

        // Skip self
        if(k == j)
          continue;

        // Distance between points
        // Threshold represents min distance between connected points before repulsion
        double distSquarePoint = Point_getDistSquare(pPoint, pOther->x, pOther->y);
        double distPoint = Point_getDist(pPoint, pOther->x, pOther->y);
        double threshold = (pPoint->w + pOther->w) * (maxPointRadius + minPointBuffer);
        double K = 1;
        distSquarePoint = distSquarePoint < 0.1 ? 0.1 : distSquarePoint;
        
        // Check if there's a connection
        // If there is, invert the force
        if(HashMap_get(pPointNode->adjNodes, pOtherNode->id) != NULL)
          mult *= -(threshold - distPoint) * K;

        // Add attractive / repuslive force
        Point_addForce(pPoint, 
          mult * Point_getDistX(pPoint, pOther->x) / distSquarePoint / 25.0 * pOther->w * pOther->w, 
          mult * Point_getDistY(pPoint, pOther->y) / distSquarePoint / 25.0 * pOther->w * pOther->w);
      }
    }

    // Move the points after
    for(int j = 0; j < size; j++)
      Point_move(&points[j]);
  }

  // Create the bmp
  BMP bmp;
  BMP_create(&bmp, width, height);

  // Populate with random pixels for now
  color red = Color_fromRGB(255, 121, 0);
  color blue = Color_fromRGB(90, 24, 154);
  color black = Color_fromRGB(0, 0, 0);
  color white = Color_fromRGB(255, 255, 255);

  // Draw the adjacencies
  for(int i = 0; i < size; i++) {

    // Grab the node
    Node *pNode = Model.nodePointers[i];
    char *id = pNode->id;

    // Grab the adjacent nodes
    for(int j = 0; j < pNode->adjNodes->count; j++) {

      // Grab the adjacent node and its deets
      char *key = HashMap_getKeys(pNode->adjNodes)[j];
      Node *pAdj = HashMap_get(Model.nodes, key);
      char *adjId = pAdj->id;

      // Grab the points
      Point *p1 = HashMap_get(pointMap, id);
      Point *p2 = HashMap_get(pointMap, adjId);

      // Draw the line between the points
      BMP_encodeLine(&bmp, p1->x, p1->y, p2->x, p2->y, white, 0.04);
    }
  }

  // Draw the nodes themselves
  for(int i = 0; i < size; i++) {

    // Grab the coords
    int x = points[i].x;
    int y = points[i].y;
    double w = points[i].w;

    // Make sure the point is in bounds
    if(x - 1 >= 0 && x + 1 < width &&
      y - 1 >= 0 && y + 1 < height) {
      BMP_encodeCircle(&bmp, 
        x, y, 
        w * (maxPointRadius - minPointRadius) + minPointRadius, 
        Color_lerp(blue, red, w), 1);
    }
  }

  // Write the file
  BMP_writeFile(&bmp, filename);
  BMP_kill(&bmp);

  // Kill the hashmap
  HashMap_kill(pointMap, 0);
}

/**
 * Draws the model into a bmp.
*/
void Model_renderDrawing(char *filename) {
  
  // The bmp file and its details
  BMP bmp;
  int width = 512;
  int height = 512;

  // Create the bmp
  BMP_create(&bmp, width, height);

  // Populate with random pixels for now
  color red = Color_fromRGB(255, 0, 0);
  color blue = Color_fromRGB(0, 0, 255);

  // Encode the pixel data
  for(int i = 0; i < width; i++) {
    for(int j = 0; j < height; j++) {
      if((i + j) % 2) BMP_encodePixel(&bmp, i, j, Color_lerp(red, blue, (i) / ((double) width)));
      else BMP_encodePixel(&bmp, i, j, Color_lerp(blue, red, (j) / ((double) height)));
    }
  }

  // Write the file
  BMP_writeFile(&bmp, filename);
  BMP_kill(&bmp);
}

#endif