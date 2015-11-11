#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include "symtab.hh"
#include "scanner.hh"

using namespace std;

/* Used for looking up token int id : token string name */
typedef struct {
    int  token;
    string name;
} token_name;


/*** Global variables ***/
YYSTYPE yylval;
YYLTYPE yylloc;


/* List of all tokens. */
token_name tokens[] = {
    { T_ERROR,          "T_ERROR" },
    { T_EOF,            "T_EOF" },
    { T_DOT,            "T_DOT" },
    { T_SEMICOLON,      "T_SEMICOLON" },
    { T_EQ,             "T_EQ" },
    { T_COLON,          "T_COLON" },
    { T_LEFTBRACKET,    "T_LEFTBRACKET" },
    { T_RIGHTBRACKET,   "T_RIGHTBRACKET" },
    { T_LEFTPAR,        "T_LEFTPAR" },
    { T_RIGHTPAR,       "T_RIGHTPAR" },

    { T_COMMA,          "T_COMMA" },
    { T_LESSTHAN,       "T_LESSTHAN" },
    { T_GREATERTHAN,    "T_GREATERTHAN" },
    { T_ADD,            "T_ADD" },
    { T_SUB,            "T_SUB" },
    { T_MUL,            "T_MUL" },
    { T_RDIV,           "T_RDIV" },
    { T_OF,             "T_OF" },
    { T_IF,             "T_IF" },
    { T_DO,             "T_DO" },

    { T_ASSIGN,         "T_ASSIGN" },
    { T_NOTEQ,          "T_NOTEQ" },
    { T_OR,             "T_OR" },
    { T_VAR,            "T_VAR" },
    { T_END,            "T_END" },
    { T_AND,            "T_AND" },
    { T_IDIV,           "T_IDIV" },
    { T_MOD,            "T_MOD" },
    { T_NOT,            "T_NOT" },
    { T_THEN,           "T_THEN" },

    { T_ELSE,           "T_ELSE" },
    { T_IDENT,          "T_IDENT" },
    { T_CONST,          "T_CONST" },
    { T_ARRAY,          "T_ARRAY" },
    { T_BEGIN,          "T_BEGIN" },
    { T_WHILE,          "T_WHILE" },
    { T_ELSIF,          "T_ELSIF" },
    { T_INTNUM,         "T_INTNUM" },
    { T_RETURN,         "T_RETURN" },
    { T_PROGRAM,        "T_PROGRAM" },

    { T_REALNUM,        "T_REALNUM" },
    { T_FUNCTION,       "T_FUNCTION" },
    { T_PROCEDURE,      "T_PROCEDURE" },
    { T_STRINGCONST,    "T_STRINGCONST" }
};


const int nr_tokens = sizeof(tokens) / sizeof(*tokens);


/* This part of the code is template magic, see Stroustrup page 346-347. */
ostream &print_token(ostream &o, int token)
{
    int i;
    double re; // variable used to printing yylval.rval
    long in; // variable used to printing yylval.ival
    extern char *yytext;
    extern YYLTYPE yylloc;
    extern YYSTYPE yylval; /* #change extern YYSTYPE yylval;*/
    for (i = 0; i < nr_tokens; i++) {


        if (token == tokens[i].token) {
            if (i == 40) {
                re = yylval.rval;
                o << tokens[i].name << " \'" << yytext << "\' "
                  << "(" << yylloc.first_line << ", " << yylloc.first_column
                  << ")" << " <yylval.rval = " << re << ">";
                return o;
            } else if (i == 37) {
                in = yylval.ival;
                o << tokens[i].name << " \'" << yytext << "\' "
                  << "(" << yylloc.first_line << ", " << yylloc.first_column
                  << ")" << " <yylval.ival = " << in << ">";
                return o;
            } else if (i == 43) {
                o << tokens[i].name << " \'" << yytext << "\' "
                  << "(" << yylloc.first_line << ", " << yylloc.first_column
                  << ")" << " <yylval.str = " << sym_tab->capitalize(sym_tab->pool_lookup(yylval.str)) << ">";
                return o;
            } else if (i == 31) {
                o << tokens[i].name << " \'" << yytext << "\' "
                  << "(" << yylloc.first_line << ", " << yylloc.first_column
                  << ")" << " <yylval.pool_p = " << sym_tab->pool_lookup(yylval.pool_p) << ">";
                return o;
            } else {
                o << tokens[i].name << " \'" << yytext << "\' "
                  << "(" << yylloc.first_line << ", " << yylloc.first_column
                  << ")";
                return o;
            } //sym_tab->capitalize(sym_tab->pool_lookup(yylval.str))
        }
    }

    o << '\'' << (unsigned char)token << '\'';

    return o;
}



class Omanip_int
{
public:
    Omanip_int(ostream & (*f)(ostream &, int), int t): _func(f), _arg(t) {}
    friend ostream &operator << (ostream &os, const Omanip_int &manip) {
        (*manip._func)(os, manip._arg);
        return os;
    }
private:
    ostream  &(* _func)(ostream &, int);
    int _arg;
};



const Omanip_int Token(int t)
{
    return Omanip_int(&print_token, t);
}



/* Magic part ends here. */


/* Interactive scanner. We just parse whatever is typed in, and the token
   type and corresponding yytext is printed. */
int main(int argc, char **argv)
{
    int     token;
    extern  FILE *yyin;
    extern  int yylex();

    /* Open the input file, if any. */
    switch (argc) {
    case 1:
        yyin = stdin;
        break;
    case 2:
        yyin = fopen(argv[1], "r");
        if (yyin == NULL) {
            perror(argv[1]);
            exit(1);
        }
        break;
    default:
        cerr << "Usage: " << argv[0] << " [ filename ]\n";
        exit(1);
    }

    /* Loop for as long as there are tokens */

    while ((token = yylex()) != 0) {
        cout << "Scanned " << Token(token) << '\n' << flush;
    }

    cout << "End of file\n";
    exit(0);
}

