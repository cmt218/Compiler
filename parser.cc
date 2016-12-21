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

		      "LABEL", "SEQ", "FUNC" };


Parser::Parser(Lexer& lexerx, ostream& outx): lexer(lexerx), out(outx), lindex(1), tindex(1), table(Stack<Tri>(15)){
  //token = lexer.nextToken();
}

Parser::~Parser() {
}

void Parser::getinst(TreeNode *node){
  //Post order traversal of generated tree
  if(node == NULL){
    return;
  }
  getinst(node->leftChild);
  getinst(node->rightChild);
  if(node->op != 23){ 
    nodeCode(node);
  }
}

void Parser::emit(string str){
  cout << str << endl;
}

//Declarations as directed
string fmt;
int nparams;
string currentFunc;
int nfmts = 0;
string fmts[100];

//Generate assembly code dependent on operation type
//Most logic courtesy of J Femister
void Parser::nodeCode(TreeNode *node){

  switch(node->op){
  case 0 ://add
    //emit("add");
    cout << "  pop rbx" << endl;
    cout << "  pop rax" << endl;
    cout << "  add rax,rbx" << endl;
    cout << "  push rax" << endl;
    break;

  case 1://sub
    //emit("sub");
    cout << "  pop rbx" << endl;
    cout << "  pop rax" << endl;
    cout << "  sub rax,rbx" << endl;
    cout << "  push rax" << endl;
    break;
    
  case 2://mult
    //emit("mult");
    cout << "  pop rbx" << endl;
    cout << "  pop rax" << endl;
    cout << "  imul rbx" << endl;
    cout << "  push rax" << endl;
    break;

  case 3://div
    //emit("div");
    cout << "  mov rdx,0" << endl;
    cout << "  pop rbx" << endl;
    cout << "  pop rax" << endl;
    cout << "  idiv rbx" << endl;
    cout << "  push rax" << endl;
    break;
    
  case 4://iseq
    //emit("iseq");
    compare("je");
    break;

  case 5://isne
    //emit("isne");
    compare("jne");
    break;

  case 6://islt
    //emit("islt");
    compare("jl");
    break;

  case 7://isle
    //emit("isle");
    compare("jle");
    break;

  case 8://isgt
    //emit("isgt");
    compare("jg");
    break;

  case 9://isge
    //emit("isge");
    compare("jge");
    break;

  case 10://and
    //emit("and");
    cout << "  pop rbx" << endl;
    cout << "  pop rax" << endl;
    cout << "  and rax,rbx" << endl;
    cout << "  push rax" << endl;
    break;
    
  case 11://or
    //emit("or");
    cout << "  pop rbx" << endl;
    cout << "  pop rax" << endl;
    cout << "  or rax,rbx" << endl;
    cout << "  push rax" << endl;
    break;

  case 12://pushl
    //emit("pushl");    
    cout << "  mov rax," << node->val << endl;
    cout << "  push rax" << endl;
    break;
    
  case 13://pushv
    //emit("pushv");
    if(isUnique(node->val)){
      cout << "  push" << "  qword[" << node->val << "]" << endl;

    }
    
    break;
    
  case 14://store
    //emit("store");
    cout << "  pop" << "  qword[" << node->val << "]" << endl;
    break;
    
  case 15://jump
    //emit("jump");
    cout << "  jmp " << node->val << endl;
    break;
    
  case 16://jumpf
    //emit("jumpf");
    cout << "  pop rax" << endl;
    cout << "  cmp rax, 0" << endl;
    cout << "  je " << node->val << endl;
    break;

  case 17://jumpt
    //emit("jumpt");
    cout << "  pop rax" << endl;
    cout << "  cmp rax,0" << endl;
    cout << "  jne " << node->val << endl;
    break;

  case 18://call
    //emit("call");
    cout << "  call  " << node->val << endl;
    cout << "  push  " << "rax" << endl;
    break;
    
  case 19://ret
    //emit("ret");
    emit("  pop   rax");
    if(currentFunc != "main:"){
      emit("  push   r15");
    }
    emit("  ret");
    break;
    
    

  case 20://printf
    //emit("printf");
    fmt = node->val;
    nparams = fmt.at(0) - '0';
    fmt = "`" + fmt.substr(1) + "`";
    fmts[nfmts++] = fmt;
    emit("  mov   rdi,fmt" +itos(nfmts));
    if(nparams == 5){
      emit("  pop   r9");
      --nparams;
    }
    if(nparams == 4){
      emit("  pop   r8");
      --nparams;
    }
    if(nparams == 3){
      emit("  pop   rcx");
      --nparams;
    }
    if(nparams == 2){
      emit("  pop   rdx");
      --nparams;
    }
    if(nparams == 1){
      emit("  pop   rsi");
    }
    emit("  mov   rax,0");
    emit("  push  rbp");
    emit("  call  printf");
    emit("  pop   rbp");
    break;


  case 22://label
    //emit("label")
    cout << node->val << endl;
    break;

  case 23://seq
    break;

  case 24://func
    //emit("func");
    currentFunc = node->val;
    emit(currentFunc);
    if(currentFunc != "main:"){
      emit("  pop   r15");
    }
    break;
    
  default:
    cout << "default: " <<  ops[node->op] << endl;    
  }

}

bool Parser::isUnique(string val){
  bool unique = false;
  int len = val.length();
  for(int x=0;x<len;x++){
    if(val[x] == '$'){
      unique = true;
      break;
    }
  }
  return unique;
}

//Makes unique labels for relational expressions
string Parser::makeJ() {
  string tmp = "J";
  stringstream ss;
  ss << ++lindex;
  string res = ss.str();
  tmp = tmp + res;
  return tmp;
}

