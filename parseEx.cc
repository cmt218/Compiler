
//Abides by the same rule to read 1 token past what you are looking at
TreeNode* term() {
  factor();
  while (token.getType() == Token::TIMES || token.getType() == Token::DIVIDE){
    token = lexer.nextToken();
    factor();
  }
}
