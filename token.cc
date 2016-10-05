/*
  CSE109
  Cole Tomlinson
  cmt218
  Lexer program
  Program #4
*/

#include "token.h"

//No arg constructor
Token::Token(){
  type = 0;
  lexeme = "";
  line = 0;
  pos = 0;
}

//Constructor for initializing fields
Token::Token(int t,string l,int n,int p){
  type = t;
  lexeme = l;
  line = n;
  pos = p;
}

//Empty destructor
Token::~Token(){
}

//Gets type of token
int Token::getType(){
  return type;
}

//Gets lexeme of token
string Token::getLexeme(){
  return lexeme;
}

//Gets line token is on
int Token::getLine(){
  return line;
}

//Gets line position of token
int Token::getPos(){
  return pos;
}