void Parser::compare(string comp){
  string j1 = makeJ();
  string j2 = makeJ();
  
  cout << "  pop rbx" << endl;
  cout << "  pop rax" << endl;
  cout << "  cmp rax,rbx" << endl;
  cout << "  " << comp << " " << j1 << endl;
  cout << "  mov rax,0" << endl;
  cout << "  jmp " << j2 << endl;
  cout << j1 << ":" << endl;
  cout << "  mov rax,1" << endl;
  cout << j2 << ":" << endl;
  cout << "  push rax" << endl;
}

//Data structure and method for collecting all unique variables
Stack<string> vars(20);
int varct = 0;
void Parser::vardefs(TreeNode *node){
  if(node == NULL){
    return;
  }
  vardefs(node->leftChild);
  vardefs(node->rightChild);
  if(node->op == 13 || node->op == 14){
    bool dupe = false;
    for(int x=0;x<varct;x++){
      if(vars[x] == node->val){
	dupe = true;
	break;
      }
    }
    if(!dupe){
      vars.push(node->val);
      varct++;
    }
  }
}


void Parser::genasm(TreeNode *node){
  cout << "  global main" << endl;
  cout << "  extern printf" << endl;
  emit("");
  cout << "  segment .bss" << endl;
  vardefs(node);
  while(!vars.isEmpty()){
    string check = vars.pop();
    if(isUnique(check)){
	cout << "  " << check << " resq 1" << endl;
      }
      
  }
  emit("");
  emit("  section .text");
  getinst(node);

  emit("");
  out << "  section .data" << endl;
  for(int i=0; i<nfmts; ++i){
    cout << "  fmt" << i+1 << ": db " << fmts[i] << ", 0" << endl;
  }
  cout << "  fmt:  db '%ld  ', 0" << endl;
  cout << "  endl: db 10, 0" << endl;
  
}

void Parser::error(string message) {
  cerr << message << " Found " << token.getLexeme() << " at line " << token.getLine() << " position " << token.getPos() << endl;
  exit(1);
}

void Parser::check(int tokenType, string message) {
  if (token.getType() != tokenType)
    error(message);
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
	  //MIGHT NOT NEED THIS
	  //table.addSymbol(token.getLexeme());
	  R = new TreeNode(PUSHV, token.getLexeme());
	  R = expression();
	  exps.push(R);
	}

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
	return R;
      }
      else{
	return C;
      }      
    }
    //Identifier case 
    else{
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

  return SEQ5;
}

Parser:: TreeNode* Parser::assignStatement(){
  TreeNode* a;

  //Assigns a literal to a variable 
  if(token.getType() == 1){
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
  //Printf case
  else if(token.getType() == 28){
    s = printfStatement();
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

//Removes quotation characters from format string  
string Parser::remQuote(string fmt){
  string ret = "";
  int len = fmt.length();
  for(int y=0;y<len;y++){
    if(fmt[y] == '"'){
      continue;
    }
    else{
      ret+=fmt[y];
    }
  }
  return ret;
}

//Function logic courtesy of J Femister
Parser:: TreeNode* Parser::printfStatement(){
  TreeNode* paramList = NULL;
  int nparams = 0;
  check(28, "Expecting printf");
  token = lexer.nextToken();
  check(17, "Expecting (");
  token = lexer.nextToken();
  check(3, "Expecting string literal");
  string formatString = token.getLexeme();
  formatString = remQuote(formatString);
  token = lexer.nextToken();
  if(token.getType() == 21){
    token = lexer.nextToken();
    paramList = expression();
    ++nparams;
    while(token.getType() == 21){
      token = lexer.nextToken();
      paramList = new TreeNode(SEQ, paramList, expression());
      ++nparams;
    }
  }
  check(18, "Expecting )");
  token = lexer.nextToken();
  check(22, "Expecting ;");
  token = lexer.nextToken();
  TreeNode* printStatement = new TreeNode(SEQ, paramList, new TreeNode(PRINTF, itos(nparams) + formatString));
  return printStatement;
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
    string fun = token.getLexeme() + ":";
    F = new TreeNode(FUNC, fun);
      TreeNode* R;
      TreeNode* B;
      Stack<TreeNode*> exps;
      //Gets to token after opening paren
      token = lexer.nextToken();
      token = lexer.nextToken();
      bool pCheck = false;
      //Pushes parameters to stack if they exist

      //ENTERS SCOPE WHEN STORING PARAMS
      table.enterScope();
      
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

	  //made STORE instead of PUSHV
	  R = new TreeNode(STORE, token.getLexeme());
	  R = expression();

	  //This might break something
	  R = new TreeNode(STORE, R->val);
	  exps.push(R);
	}

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
	//changed from R,F to F,R
	R = new TreeNode(SEQ, F, R);
      }
      token = lexer.nextToken();
      B = block();

      //EXITS SCOPE AFTER BLOCK
      table.exitScope();

      
      //Returns root of the funcall
      if(pCheck){
	R = new TreeNode(SEQ, R, B);
	return R;
      }
      else{
	F = new TreeNode(SEQ, F, B);
	return F;
      }      
		     
  }

  return NULL;
}

Parser:: TreeNode* Parser::compilationunit(){
  token = lexer.nextToken();
  table.enterScope();
  TreeNode* C;
  Stack<TreeNode*> funcs(10);
  while(token.getType() != 30){
    C = function();
    funcs.push(C);
  }

  C = new TreeNode(SEQ, NULL, funcs.pop());
  if(!funcs.isEmpty()){
    C = new TreeNode(SEQ, funcs.pop(), C->rightChild);
  }
  while(!funcs.isEmpty()){
    C = new TreeNode(SEQ, C, funcs.pop());
  }
  return C;
}

