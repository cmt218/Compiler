
/*
CSE109
Cole Tomlinson
cmt218
Trie data structure program
Program #3
*/

#ifndef LINK_H
#define LINK_H

#include <iostream>
using namespace std;

class Node;

class Link {
public:

  //Member Variables
  char letter;
  Node* next;

  //Member Functions
  void setLet(char a);
  char getLet();
  
  //Constructor
  Link();

  //Destructor
  ~Link();
  

};

#endif
