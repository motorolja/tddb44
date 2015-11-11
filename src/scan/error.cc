/* Some global error routines. NOTE: Solve this in a better way later. */

#include <cstdlib>

#include "error.hh"

/* Since we are using bison, one might think that this home-made error counter
   isn't really necessary - bison provides the yynerrs variable which counts
   errors, right? - Yes, but we also want to keep track of semantic errors
   and the like, which bison can't detect. */
int error_count = 0;


/* General error outstream. */
ostream &error(string header)
{
    error_count++;
    return cerr << header;
}


/* Error outstream with position information given. */
ostream &error(position_information *pos)
{
    return error("Error") << " line " << pos->get_line()
           << ", col " << pos->get_column() << ": ";
}


/* Abort program with error message. */
void fatal(string msg)
{
    error() << "Fatal: " << msg << endl << flush;
    abort();
}


/* Used for parser errors. Bison uses this for parse errors not caught by
   the grammar, so it's useful to at least include the line number. Since
   the error is not one we've accounted for, we don't have access to any
   position_information. NOTE: Fix scanner.l so it catches weird syntax? */
void yyerror(string msg)
{
    error() << "line " << yylineno << ": " << msg << endl << flush;
}

/* Type conflict error outstream. */
ostream &type_error()
{
    return error("Type conflict: ");
}


/* Same as above, but with position information given as well. */
ostream &type_error(position_information *pos)
{
    if (pos == NULL) {
        return type_error();
    }
    return error("Type conflict, line ") << pos->get_line()
           << ", col " << pos->get_column()
           << ": ";
}


/* General trace print function, used for debugging. */
ostream &debug(string header)
{
    return cerr << header;
}


/* General trace print function, used for debugging. */
ostream &debug(position_information *pos)
{
    if (pos == NULL) {
        return debug();
    }
    return debug("Debug") << " (line " << pos->get_line()
           << ", col " << pos->get_column() << "): ";
}



/*** Function bodies for the position_information class. ***/

/* Default constructor for position information. */
position_information::position_information()
{
    line = 0;
    column = 0;
}


/* Constructor for position information with positions given. */
position_information::position_information(int l, int c)
{
    line = l;
    column = c;
}


/* Get the line number. */
int position_information::get_line()
{
    return line;
}


/* Get the column number. */
int position_information::get_column()
{
    return column;
}
