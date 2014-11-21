#ifndef parser_h
#define parser_h

#include "ast.h"
#include "scanner.h"
#include <map>

class Parser {
  public:
    Parser(istream* in);
    ~Parser();
    AST* parse();

  private:
    AST* Prog();
    AST* Stmts(AST* a, Token* t);
    AST* Stmt();
    AST* Expr();
    AST* RestExpr(AST* e);
    AST* Term();
    AST* RestTerm(AST* t);
    AST* Storable();
    AST* MemOperation(AST* r);
    AST* Factor();

    Scanner* scan;
};

extern map<string, int> identifiers;

#endif
