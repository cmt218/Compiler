/*
CSE109
Cole Tomlinson
cmt218
Trie data structure program
Program #3
*/

#include <cstdlib>
#include "node.h"
#include <cstring>

//Constructor
Node::Node(){
  numLinks = 0;
}

//Destructor
Node::~Node() {
}

//Method for setting the value of a node
void Node::setVal(string val){
  nodeVal = val;
}

//Method for returning the value of a node
string Node::getVal() {
  return nodeVal;
}
