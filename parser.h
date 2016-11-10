/*
    # CSE109
    # Cole Tomlinson
    # cmt218
    # Parser Program
    # Program 5
*/
#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "lexer.h"
#include "tri.h"
#include "symbolTable.h"
#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <sstream>

using namespace std;

class Parser {
  
private:

  enum Operation {
    ADD, SUB, MULT, DIV, // Arithmetic Operators
    ISEQ, ISNE, ISLT, ISLE, ISGT, ISGE, // Relational Operators
    AND, OR, // Logical Operators
    PUSHL, PUSHV, STORE, // Value Transfer Instructions
    JUMP, JUMPF, JUMPT, CALL, RET, // Location Transfer Instructions
    PRINTF, PARAM, // Misc
    LABEL, SEQ // Pseudo Operations
  };



  
public:  
  class TreeNode {
    
  public:

    Operation op;
    string val; // Variable name or jump label
    TreeNode *leftChild;
    TreeNode *rightChild;

    
    TreeNode* getl() {
      return leftChild;
    }

    TreeNode* getr() {
      return rightChild;
    }
    
    // Utility function used by constructors
    void init(Operation opx, string valx, TreeNode *leftChildx, TreeNode *rightChildx) {
      op = opx;
      val = valx;
      leftChild = leftChildx;
      rightChild = rightChildx;
    }


    TreeNode(Operation op, string val) {
      init(op, val, NULL, NULL);
    }
    
    TreeNode(Operation op, string val, TreeNode *leftChild, TreeNode *rightChild) {
      init(op, val, leftChild, rightChild);
    }

    TreeNode(Operation op) {
      init(op, "", NULL, NULL);
    }

    TreeNode(Operation op, TreeNode *leftChild, TreeNode *rightChild) {
      init(op, "", leftChild, rightChild);
    }

    static string toString(TreeNode *node) {
      return toString0(node, 0);
    }
    
    static string toString0(TreeNode *node, int spaces) {


      static string blanks = "                                        ";
      string left = "";
      string right = "";
      bool isLeaf = true;
      if (node->leftChild != NULL) {
	left = toString0(node->leftChild, spaces+2);
	isLeaf = false;
      }
      if (node->rightChild != NULL) {
	right = toString0(node->rightChild, spaces+2);
	isLeaf = false;	
      }
      string ret;
      if (isLeaf) {
	ret = blanks.substr(0, spaces) + ops[node->op] +  "[" + node->val + "]";
      } else {
	ret = blanks.substr(0, spaces) + ops[node->op] +  "(\n" + left + ",\n" + right + "\n" + 
	      blanks.substr(0, spaces) + ")";
      }
      return ret;
    }
    
  };
  
private:
  Lexer lexer;
  Token token;
  ostream& out;
  int lindex;
  int tindex;
  Stack<Tri> table;
  
  string itos(int i) { stringstream ss; ss << i; string res = ss.str(); return res;}
  string makeLabel() { string tmp = "L"; stringstream ss; ss << ++lindex; string res = ss.str(); tmp = tmp + res;

  return tmp;}

  static const string ops[];  
  void error(string message);
  void check(int tokenType, string message);
  
 public:

  void tableTest();
  
  //TreeNode* funcall(string functionName);
  TreeNode* factor();
  TreeNode* term();
  TreeNode* expression();
  TreeNode* relationalExpression();
  TreeNode* logicalExpression();
  //TreeNode* assignmentExpression();
  TreeNode* returnStatement();
  //TreeNode* printfStatement();
  TreeNode* whileStatement();
  TreeNode* ifStatement();
  TreeNode* assignStatement();
  TreeNode* vardefStatement();  
  TreeNode* statement();
  TreeNode* block();
  //TreeNode* parameterdef();
  //TreeNode* parameterdefs();
  TreeNode* function();
  TreeNode* compilationunit();  
  
    
  Parser(Lexer& lexer, ostream& out);
  ~Parser(); 

};

#endif
