#ifndef __ERROR_HH__
#define __ERROR_HH__

#include <iostream>
#include <sstream>
#include <ostream>

using namespace std;


/*** This file contains some error and debug output routines common to all
     classes and files. Breaking the OO paradigm for the sake of convenience...
     So sue me. ***/

// Defined in error.cc.
extern int error_count;

// Defined in scanner.cc (the generated file)
extern int yylineno;

/* This class contains (starting) line and column of a token, and is used to
   report the positions of errors in the code. */
class position_information
{
private:
    int line;

    int column;

public:
    position_information();

    position_information(int l, int c);

    int get_line();

    int get_column();
};


/* Various methods for printing things, with or without position info.
   They are all defined for real in error.cc. */

// Prints message, aborts compiling.
extern void fatal(string);

// This must be defined, but using error(pos) << "foo" is preferrable.
extern void yyerror(string);

extern ostream  &error(string header = "Error: ");

extern ostream  &error(position_information *);

extern ostream  &type_error();

extern ostream  &type_error(position_information *);

extern ostream  &debug(string header = "Debug: ");

extern ostream  &debug(position_information *);


#endif
