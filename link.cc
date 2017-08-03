/*
  CSE109
  Cole Tomlinson
  cmt218
  Trie data structure program
  Program #3
*/


#include <cstdlib>
#include "link.h"

//Constructor
Link::Link() {
}

//Destructor
Link::~Link() {
}

//Method for setting the letter for a link
void Link::setLet(char a) {
  letter = a;
}

//Method for returning the letter of a link
char Link::getLet() {
  return letter;
}
