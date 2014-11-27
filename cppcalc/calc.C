#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h> // needed for .c_str()
#include <string>
#include <stdlib.h>
#include "calcex.h"
#include "calculator.h"
#include "scanner.h"
#include "parser.h"

using namespace std;

Calculator* calc;
bool eweCompiler;
int compilerCount;
ofstream output;

bool isSpace(char c) {
  return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

//ardVaribleInsert is used to take a given string and
//separate it into substrings. Those substrings are
//checked according with the command line - initVar syntax
//and used to initialize a variable(identifier).
void argVaribleInsert(string s) {
  char charArg[s.size() + 1];
  strcpy(charArg, s.c_str());
  int length = sizeof(charArg) / sizeof(char); //Includes '\n' char

  int colCounter = 0;
  for(int i = 0; i < length - 1; i++) {
    if(charArg[i] != '=') colCounter++;
    else break;
    if(i == length - 2)
      throw ArgsException;
  }

  //The following 8 lines are used to isolate the identifier,
  //that precedes the '=' char, from the given expression.
  char charIdentifier[colCounter + 1];
  if(charArg[0] != '='){
    for(int i = 0; i < colCounter; i++) {
      if(charArg[i]>='a' and charArg[i]<='z')
        charIdentifier[i] = charArg[i];
      else throw ArgsException;
    }
  }else throw ArgsException;

  //The following 7 lines are used to isolate the number,
  //that follows the '=' char, from the given expression.
  string strValue;
  if(int i = colCounter + 1 < length-1){
    for(int i = colCounter + 1; i < length-1; i++)
      if(charArg[i]>='0' and charArg[i]<='9')
        strValue += charArg[i];
      else throw ArgsException;
  }else throw ArgsException;

  //The following 2 lines are used to insert the number given
  //in the expression in the identifiers map.
  //Remark: atoi function converts a c string into an int if
  //it's possible.
  int b = atoi(strValue.c_str());
  identifiers[charIdentifier] = b;
}

//endsComparison compares if str ends with suffix.
bool endsComparison(string str, string suffix) {
  return str.rfind(suffix) == (str.size() - suffix.size());
}

//beginCalculation starts all the calculation process.
void beginCalculation(string line){
  try {
    if(line.length() == 0) throw ScannerException;
    calc = new Calculator();
    int result = calc->eval(line);

    char charArray[line.size()+1]; //Includes the '\0' char (+1)
    strcpy(charArray, line.c_str());
    int length = sizeof(charArray) / sizeof(char);
    int counter = 0;

    for(int i = 0; i < length - 1; i++){ //Doesn't include the '\0' char (-1)
      if (charArray[i] == ';') { counter = i + 1; break; }
    }

    for(int i = counter; i < length - 1; i++) {
      if (!isSpace(charArray[i])) throw ScannerException;
    }

    char verifyChar = tokenRepository[tokenRepository.size() - 1];
    if ((verifyChar >= 'a' && verifyChar <= 'z') ||
        (verifyChar >= 'A' && verifyChar <= 'Z'))
      throw ScannerException;
    if (verifyChar >= '0' && verifyChar <= '9')
      throw ScannerException;

    cout << "= " << tokenRepository[0] << " <- " << result << endl;
    tokenRepository.clear();

    delete calc;

  }catch(Exception ex){
    tokenRepository.clear();
    cout << " * Invalid expression" << endl;
  }
}

//initCompiler turns eweCompiler true and allows the program
//to work as a compiler.
void initCompiler(string line, int lineCounter){
  try{
    if(line.length() == 0) throw ScannerException;
    if(eweCompiler != true) eweCompiler = true;
    output << "# Expression: " << line +"\n\n";
    if(lineCounter == 0){
      output << "start:\n\n";
      output << "# Instructions before evaluating the AST\n";
    }else{
      output << "expr"<<(lineCounter+1)<<":\n";
      output << "# Instructions before evaluating the AST from expr"
         <<(lineCounter+1)<<'\n';
    }
    output << "sp:= 1000\n";
    output << "one:= 1\n";
    output << "zero:= 0\n";
    output << "memory:= zero\n";

    if(identifiers.size() != 0){

      //The following 4 lines are used to check every map key,
      //and the values labeled by them, and write them into
      //a.ewe
      map<string, int>::iterator i;
      for(i=identifiers.begin(); i!=identifiers.end();++i){
        output << (*i).first << ":= " << (*i).second << '\n';
      }
    }
    output << '\n';
    beginCalculation(line);
  }catch(...){
      tokenRepository.clear();
      cout << " * Invalid expression" << endl;
  }
}

//initFileCompiler creates, or superscribes, a .ewe file
//which name is the same as a given .calc file.
void initFileCompiler(string file, int lineCounter){
  string eweFile = file.substr(0,(file.length()-5));
  eweFile = eweFile + ".ewe";

  output.open(eweFile.c_str());
  ifstream inputE(file.c_str());
  string inputLineE;
  while (getline(inputE, inputLineE)){
    initCompiler(inputLineE,lineCounter);
    lineCounter++;
  }
}

//commandVerify checks the command line syntax.
void commandVerify(int &i,char* argv[],int argc){
  string arg = argv[i];
  string line;
  int lineCounter = 0;
  int compilerCount = 0;
  bool vUsed = false;

  //The following condition identifies if a variable is defined
  //in the command line.
  if(arg.compare("-v") == 0) argVaribleInsert(argv[++i]);

  //The following condition checks if the user is trying to
  //execute a single .calc file or multiple .calc files.
  //Remark: If an argument, different from a .calc file, is
  //found after a .calc file the program will stop and show
  //an exception.
  else if(endsComparison(arg, ".calc") and arg.size()>5){
    int aux = i;
    string verify;
    while(aux < argc){
      verify = argv[aux++];
      if(!endsComparison(verify, ".calc")) throw ArgsException;
    }
    ifstream input(arg.c_str());
    string inputLine;
    while (getline(input, inputLine)){
      beginCalculation(inputLine);
    }
    if(i == argc-1) exit(0);
  }

  //The following condition checks if the user wants to
  //generate a .ewe file using the ewe compiler function
  //of the program.
  else if(arg.compare("-c") == 0 and compilerCount < 1){
    int aux = i;
    compilerCount++;
    if(i<(argc-1) and endsComparison(argv[++aux], ".calc")){

      while(aux < argc){
        if(!endsComparison(argv[aux], ".calc")) throw ArgsException;
        aux++;
      }
      aux = i+1;
      while(aux < argc){
        i++;
        initFileCompiler(argv[aux++],lineCounter);

        output << "end: halt\n\n";
        output << "equ memory M[0]\n";
        output << "equ one M[1]\n";
        output << "equ zero M[2]\n";
        output << "equ operator1 M[3]\n";
        output << "equ operator2 M[4]\n";
        output << "equ sp M[5]\n";

        map<string, int>::iterator itr;
        int j = 6;

        //The following loop checks every identifier stored in the
        //identifiers map.
        //Those identifiers are translated into ewe code as
        //instructions to allocate space in memory.
        for(itr=identifiers.begin(); itr!=identifiers.end();++itr){
          output << "equ "  << (*itr).first << " M["<< j << "]\n";
          j++;
        }
        output << "equ stack M[1000]\n";
        output.close();
      }
      exit(0);
    }else if(i!= argc-1) throw ArgsException;
      output.open("a.ewe");
      cout << "> ";
      while(getline(cin,line)){
        initCompiler(line,lineCounter);
        lineCounter++;
        cout << "> ";
      }

      output << "end: halt\n\n";
      output << "equ memory M[0]\n";
      output << "equ one M[1]\n";
      output << "equ zero M[2]\n";
      output << "equ operator1 M[3]\n";
      output << "equ operator2 M[4]\n";
      output << "equ sp M[5]\n";

      map<string, int>::iterator i;
      int j = 6;
      for(i=identifiers.begin(); i!=identifiers.end();++i){
        output << "equ "  << (*i).first << " M["<< j << "]\n";
        j++;
      }

      output << "equ stack M[1000]\n";
      output.close();
      if(EOF){
        cout << endl;
        exit(0);
      }
    }else if (i == argc - 1 and compilerCount > 1)
      throw ArgsException;

  else throw ArgsException;
}

int main(int argc, char* argv[], char* env[]) {

  string line;
  eweCompiler = false;
  compilerCount = 0;

  //The following "for loop" starts evaluating
  //the command line arguments.
  //Remark: "./calc" is the first argument and
  //this loop doesn't count it (i=1).
  for(int i = 1; i < argc; i++) {
    try {
      commandVerify(i,argv,argc);
    } catch(...) {
      cout << " * Invalid argument." << endl;
      return 0;
    }
  }
  output.close();
  if(eweCompiler == false){
    cout << "> ";
    while(getline(cin,line)) {
    beginCalculation(line);
    cout << "> ";
    }
  }
  cout << "" << endl;

}
