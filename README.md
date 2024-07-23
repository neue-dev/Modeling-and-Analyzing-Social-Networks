# Modeling and Analyzing Networks

## Overview

## Data Structures Used

### Hash Map

### Queue

### Stack

## Graph Representation

### Record

this represents the actual data we store in the graph
in this case, given the nature of the data, a record only contains an id and a name (which we just make equal for the purposes of this program)

we never actually needed to access the records for the purposes of this program, but that's okay; in case we want to extend our program to store much more complicated data, we could do so by changing this struct. the behaviour of the program will still be the same, and graphs can still be modeled.

### Node

these are just pointers; the actual data are stored in the records
all the nodes do are help us create relationships among the records; so by swapping the records out, we can store somthing else
it's proper encapsulation!

### Model

The model performs all the graph-related logic and operates on the nodes.
This is where traversing the graph, adding /removing nodes, etc. are done. 
I tried my best to separate out the printing functions as much as i could.

> Constructing the representation from the data structures