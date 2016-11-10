/*
  # CSE109
  # Cole Tomlinson
  # cmt218
  # Parser Program 
  # Program 5
*/
#include "parser.h"
#include "symbolTable.cc"
#include "tri.h"
#include <cstring>
#include <stdlib.h>
#include <stdio.h> 

const string Parser::ops[] = {"ADD", "SUB", "MULT", "DIV",

		      "ISEQ", "ISNE", "ISLT", "ISLE", "ISGT", "ISGE",

		      "AND", "OR",

		      "PUSHL", "PUSHV", "STORE",

		      "JUMP", "JUMPF", "JUMPT", "CALL", "RET",

		      "PRINTF","PARAM",

		      "LABEL", "SEQ" };


Parser::Parser(Lexer& lexerx, ostream& outx): lexer(lexerx), out(outx), lindex(1), tindex(1), table(Stack<Tri>(15)){
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


//Method for testing symbolTable functionality
void Parser::tableTest(){

}



Parser::TreeNode* Parser::factor() {
  TreeNode* f;
  
  //Case with expression in parenthesis
  if(token.getType() == 17){
    token = lexer.nextToken();
    f = expression();
    check(18, "Missing closing parenthesis!");
  }
  //Case of either funcall or identifier
  else if(token.getType() == 1){
    //Funcall case DONE
    if(lexer.peek() == '('){
      TreeNode* C = new TreeNode(CALL, token.getLexeme());
      TreeNode* R;
      Stack<TreeNode*> exps;
      //Gets to token after opening paren
      token = lexer.nextToken();
      token = lexer.nextToken();
      bool pCheck = false;
      //Pushes parameters to stack if they exist
      if(token.getType() != 18){
	pCheck = true;
	int numParams = lexer.peep();
	Stack<TreeNode*> exps(numParams);
	Stack<TreeNode*> ordered(numParams);
	while(token.getType() != 18){	
	  if(token.getType() == 21){
	    token = lexer.nextToken();
	    continue;
	  }

	  table.addSymbol(token.getLexeme());
	  R = expression();
	  R = new TreeNode(PUSHV, token.getLexeme());
	  //table.addSymbol(token.getLexeme());
	  exps.push(R);
	}
	//PUT THIS BACK IN IF SOMETHING BREAKS
	token = lexer.nextToken();
	//Puts parameters on another stack in reverse order
	while(!exps.isEmpty()){
	  ordered.push(exps.pop());
	}
	//Assembles tree with parameters and call
	R = new TreeNode(SEQ, ordered.pop(), NULL);
	if(!ordered.isEmpty()){
	  R = new TreeNode(SEQ, R->leftChild, ordered.pop());
	}
	while(!ordered.isEmpty()){
	  R = new TreeNode(SEQ, R, ordered.pop());
	}
	R = new TreeNode(SEQ, R, C);
      }	
      //Returns root of the funcall
      if(pCheck){
	//TAKE THIS OUT IF SOMETHING BREAKS
	//token = lexer.nextToken();
	return R;
      }
      else{
	
	//token = lexer.nextToken();
	return C;
      }      
    }
    //Identifier case 
    else{
      //f = new TreeNode(PUSHV, token.getLexeme());
      string unique = table.getUniqueSymbol(token.getLexeme());
      if(unique.compare("") == 0){
	error("variable not found");
      }
      f = new TreeNode(PUSHV, unique);
    }
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
  //check(19, "Missing opening brace!");
  //THIS IS A BAD FIX
  if(token.getType() == 19){
    token = lexer.nextToken();
  }
  
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
    table.enterScope();
    TreeNode* TB = block();
    table.exitScope();
    //Creates tree if there is an additional else statement
    if(token.getType() == 24){
      token = lexer.nextToken();
      table.enterScope();
      TreeNode* EB = block();
      table.exitScope();
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
    table.enterScope();
    TreeNode* BK = block();
    table.exitScope();
    
    //Links everything together with SEQ nodes
    SEQ1 = new TreeNode(SEQ, L1, LE);
    SEQ2 = new TreeNode(SEQ, SEQ1, JF);
    SEQ3 = new TreeNode(SEQ, SEQ2, BK);
    SEQ4 = new TreeNode(SEQ, SEQ3, J);
    SEQ5 = new TreeNode(SEQ, SEQ4, L2);
  }

  //token = lexer.nextToken();
  return SEQ5;
}

Parser:: TreeNode* Parser::assignStatement(){
  TreeNode* a;

  //Assigns a literal to a variable 
  if(token.getType() == 1){
    //TreeNode* ID = new TreeNode(STORE, token.getLexeme());
    string unique = table.getUniqueSymbol(token.getLexeme());
    if(unique.compare("") == 0){
      error("Variable not found");
    }
    TreeNode* ID = new TreeNode(STORE, unique);
    token = lexer.nextToken();
    check(8, "Missing assignment operator");
    token = lexer.nextToken();
    TreeNode* VAL = logicalExpression();
    a = new TreeNode(SEQ, VAL, ID);
    check(22, "Missing semicolon!!");
  }

  token = lexer.nextToken();
  return a;
}

Parser:: TreeNode* Parser::vardefStatement() {
  TreeNode* V;
  Stack<TreeNode*> vars(10);
  token = lexer.nextToken();
  //Push variables onto stack
  while(token.getType() != 22){
    if(token.getType() == 21){
      token = lexer.nextToken();
      continue;
    }
    V = new TreeNode(PUSHV, token.getLexeme());
    table.addSymbol(token.getLexeme());
    vars.push(V);
    token = lexer.nextToken();
  }

  //SEQ variables together
  V = new TreeNode(SEQ, vars.pop(), NULL);
  if(!vars.isEmpty()){
    V = new TreeNode(SEQ, V->leftChild, vars.pop());
  }
  while(!vars.isEmpty()){
    V = new TreeNode(SEQ, V, vars.pop());
  }
  token = lexer.nextToken();
  return V;
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

  //Vardef case
  else if(token.getType() == 27){
    s = vardefStatement();
  }

  //Return case
  else if(token.getType() == 29){
    s = returnStatement();
  }
  
  //Unrecognized case
  else{
    error("Unrecognized token type");
  }

  return s;
}

Parser:: TreeNode* Parser::returnStatement(){
  TreeNode* R;
  if(token.getType() == 29){
    token = lexer.nextToken();
    R = logicalExpression();
    TreeNode* RE = new TreeNode(RET, NULL, NULL);
    R = new TreeNode(SEQ, R, RE); 
  }
  check(22, "missing semicolon!");
  token = lexer.nextToken();
  return R;
}


Parser:: TreeNode* Parser::function(){
  TreeNode* F;
  if(token.getType() == 26){
    token = lexer.nextToken();
    F = factor();
    token = lexer.nextToken();
    table.enterScope();
    TreeNode* BK = block();
    table.exitScope();
    F = new TreeNode(SEQ, F, BK);
    return F;
  }

  return NULL;
}

Parser:: TreeNode* Parser::compilationunit(){
  table.enterScope();
  TreeNode* C;
  Stack<TreeNode*> funcs(10);
  while(token.getType() != 30){
    C = function();
    funcs.push(C);
  }

  C = new TreeNode(SEQ, funcs.pop(), NULL);
  if(!funcs.isEmpty()){
    C = new TreeNode(SEQ, C->leftChild, funcs.pop());
  }
  while(!funcs.isEmpty()){
    C = new TreeNode(SEQ, C, funcs.pop());
  }
  
  
  return C;
}

