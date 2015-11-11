#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "scanner.hh"
#include "symtab.hh"

using namespace std;

// Set the #defines to 1 if you want the code which produces the
// the corresponding trace file to be generated, and 0 otherwise.

// Simulate a diesel program.
#define TEST_TRACE1 1

// Simulate a diesel program with an erroneous redeclaration (TEST_TRACE must
// also be 1 for this to be run).
#define TEST_REDECLARATION 0

// Test all enter_ methods in the symbol table.
#define TEST_TRACE2 0


YYSTYPE yylval;
YYLTYPE yylloc;


int main(int argc, char **argv) {
    // This is just a dummy position for the preinstalled functions.
    position_information *pos = new position_information();

    cout << "Starting symtabtest.cc...\n" << flush;

#if TEST_TRACE1
    // Used for debugging printouts.
    pool_index a_var, b_var, c_var;

    // Uncomment the printing of the symbols with these indices for detailed
    // symbol information at every step in the program (spam alert, though...)
    sym_index ai, bi, ci, p1i, p2i, b2i, c2i, c3i;

    // The following function calls simulate the DIESEL program inside the
    // comments. Simply run the binary file 'symtab' that you get when
    // compiling using 'make' in this dir. It will print a symbol table.
    // Compare the output from your program with symtab.trace1 file which
    // resides in the course home directory.

/*program prog;                */ sym_tab->enter_procedure(pos, sym_tab->pool_install("prog"));
/*var                        */ sym_tab->open_scope();
/*   a : integer;            */ a_var = sym_tab->pool_install("a");
/*                            */ ai = sym_tab->enter_variable(pos, a_var, integer_type);
                            // cout << sym_tab->get_symbol(ai) << endl;

                            // Note: Uncomment this to simulate a redeclaration
                            //       of the variable "a". Your symbol table should
                            //       complain about it if it works correctly.
#if TEST_REDECLARATION
/*   a : integer;            */ a_var = sym_tab->pool_install("a");
/*                            */ ai = sym_tab->enter_variable(pos, a_var, real_type);
#endif
/*   b : integer;            */ b_var = sym_tab->pool_install("b");
/*                            */ bi = sym_tab->enter_variable(pos, b_var, integer_type);
                            // cout << sym_tab->get_symbol(bi) << endl;
/*   c : integer;            */ c_var = sym_tab->pool_install("c");
/*                            */ ci = sym_tab->enter_variable(pos, c_var, integer_type);
                            // cout << sym_tab->get_symbol(ci) << endl;

/*   procedure p1;            */ p1i = sym_tab->enter_procedure(pos, sym_tab->pool_install("p1"));
                            // cout << sym_tab->get_symbol(p1i) << endl;
/*   var                    */ sym_tab->open_scope();
/*      b : real;            */ b_var = sym_tab->pool_install("b");
/*                            */ b2i = sym_tab->enter_variable(pos, b_var, real_type);
                            // cout << sym_tab->get_symbol(b2i) << endl;
/*      c : real;            */ c_var = sym_tab->pool_install("c");
/*                            */ c2i = sym_tab->enter_variable(pos, c_var, real_type);
                            // cout << sym_tab->get_symbol(c2i) << endl;

/*      procedure p2;        */ p2i = sym_tab->enter_procedure(pos, sym_tab->pool_install("p2"));
                            // cout << sym_tab->get_symbol(p2i) << endl;
/*      var                    */ sym_tab->open_scope();
/*        c : integer;        */ c_var = sym_tab->pool_install("c");
/*                            */ c3i = sym_tab->enter_variable(pos, c_var, integer_type);
/*      begin                */ // cout << sym_tab->get_symbol(c3i) << endl;
/*        c := b + a;        */ cout << "Inside p2:\n"
                                    << sym_tab->get_symbol(sym_tab->lookup_symbol(c_var))<< endl
                                    << sym_tab->get_symbol(sym_tab->lookup_symbol(b_var))<< endl
                                     << sym_tab->get_symbol(sym_tab->lookup_symbol(a_var))<< endl;
/*      end;                */ sym_tab->close_scope();
/*                            */
/*   begin                    */
/*      c := b + a;            */ cout << "Inside p1:\n"
                                    << sym_tab->get_symbol(sym_tab->lookup_symbol(c_var))<< endl
                                    << sym_tab->get_symbol(sym_tab->lookup_symbol(b_var))<< endl
                                    << sym_tab->get_symbol(sym_tab->lookup_symbol(a_var))<< endl;
/*   end;                    */ sym_tab->close_scope();
/*                            */
/*begin                        */
/*end.                        */ sym_tab->close_scope();

#endif

/* If you want to test your symtab more, this part also does some simulation
   of DIESEL code, calling enter_* for all the various symbol types except
   nametypes. There is a symtab.trace2 file in the course home dir which
   shows the result of running this code AND the one above,
   with a correct scanner/symbol table.
   You can compare the output of your program with that file. */

#if TEST_TRACE2
    sym_index tmp;
    pool_index p_func;

    cout << "Current environment: " << sym_tab->current_environment() << endl;
    sym_tab->enter_procedure(pos,sym_tab->pool_install("test_proc"));

    sym_tab->open_scope();


    sym_tab->print(2);
    sym_tab->enter_parameter(pos,
                             sym_tab->pool_install("test_param1"),
                             integer_type);

    sym_tab->enter_parameter(pos,
                             sym_tab->pool_install("test_param2"),
                             real_type);

    cout << "Current environment: " << sym_tab->current_environment() << endl;

    tmp = sym_tab->enter_constant(pos,
                                  sym_tab->pool_install("test_const1"),
                                  real_type,
                                  atof("2.45"));
    cout << "Constant stored as:\n" << sym_tab->get_symbol(tmp);

    sym_tab->enter_variable(pos,
                            sym_tab->pool_install("test_var1"),
                            integer_type);
    p_func = sym_tab->pool_install("test_func"); // install a string and puts its pool_index into p_func
    sym_tab->enter_function(pos,p_func);
    sym_tab->set_symbol_type(sym_tab->lookup_symbol(p_func), integer_type);  // sets symbol_type which
                   // will be set in labb 3, the parser, but this is the testprogram to lab 2 and
                   // therefore we have to set it manually here.
    sym_tab->open_scope();
    sym_tab->enter_array(pos,sym_tab->pool_install("test_array1"), integer_type, 3);



    sym_tab->close_scope();
    sym_tab->close_scope();

    cout << "Current environment: " << sym_tab->current_environment() << endl;
#endif
    sym_tab->print(2);
    sym_tab->print(1);

    cout << "ENDING TEST PROGRAM RUN -----------------------------\n\n\n\n";

    return 1;
}


