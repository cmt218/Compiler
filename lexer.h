/*
  CSE109
  Cole Tomlinson
  cmt218
  Lexer Program
  Program #4
*/

#ifndef LEXER_H
#define LEXER_H
#include <iostream>
#include "token.h"

using namespace std;

class Lexer {
public:
  //Member variables
  istream &input;
  int lineNum;
  int linePos;
  
  //Constructor
  Lexer(istream&);

  //Member functions
  Token nextToken();
  int peep();
  char peek();
  
private:
  //Private member function
  char nextChar();
};

#endif
