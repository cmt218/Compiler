cout << "pop rbx" << endl;
cout << "pop rax" << endl;
cout << "add rax,rbx" << endl;
cout << "push rax" << endl;
#include "token.h"
#include "lexer.h"
#include <iostream>
#include <fstream>

using namespace std;

void processFile(istream& in) {
  Lexer lexer(in);
  Token token;
  
  token = lexer.nextToken();
  printf(" Type     Lexeme        Line #    Pos\n");
  while (token.getType() != 30) {
    printf("%5d     %-14s %5d  %5d\n", token.getType(), token.getLexeme().c_str(), token.getLine(), token.getPos());
    token = lexer.nextToken();
  }
}


int main(int argc, char **argv) {
  ifstream in;
  
  if (argc > 1) {
    in.open(argv[1]);
    processFile(in);
    in.close();
  } else {
    processFile(cin);
  }
  
}
