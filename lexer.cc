/*
  CSE109
  Cole Tomlinson
  cmt218
  Lexer Program
  Program #4
*/

#include "lexer.h"
#include "token.h"
#include <string>
#include <iostream>
#include <cctype>

using namespace std;

//Constructor
Lexer::Lexer(istream &in):input(in){
  lineNum = 0;
  linePos = 0;
}


//Method for parser to see next character
char Lexer::peek() {
  return input.peek();
}

//Method for parser to count parameters
int Lexer::peep() {
  int params = 1;
  int og = input.tellg();
  char cur = input.get();
  while(cur != ')'){
    if(cur == ','){
      params ++;
    }
    cur = input.get();
  }
  
  input.seekg(og);
  return params;
}



//Nextchar method
char Lexer::nextChar(){
  //Increment line position every time a new character is accessed
  linePos++;
  char cur = input.get();
  //Detects if the end of file has been reached
  if(input.eof()){
    return '#';
  }
  //Detects new line character
  else if(cur == '\n'){
    lineNum++;
    linePos = 0;
    return ' ';
  }
  //Return the next character otherwise
  else{
    return cur;
  }
}

//Nexttoken method
Token Lexer::nextToken() {
  char next = nextChar();
  //Ignore blank spaces
  while(next == ' '){
    next = nextChar();
    linePos++;
  }
  //ignore tab
  if(next == '\t'){
    next = nextChar();
    linePos+=3;
  }

  int startLine = lineNum;
  int startPos = linePos;
  string lexeme = "";
  lexeme += next;
  //Will return error token if it is not one of the possible tokens
  Token toReturn(31,lexeme,startLine,startPos);

  //End of file case
  if(next == '#'){
    Token toReturn(30,lexeme,startLine,startPos);
    return toReturn;
  }
  
  //First char is a letter
  else if(isalpha(next)){
    //Keeps adding subsequent letters and numbers to lexeme
    while(isalpha(next) || isdigit(next)){
      //this is experimental
      if(isalpha(input.peek()) || isdigit(input.peek())){
	  next = nextChar();
      }
      else{
	break;
      }
      lexeme += next;
    }

    //Array of keywords and matching values
    string keyWords[7] = {"if","else","while","function","var","printf","return"};
    int vals[7] = {23,24,25,26,27,28,29};
    bool ident = true;

    //Iterates through keywords to see if the lexeme is one of them
    for(int i = 0;i < 7;i++){
      if(lexeme == keyWords[i]){
	Token toReturn(vals[i],lexeme,startLine,startPos);
	ident = false;
	return toReturn;
      }
    }

    if(ident){
      Token toReturn(1,lexeme,startLine,startPos);
      return toReturn;
    }
  }

  //First char is a number
  else if(isdigit(next)){
    //Keep adding subsequent digits
    while(isdigit(next)){
      if(isdigit(input.peek())){
	next = nextChar();
      }
      else{
	break;
      }
      lexeme += next;
    }
    Token toReturn(2,lexeme,startLine,startPos);
    return toReturn;
  }

  //String literal
  else if(next == '"'){
    //Keep adding characters until next quote is reached
    while(!(input.peek() == '"')){
      next = nextChar();
      lexeme += next;
    }
    next = nextChar();
    lexeme += next;
    Token toReturn(3,lexeme,startLine,startPos);
    return toReturn;
  }
  //Operators and punctuation
  else{
    bool spec = false;
    //Array of special characters and repeatable special characters
    char specs[17] = {'+','-','*','/','=','!','<','>','&','|','(',')','{','}',',','.'};
    char sspecs[3] = {'=','&','|'};

    //Checks if character is a special character
    for(int x = 0;x<17;x++){
      if(next == specs[x]){
	spec = true;
	break;
      }
    }

    //Checks if there is a subsequent character
    char check = input.peek();
    spec = false;
    for(int a = 0;a<3;a++){
      if(check == sspecs[a]){
	spec = true;
	break;
      }
    }

    //Adds subsequent special characters to lexeme
    while(spec){
      next = nextChar();
      spec = false;
      for(int y = 0;y<17;y++){
	if(next == specs[y]){
	  lexeme += next;
	  break;
	}
	for(int b=0;b<3;b++){
	  if(input.peek() == sspecs[b]){
	    spec = true;
	  }
	}
      }
    }

    //Arrays of all special tokens and their values
    string stoks[19] = {"+","-","*","/","=","==","!=","<","<=",">",">=","&&","||","(",")","{","}",",",";"};
    int stokvals[19] = {4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22};

    //Returns token of the correct type
    for(int z = 0;z<19;z++){
      if(lexeme == stoks[z]){
	Token toReturn(stokvals[z],lexeme,startLine,startPos);
	return toReturn;
      }
    }
  }
  return toReturn;
}
