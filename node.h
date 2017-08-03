/*
  CSE109
  Cole Tomlinson
  cmt218
  Trie data structure program
  Program #3
*/

#ifndef NODE_H
#define NODE_H

#include "link.h"
#include <iostream>
#include <cstring>

using namespace std;

class Node {
public:

  //Member variables
  string nodeVal;
  Link* links[10];
  int numLinks;

  //Member functions
  void setVal(string val);
  string getVal();
  
  //Constructor
  Node();

  //Destructor
  ~Node();

}; 

#endif
