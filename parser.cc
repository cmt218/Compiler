/*
  # CSE109
  # Cole Tomlinson
  # cmt218
  # Parser Program 
  # Program 5
*/
#include "parser.h"
#include <cstring>
#include <stdlib.h>
#include <stdio.h> 

const string Parser::ops[] = {"ADD", "SUB", "MULT", "DIV",

		      "ISEQ", "ISNE", "ISLT", "ISLE", "ISGT", "ISGE",

		      "AND", "OR",

		      "PUSHL", "PUSHV", "STORE",

		      "JUMP", "JUMPF", "JUMPT", "CALL", "RET",

		      "PRINTF",

		      "LABEL", "SEQ" };


Parser::Parser(Lexer& lexerx, ostream& outx): lexer(lexerx), out(outx), lindex(1), tindex(1) {  
  token = lexer.nextToken();
}

Parser::~Parser() {
}

void Parser::error(string message) {
  cerr << message << " Found " << token.getLexeme() << " at line " << token.getLine() << " position " << token.getPos() << endl;
  exit(1);
}

void Parser::check(int tokenType, string message) {
  if (token.getType() != tokenType)
    error(message);
}

//DONE
Parser::TreeNode* Parser::factor() {
  TreeNode* f;

  //Case with expression in parenthesis
  if(token.getType() == 17){
    token = lexer.nextToken();
    f = expression();
    check(18, "Missing closing parenthesis!");
  }

  //Case of identifier
  else if(token.getType() == 1){
    f = new TreeNode(PUSHV, token.getLexeme());
  }

  //Case of int literal
  else if(token.getType() == 2){
    f = new TreeNode(PUSHL, token.getLexeme());
  }

  //Ready the next token and return factor
  token = lexer.nextToken();
  return f;
}

//DONE
Parser::TreeNode* Parser::term() {
  TreeNode* t = factor();
  TreeNode* o;

  //Multiplication case
  if(token.getType() == 6){
    o = new TreeNode(MULT);
    o -> leftChild = t;
    token = lexer.nextToken();
    o -> rightChild = factor();
  }

  //Division case
  else if(token.getType() == 7){
    o = new TreeNode(DIV);
    o -> leftChild = t;
    token = lexer.nextToken();
    o -> rightChild = factor();
  }

  //Factor case
  else{
    o = t;
  } 
  return o;
}

//DONE
Parser::TreeNode* Parser::expression() {
  TreeNode* e = term();
  TreeNode* p;

  //Addition case
  if(token.getType() == 4){
    p = new TreeNode(ADD);
    p -> leftChild = e;
    token = lexer.nextToken();
    p -> rightChild = term();
  }

  //Subtraction case
  else if(token.getType() == 5){
    p = new TreeNode(SUB);
    p -> leftChild = e;
    token = lexer.nextToken();
    p -> rightChild = term();
  }

  //Term case
  else{
    p = e;
  }
  return p;
}

//DONE
Parser::TreeNode* Parser::relationalExpression() {
  TreeNode* r = expression();
  TreeNode* s;

  //Equality case
  if(token.getType() == 9){
    s = new TreeNode(ISEQ);
    s -> leftChild = r;
    token = lexer.nextToken();
    s -> rightChild = expression();
  }

  //Not equal to case
  else if(token.getType() == 10){
    s = new TreeNode(ISNE);
    s -> leftChild = r;
    token = lexer.nextToken();
    s -> rightChild = expression();
  }

  //Less than case
  else if(token.getType() == 11){
    s = new TreeNode(ISLT);
    s -> leftChild = r;
    token = lexer.nextToken();
    s -> rightChild = expression();
  }

  //Less than or equal to case
  else if(token.getType() == 12){
    s = new TreeNode(ISLE);
    s -> leftChild = r;
    token = lexer.nextToken();
    s -> rightChild = expression();
  }

  //Greater than case
  else if(token.getType() == 13){
    s = new TreeNode(ISGT);
    s -> leftChild = r;
    token = lexer.nextToken();
    s -> rightChild = expression();
  }

  //Greater than or equal case
  else if(token.getType() == 14){
    s = new TreeNode(ISGE);
    s -> leftChild = r;
    token = lexer.nextToken();
    s -> rightChild = expression();
  }

  //Expression case
  else{
    s = r;
  }
  return s;
}

//DONE
Parser::TreeNode* Parser::logicalExpression() {
  TreeNode* l = relationalExpression();
  TreeNode* e;

  //Logical and case
  if(token.getType() == 15){
    e = new TreeNode(AND);
    e -> leftChild = l;
    token = lexer.nextToken();
    e -> rightChild = relationalExpression();
  }

  //Logical or case
  else if(token.getType() == 16){
    e = new TreeNode(OR);
    e -> leftChild = l;
    token = lexer.nextToken();
    e -> rightChild = relationalExpression();
  }

  //Relational expression case
  else{
    e = l;
  }
  return e;
}

