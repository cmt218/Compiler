/*
  CSE109
  Cole Tomlinson
  cmt218
  Trie data structure program
  Program #3
*/

class Node;
#ifndef TRI_H
#define TRI_H
#include "node.h"
#include "link.h"
#include <iostream>
#include <cstring>

using namespace std;

class Tri{
public:

  //Member variable
  Node* sNode;

  //Member functions
  void put(string key, string val);
  string get(string key);
  bool contains(string key);
  
  //Constructor
  Tri();

  //Destructor
  ~Tri();

};

#endif
