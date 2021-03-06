/*J Femister
  CSE 109
*/
#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include <iostream>
#include "tri.h"
using namespace std;

/* 
   A modified implementation of a stack template.
   Credit to: J Femister
*/

template<class T>
class Stack {
public:
  int size;
  Stack(int n);
  Stack();
  Stack(const Stack&);
  ~Stack();
  //Methods for parser symbol table
  string itos(int i);
  void enterScope();
  T exitScope();
  int addSymbol(string sym);
  string getUniqueSymbol(string sym);
  void push(T val);
  T pop();
  T peek() const;
  bool isEmpty() const { return tos == EMPTY; } 
  friend ostream& operator <<(ostream& out,const Stack<T>& s);
  T& operator [](int i);
  const T operator [](int i) const;
private:
  T *stack;
  //int size;
  int tos;
  static const int EMPTY = -1;
  void checkIndex(int i) const;
};
#endif
