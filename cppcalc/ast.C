#include "ast.h"
#include <iostream>
#include "calculator.h"

// For debug information uncomment
// #define debug

AST::AST() {}

AST::~AST() {}

BinaryNode::BinaryNode(AST* left, AST* right):
  AST(),
  leftTree(left),
  rightTree(right)
{}

BinaryNode::~BinaryNode() {
  #ifdef debug
    cout << "In BinaryNode destructor" << endl;
  #endif

  try {
    delete leftTree;
  } catch (...) {}

  try {
    delete rightTree;
  } catch(...) {}
}

AST* BinaryNode::getLeftSubTree() const {
  return leftTree;
}

AST* BinaryNode::getRightSubTree() const {
  return rightTree;
}

UnaryNode::UnaryNode(AST* sub):
  AST(),
  subTree(sub)
{}

UnaryNode::~UnaryNode() {
  #ifdef debug
     cout << "In UnaryNode destructor" << endl;
  #endif

  try {
    delete subTree;
  } catch (...) {}
}

AST* UnaryNode::getSubTree() const {
  return subTree;
}

AddNode::AddNode(AST* left, AST* right):
  BinaryNode(left,right)
{}

int AddNode::evaluate() {
  int result = getLeftSubTree()->evaluate() + getRightSubTree()->evaluate();
  if(eweCompiler){
    output<<"# Add\n";
    output<<"operator2 := M[sp+0]\n";
    output<<"operator1 := M[sp+1]\n";
    output<<"operator1 := operator1 + operator2\n";
    output<<"sp := sp + one\n";
    output<<"M[sp+0]:= operator1\n\n";
  }	
  return result;
}

SubNode::SubNode(AST* left, AST* right):
  BinaryNode(left,right)
{}

int SubNode::evaluate() {
  int result = getLeftSubTree()->evaluate() - getRightSubTree()->evaluate();
  if(eweCompiler){
    output<<"# Sub\n";
    output<<"operator2 := M[sp+0]\n";
    output<<"operator1 := M[sp+1]\n";
    output<<"operator1 := operator1 - operator2\n";
    output<<"sp := sp + one\n";
    output<<"M[sp+0]:= operator1\n\n";
  }	
  return result;
}

TimesNode::TimesNode(AST* left, AST* right):
  BinaryNode(left,right)
{}

int TimesNode::evaluate() {
  int result = getLeftSubTree()->evaluate() * getRightSubTree()->evaluate();
  if(eweCompiler){
    output<<"# Times\n";
    output<<"operator2 := M[sp+0]\n";
    output<<"operator1 := M[sp+1]\n";
    output<<"operator1 := operator1 * operator2\n";
    output<<"sp := sp + one\n";
    output<<"M[sp+0]:= operator1\n\n";
  }	
  return result;
}

DivideNode::DivideNode(AST* left, AST* right):
  BinaryNode(left,right)
{}

int DivideNode::evaluate() {
  int result = getLeftSubTree()->evaluate() / getRightSubTree()->evaluate();
  if(eweCompiler){
    output<<"# Divide\n";
    output<<"operator2 := M[sp+0]\n";
    output<<"operator1 := M[sp+1]\n";
    output<<"operator1 := operator1 / operator2\n";
    output<<"sp := sp + one\n";
    output<<"M[sp+0]:= operator1\n\n";
  }	
  return result;
}

ModNode::ModNode(AST* left, AST* right):
  BinaryNode(left,right)
{}

int ModNode::evaluate() {
  int result = getLeftSubTree()->evaluate() % getRightSubTree()->evaluate();
  if(eweCompiler){
    output<<"# Mod\n";
    output<<"operator2 := M[sp+0]\n";
    output<<"operator1 := M[sp+1]\n";
    output<<"operator1 := operator1"<<'%'<<"operator2\n";
    output<<"sp := sp + one\n";
    output<<"M[sp+0]:= operator1\n\n";
  }	
  return result;
}

StmtNode::StmtNode(AST* left, AST* right):
  BinaryNode(left,right)
{}

int StmtNode::evaluate() {
  return 0;
}

StoreNode::StoreNode(AST* sub) :
  UnaryNode(sub)
{}

StoreNode::~StoreNode() {}

int StoreNode::evaluate() {
  calc->store(getSubTree()->evaluate());
  if(eweCompiler){
    output << "# Store\n";
    output << "memory := M[sp+0]\n\n";
  }
  return calc->recall();
}

MemoryPlusNode::MemoryPlusNode(AST* sub) :
  UnaryNode(sub)
{}

//In the following method the subtree evaluation is
//done before calling the "recall" method.
//This procedure is followed to ensure that all
//memOperations found at the subtree work properly.
int MemoryPlusNode::evaluate() {
  int val = getSubTree() -> evaluate();
  int mem = calc -> recall();
  int result = mem + val;
  if(eweCompiler){
    output << "# Memory Plus\n";
    output << "operator2 := M[sp+0]\n";
    output << "memory := memory + operator2\n";
    output << "M[sp+0] := memory\n\n";
  }
  calc -> store(result);
  return result;
}

MemoryMinusNode::MemoryMinusNode(AST* sub) :
  UnaryNode(sub)
{}

//In the following method the subtree evaluation is
//done before calling the "recall" method.
//This procedure is followed to ensure that all
//memOperations found at the subtree work properly.
int MemoryMinusNode::evaluate() {
  int val = getSubTree() -> evaluate();
  int mem = calc -> recall();
  int result = mem - val;
  if(eweCompiler){
    output << "# Memory Minus\n";
    output << "operator2 := M[sp+0]\n";
    output << "memory := memory - operator2\n";
    output << "M[sp+0] := memory\n\n";
  }
  calc -> store(result);
  return result;
}

RecallNode::RecallNode() : AST() {}

int RecallNode::evaluate() {
  if(eweCompiler){
    output <<"# Recall\n";
    output << "sp := sp - one\n";
    output << "M[sp+0] := memory\n\n";
  }	
  return calc->recall();
}

MemoryClearNode::MemoryClearNode() : AST() {}

int MemoryClearNode::evaluate() {
  if(eweCompiler){
    output << "# Memory Clear\n";
    output << "memory := zero\n";
    output << "sp := sp - one\n";
    output << "M[sp+0] := memory\n\n";
  }
  calc -> store(0);
  return 0;
}

IdentifierNode::IdentifierNode(AST* sub) :
  UnaryNode(sub)
{}

int IdentifierNode::evaluate() {
  return getSubTree() -> evaluate();
}

IdentifierLNode::IdentifierLNode(int n) :
  AST(),
  val(n)
{}

int IdentifierLNode::evaluate() {
  if(eweCompiler){
    output << "# Push("<<val<<")\n";
    output << "sp:= sp - one\n";
    output << "operator1:= "<<val<<'\n';
    output << "M[sp+0]:= operator1\n\n";
  }
  return val;
}

EqualsNode::EqualsNode(AST* sub) :
  UnaryNode(sub)
{}

int EqualsNode::evaluate() {
  return getSubTree() -> evaluate();
}

NumNode::NumNode(int n) :
  AST(),
  val(n)
{}

int NumNode::evaluate() {
  if(eweCompiler){
    output << "# Push("<<val<<")\n";
    output << "sp:= sp - one\n";
    output << "operator1:= "<<val<<'\n';
    output << "M[sp+0]:= operator1\n\n";
  }
  return val;
}
