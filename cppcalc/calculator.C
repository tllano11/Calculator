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

  //Variable allocation
  string key(1, tokenRepository[0]);
  identifiers[key] = result;
  if(eweCompiler){
    output << "# Assign\n";
    output << key << ":= M[sp+0]\n\n";

    output << "# Write result\n";
    output << "operator1 := M[sp+0]\n";
    output << "sp := sp - one\n";
    output << "writeInt(operator1)\n\n";
  }

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
