/**
 * @ Author: Mo David
 * @ Create Time: 2024-07-16 16:33:56
 * @ Modified time: 2024-07-16 16:59:48
 * @ Description:
 * 
 * Nodes will always have a string id associated with them.
 * They also store a collection of references to other nodes via their ids.
 */

package Model;

import java.util.HashMap;

public class Node<Data> {
  
  // Data of the node
  private String id;
  private Data data;

  // References to adjacent nodes
  private HashMap<String, Node> prevNodes;
  private HashMap<String, Node> nextNodes;

  /**
   * Instantiates an instance of the Node class.
   * 
   * @param   { String }  The id associated with the node.
   * @param   { Data }    Any object of the given type, to be stored by the node.
   */
  public Node(String id, Data data) {

    // Save the node details
    this.id = id;
    this.data = data;

    // Init the data structs
    this.prevNodes = new HashMap<>();
    this.nextNodes = new HashMap<>();
  }

  /**
   * Retrieves the id of the given node.
   * 
   * @return  { String }  The id of the node.
   */
  public String getId() {
    return this.id;
  }

  /**
   * Returns a string array containing all the ids of the next nodes.
   * 
   * @return  { String[]  }   A string array with all next node ids. 
   */
  public String[] getNextNodes() {
    return (String[]) this.nextNodes.keySet().toArray();
  }

  /**
   * Returns a string array containing all the ids of the prev nodes.
   * 
   * @return  { String[]  }   A string array with all prev node ids. 
   */
  public String[] getPrevNodes() {
    return (String[]) this.prevNodes.keySet().toArray();
  }

  /**
   * Adds a node in the next map of the current node.
   * 
   * @param   { Node }  next  The node to append.
   */
  public void addNextNode(Node next) {
    String id = next.getId();

    this.nextNodes.put(id, next);
  }

  /**
   * Adds a node in the prev map of the current node.
   * 
   * @param   { Node }  prev  The node to append.
   */
  public void addPrevNode(Node prev) {
    String id = prev.getId();

    this.prevNodes.put(id, prev);
  }

  /**
   * Returns whether or not the node is in the next map of the current node.
   * 
   * @param   { Node }  next  The node to check for.
   */
  public boolean hasNextNode(Node next) {
    String id = next.getId();

    return this.nextNodes.containsKey(id);
  }

  /**
   * Returns whether or not the node is in the prev map of the current node.
   * 
   * @param   { Node }  prev  The node to check for.
   */
  public boolean hasPrevNode(Node prev) {
    String id = prev.getId();

    return this.prevNodes.containsKey(id);
  }
}
