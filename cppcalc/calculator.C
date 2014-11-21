#include "calculator.h"
#include "parser.h"
#include "ast.h"
#include <string>
#include <iostream>
#include <sstream>

//In the following line a map is defined.
//identifiers is used to store the results 
//of every operation.
//Remark: In this case, the "mapkeys" are
//identifiers.
map<string, int> identifiers;

//In the following line a vector is defined.
//tokenRepository is used to store every token 
//from a given expression.
vector<char> tokenRepository;

Calculator::Calculator():
  memory(0)
{}

int Calculator::eval(string expr) {
  Parser* parser = new Parser(new istringstream(expr));
  AST* tree = parser->parse();
  int result = tree->evaluate();
  delete tree;
  delete parser;
  return result;
}

void Calculator::store(int val) {
  memory = val;
}

int Calculator::recall() {
  return memory;
}
