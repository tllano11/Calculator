#include <iostream>
#include <sstream>
#include <string>
#include <string.h> // needed for .c_str()
#include "calcex.h"
#include "calculator.h"
#include "scanner.h"
#include "parser.h"

using namespace std;

Calculator* calc;

bool isSpace(char c) {
  return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
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
      delete calc;
      tokenRepository.clear();
    } catch(Exception ex) {
       cout << "*Program aborted due to exception!" << endl;
    }	
}

int main(int argc, char* argv[], char* env[]) {
  string line; 
  cout << "> ";

  while(getline(cin,line)) {
  	
    beginCalculation(line);
    cout << "> ";
  }
  cout << "" << endl;
}
