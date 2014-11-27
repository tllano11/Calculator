#include "parser.h"
#include "calcex.h"
#include "scanner.h"
#include <cstdlib>
#include <sstream>
#include <string>

Parser::Parser(istream* in) {
  scan = new Scanner(in);
}

Parser::~Parser() {
  try {
    delete scan;
  } catch (...) {}
}

AST* Parser::parse() {
  return Prog();
}

AST* Parser::Prog() {
  Token* t = scan->getToken();

  if (t->getType() != eof) {
    scan -> putBackToken();
    AST* result = Stmts(Stmt());
    return result;
  } else {
    cout << "* Parse error" << endl;
    throw ParseException;
  }
}

AST* Parser::Stmts(AST* a) {
  return a;
}

AST* Parser::Stmt() {
  return Expr();
}

AST* Parser::Expr() {
  return RestExpr(Term());
}

AST* Parser::RestExpr(AST* e) {
  Token* t = scan->getToken();

  if (t->getType() == add)
    return RestExpr(new AddNode(e,Term()));

  if (t->getType() == sub)
    return RestExpr(new SubNode(e,Term()));

  scan->putBackToken();
  return e;
}

AST* Parser::Term() {
  return RestTerm(Storable());
}

AST* Parser::RestTerm(AST* e) {
  Token* t = scan->getToken();

  if(t->getType() == times)
    return RestTerm(new TimesNode(e, Term()));

  if(t->getType() == divide)
    return RestTerm(new DivideNode(e, Term()));

  if(t->getType() == mod)
    return RestTerm(new ModNode(e, Term()));

  scan->putBackToken();
  return e;
}

AST* Parser::Storable() {
  AST* result = Factor();
  return MemOperation(result);
}

AST* Parser::MemOperation(AST* result) {
  Token* t = scan -> getToken();

  if (t->getType() == keyword) {
    if (t->getLex().compare("S") == 0) {
      return new StoreNode(result);
    } else {
      if (t->getLex().compare("P") == 0) {
	    return new MemoryPlusNode(result);
      } else {
        if (t->getLex().compare("M") == 0) {
          return new MemoryMinusNode(result);
        } else {
          cout << " * Expected a keyword S, P or M at column: "
               << t->getCol() << endl;
          throw ParseException;
        }
      }
    }
  }

  scan -> putBackToken();
  return result;
}

AST* Parser::Factor() {
  Token* t = scan->getToken();
  if (t->getType() == number) {
    istringstream in(t->getLex());
    int val;
    in >> val;
    return new NumNode(val);
  }

  if (t->getLex().compare("=") == 0) {
    AST* tree = new IdentifierNode(new EqualsNode(Expr()));
    return tree;
  }

  //The following if statement evaluates if the given
  //identifier is a unary node identifier of a
  //leaf node identifier.
  if (t -> getType() == identifier && equalVerify) {
    try {

      //throws exception if t -> getLex() doesn't exist in variables.
      //Remark: using [] is different from using .at
      int val = identifiers.at(t -> getLex());
      return new IdentifierLNode(val);
    } catch(...) {
      string strCalc = "CALCVAR_" + t -> getLex();
      char* envValue = getenv(strCalc.c_str());
      if (envValue != NULL)
        return new IdentifierLNode(atoi(envValue));

      return new IdentifierLNode(0);
    }
  }

  if (t->getType() == keyword) {
    if (t->getLex().compare("R") == 0) {
       return new RecallNode();
    } else if (t -> getLex().compare("C") == 0) {
      return new MemoryClearNode();
    } else {
      cout << " * Expected a keyword R or C at column: "
           << t->getCol() << endl;
      throw ParseException;
    }
  }

  if (t->getType() == lparen) {
    AST* result = Expr();
    t = scan->getToken();
    if(t->getType() == rparen) {
      return result;
    } else {
      cout << " * Expected a ) at column: "
           << t->getCol() << endl;
      throw ParseException;
    }
  }

  cout << " * Expected a number, R or ( at column: "
       << t->getCol() << endl;
  throw ParseException;
}
