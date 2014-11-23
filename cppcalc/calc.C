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

bool isSpace(char c) {
  return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

void argVaribleInsert(string s) {
  char charArg[s.size() + 1];
  strcpy(charArg, s.c_str());
  int length = sizeof(charArg) / sizeof(char); // Include \n

  int colCounter = 0;
  for(int i = 0; i < length - 1; i++) {
    if(charArg[i] != '=') colCounter++;
      else break;
  }

  // The following 4 lines are used to isolate the identifier,
  // that precedes the '=' char, from the given expression.
  char charIdentifier[colCounter + 1];
  for(int i = 0; i < colCounter; i++) {
    charIdentifier[i] = charArg[i];
  }

  // The following 3 lines are used to isolate the number,
  // that follows the '=' char, from the given expression.
  string strValue;
  for(int i = colCounter + 1; i < length; i++)
    strValue += charArg[i];
  
  //The following 2 lines are used to insert the number given
  //in the expression in the identifiers map.
  //Remark: atoi function convert a c string into an int if
  //it's possible.  
  int b = atoi(strValue.c_str());
  identifiers[charIdentifier] = b;
}

//endsComparison compares if str ends with suffix.
bool endsComparison(string str, string suffix) {
  return str.rfind(suffix) == (str.size() - suffix.size());
}

void beginCalculation(string line){
  try {
      calc = new Calculator();
      int result = calc->eval(line);

      char charArray[line.size()+1]; // +1 space for the null char.
      strcpy(charArray, line.c_str());
      int length = sizeof(charArray) / sizeof(char);
        int counter = 0;
      for(int i = 0; i < length - 1; i++) { // -1 for the null char.
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
    } catch(Exception ex) {
       cout << "* Program aborted due to exception!" << endl;
    }
}

//commandVerify identifies if a variable is defined
//in the command line. If it is, commandVerify inserts 
//it in the identifiers map.
void commandVerify(int i,char* argv[]){
  string arg = argv[i];
  
  if(arg.compare("-v") == 0) argVaribleInsert(argv[i+1]);
  if (endsComparison(arg, ".calc")) {
    ifstream input(arg.c_str());
    string inputLine;
    while (getline(input, inputLine)){
      beginCalculation(inputLine);
    }
  }
}

int main(int argc, char* argv[], char* env[]) {

  string line;
 
  for(int i = 0; i < argc; i++) {

    try {
      commandVerify(i,argv);
    } catch(...) {
      cout << "* Invalid argument." << endl;
      return 0; 
    }
  }

  cout << "> ";

  while(getline(cin,line)) {
  	
    beginCalculation(line);
    cout << "> ";
  }
  cout << "" << endl;
}
