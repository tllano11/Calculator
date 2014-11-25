#ifndef ast_h
#define ast_h

#include <fstream>

using namespace std;

class AST {
  public:
    AST();
    virtual ~AST() = 0;
    virtual int evaluate() = 0;
};

class BinaryNode : public AST {
  public:
    BinaryNode(AST* left, AST* right);
    ~BinaryNode();

    AST* getLeftSubTree() const;
    AST* getRightSubTree() const;

  private:
    AST* leftTree;
    AST* rightTree;
};

class UnaryNode : public AST {
  public:
    UnaryNode(AST* sub);
    ~UnaryNode();

    AST* getSubTree() const;

  private:
    AST* subTree;
};

class AddNode : public BinaryNode {
  public:
    AddNode(AST* left, AST* right);
    int evaluate();
};

class SubNode : public BinaryNode {
  public:
    SubNode(AST* left, AST* right);
    int evaluate();
};

class TimesNode : public BinaryNode {
  public:
    TimesNode(AST* left, AST* right);
    int evaluate();
};

class DivideNode : public BinaryNode {
  public:
    DivideNode(AST* left, AST* right);
    int evaluate();
};

class ModNode : public BinaryNode {
  public:
    ModNode(AST* left, AST* right);
    int evaluate();
};

class StmtNode : public BinaryNode {
  public:
    StmtNode(AST* left, AST* right);
    int evaluate();
};

class NumNode : public AST {
  public:
    NumNode(int n);
    int evaluate();

  private:
    int val;
};

class StoreNode : public UnaryNode {
  public:
    StoreNode(AST *sub);
    ~StoreNode();
    int evaluate();
};

//The following class(P) is used to add a
//value to memory and then return the
//the new value obtained from the operation.
class MemoryPlusNode : public UnaryNode {
  public:
    MemoryPlusNode(AST* sub);
    int evaluate();
};

//The following class(M) is used to rest a
//value from memory and then return the
//the new value obtained from the operation.
class MemoryMinusNode : public UnaryNode {
  public:
    MemoryMinusNode(AST* sub);
    int evaluate();
};

class RecallNode : public AST {
  public:
    RecallNode();
    int evaluate();
};

//The following class(C) is used to clear
//memory.
class MemoryClearNode : public AST {
  public:
    MemoryClearNode();
    int evaluate();
};

class IdentifierNode : public UnaryNode {
  public:
    IdentifierNode(AST* sub);
    int evaluate();
};

//IdentifierLNode is used to differentiate a unary
//node identifier from a leaf node identifier.
//Remark: If an identifier follows a '=' char,
//then it is a leaf node identifier.
 class IdentifierLNode : public AST {
  public:
    IdentifierLNode(int n);
    int evaluate();

  private:
    int val;
};

class EqualsNode : public UnaryNode {
  public:
    EqualsNode(AST* sub);
    int evaluate();
};

extern bool eweCompiler;
extern ofstream output;

#endif