Parser::TreeNode* Parser::block(){
  TreeNode* b;
  TreeNode* cur = new TreeNode(SEQ, NULL, NULL);
  //Accounts for one or more statements contained in brackets
  check(19, "Missing opening brace!");
  token = lexer.nextToken();
  while(token.getType() != 20){
    if(cur -> leftChild == NULL){
      cur -> leftChild = statement();
    }
    else if(cur -> rightChild == NULL){
      cur -> rightChild = statement();
    }
    else{
      cur = new TreeNode(SEQ, cur, NULL);
      cur -> rightChild = statement();
    }
    
  }
  b = cur;
  token = lexer.nextToken();
  return b;
}

Parser::TreeNode* Parser::ifStatement() {
  TreeNode* SEQ1;
  TreeNode* SEQ2;

  //Gets nodes for logical expression and then block
  if(token.getType() == 23){
    token = lexer.nextToken();
    check(17, "Missing opening parenthesis!");
    token = lexer.nextToken();
    TreeNode* LE = logicalExpression();
    check(18, "Missing closing parenthesis!");
    token = lexer.nextToken();
    TreeNode* TB = block();
    //Creates tree if there is an additional else statement
    if(token.getType() == 24){
      token = lexer.nextToken();
      TreeNode* EB = block();
      string jfLabel = makeLabel();
      TreeNode* JF = new TreeNode(JUMPF, jfLabel);
      SEQ1 = new TreeNode(SEQ, LE, JF);
      SEQ2 = new TreeNode(SEQ, SEQ1, TB);
      string jLabel = makeLabel();
      TreeNode* J = new TreeNode(JUMP, jLabel);
      SEQ1 = new TreeNode(SEQ, SEQ2, J);
      jfLabel += ":";
      TreeNode* L1 = new TreeNode(LABEL, jfLabel);
      SEQ2 = new TreeNode(SEQ, SEQ1, L1);
      SEQ1 = new TreeNode(SEQ, SEQ2, EB);
      jLabel += ":";
      TreeNode* L2 = new TreeNode(LABEL, jLabel);
      SEQ2 = new TreeNode(SEQ, SEQ1, L2);
    }
    //Creates tree in the absence of an if statement 
    else{
      token = lexer.nextToken();
      string jfLabel = makeLabel();
      TreeNode* JF = new TreeNode(JUMPF, jfLabel);
      jfLabel += ":";
      TreeNode* L1 = new TreeNode(LABEL, jfLabel);
      SEQ2 = new TreeNode(SEQ, LE, JF);
      SEQ1 = new TreeNode(SEQ, SEQ2, TB);
      SEQ2 = new TreeNode(SEQ, SEQ1, L1);
    }
  }
  
  token = lexer.nextToken();
  return SEQ2;
}

Parser::TreeNode* Parser::whileStatement() {

  //All necessary SEQ nodes for while statement
  TreeNode* SEQ1;
  TreeNode* SEQ2;
  TreeNode* SEQ3;
  TreeNode* SEQ4;
  TreeNode* SEQ5;

  
  if(token.getType() == 25){
    //Creates necessary jump and label nodes
    string jLabel = makeLabel();
    string jfLabel = makeLabel();
    TreeNode* J = new TreeNode(JUMP, jLabel);
    TreeNode* JF = new TreeNode(JUMPF, jfLabel);
    jLabel += ":";
    jfLabel += ":";
    TreeNode* L1 = new TreeNode(LABEL, jLabel);
    TreeNode* L2 = new TreeNode(LABEL, jfLabel);

    //Creates logical expression and block nodes
    token = lexer.nextToken();
    check(17, "Missing opening parenthesis!");
    token = lexer.nextToken();
    TreeNode* LE = logicalExpression();
    check(18, "Missing closing parenthesis!");
    token = lexer.nextToken();
    TreeNode* BK = block();

    //Links everything together with SEQ nodes
    SEQ1 = new TreeNode(SEQ, L1, LE);
    SEQ2 = new TreeNode(SEQ, SEQ1, JF);
    SEQ3 = new TreeNode(SEQ, SEQ2, BK);
    SEQ4 = new TreeNode(SEQ, SEQ3, J);
    SEQ5 = new TreeNode(SEQ, SEQ4, L2);
  }

  token = lexer.nextToken();
  return SEQ5;
}

Parser:: TreeNode* Parser::assignStatement(){
  TreeNode* a;

  //Assigns a literal to a variable 
  if(token.getType() == 1){
    TreeNode* ID = new TreeNode(STORE, token.getLexeme());
    token = lexer.nextToken();
    check(8, "Missing assignment operator");
    token = lexer.nextToken();
    TreeNode* VAL = logicalExpression();
    a = new TreeNode(SEQ, VAL, ID);
    check(22, "Missing semicolon!");
  }

  token = lexer.nextToken();
  return a;
}

Parser:: TreeNode* Parser::statement() {
  TreeNode* s;

  //Assign case
  if(token.getType() == 1){
    s = assignStatement();
  }

  //While case
  else if(token.getType() == 25){
    s = whileStatement();
  }

  //If case
  else if(token.getType() == 23){
    s = ifStatement();
  }

  //Unrecognized case
  else{
    error("Unrecognized token type");
  }

  return s;
}
Parser:: TreeNode* Parser::compilationunit(){
  TreeNode* c = statement();
  return c;
}

