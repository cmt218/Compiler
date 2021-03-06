#include "symbolTable.h"
#include <cstdlib>
#include "tri.h"
#include <string>
#include <iostream>
#include <sstream>
#include <cstring>

/*
     A modified implementation of a stack template.
     original template credit: J Femister
*/


template<class T>
Stack<T>::Stack(int n):size(n),tos(Stack<T>::EMPTY) {
  stack = new T[size];
}

template<class T>
Stack<T>::Stack():size(10),tos(-1) {
  stack = new T[size];
}

template<class T>
Stack<T>::Stack(const Stack<T>& s)  { // Copy constructor
  size = s.size;
  tos = s.tos;
  stack = new T[size];
  for (int i = 0; i <= tos; ++i) {
    stack[i] = s.stack[i];
  }
}

template<class T>
Stack<T>::~Stack() {
  delete [] stack;
}

//Public members for parser symbol table


template<class T>
string Stack<T>::itos(int i)  {
  stringstream ss; ss << i; string res = ss.str(); return res;
}

template<class T>
void Stack<T>::enterScope(){
  Tri C;
  if (tos >= size - 1) {
    cerr << "Stack overflow\n";
    exit(1);
  } else {
    stack[++tos] = C;
  }  
}

template<class T>
T Stack<T>::exitScope()  {
  if (tos < 0) {
    cerr << "Stack underflow\n";
    exit(1);
  } else {
    return stack[tos--];
  }
}

template<class T>
int Stack<T>::addSymbol(string sym)  {
  //bad fix to accept certain variables
  if(true/*!stack[tos].contains(sym)*/){
    stack[tos].put(sym, sym + "$" + itos(tos));
    return 1;
   }
  else{
    return 0;
  }
}

template<class T>
string Stack<T>::getUniqueSymbol(string sym)  {
  for(int x = tos; x >= 0; x--){
 
    if(stack[x].contains(sym)){
      return stack[x].get(sym);
    }
   
  }
  return "";
}
//End public members for parser symbol table 


template<class T>
void Stack<T>::push(T val) {
  if (tos >= size - 1) {
    cerr << "Stack overflow\n";
    exit(1);
  } else {
    stack[++tos] = val;
  }
}

template<class T>
T Stack<T>::pop() {
  if (tos < 0) {
    cerr << "Stack underflow\n";
    exit(1);
  } else {
    return stack[tos--];
  }
}

template<class T>
T Stack<T>::peek() const {
  if (tos < 0) {
    cerr << "Stack underflow\n";
    exit(1);
  } else {
    return stack[tos];
  }
}

//template<class T>

/*
ostream& operator <<(ostream& out, const Stack<int>& s){
  out << "[ ";
  for (int i=0; i <= s.tos; i++) {
    out << s.stack[i] << " ";
  }
  out << "]";  
  return out;
}

*/

template<class T>
void Stack<T>::checkIndex(int i) const {
  if (i < 0 || i > tos) {
    cerr << "Index out of bounds\n";
    exit(1);
  }  
}

template<class T>
T& Stack<T>::operator [](int i){
  checkIndex(i);
  return stack[i];
}

template<class T>
const T Stack<T>::operator [](int i) const {
  checkIndex(i);  
  return stack[i];
}
