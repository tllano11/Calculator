#ifndef evalex_h
#define evalex_h

enum Exception {
  UnrecognizedToken, // Token in object file not recognized
  ParseException,
  ScannerException,
  ArgsException
};

#endif
