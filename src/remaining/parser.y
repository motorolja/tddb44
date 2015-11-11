%{
#include <iostream>
#include "semantic.hh"
#include "optimize.hh"
#include "codegen.hh"

/* Defined in parser.cc */
extern char *yytext;

/* Nr of errors encountered so far. Only generate quads & assembler if
   error_count == 0. Defined in error.cc. */
extern int error_count;

/* Defined in symtab.cc. */
extern symbol_table *sym_tab;

/* Defined in semantic.cc. */
extern semantic *type_checker;

/* Defined in codegen.cc. */
extern code_generator *code_gen;

/* From scanner.l output. */
extern int yylex();

/* Defined in error.hh. */
extern void yyerror(string);

/* All these defined in main.cc. They represent some of the flags
   given to the 'diesel' script. */
extern bool print_ast;
extern bool print_quads;
extern bool typecheck;
extern bool optimize;
extern bool quads;
extern bool assembler;

#define YYDEBUG 1

/* Have this defined to give better error messages. Using it causes
   some bison warnings at compiler compile time, however. Use as you
   wish. Not mandatory. */
/* #define YYERROR_VERBOSE */
%}



/* The different semantic values that can be returned within the AST. This is
   actually the same yylval union as you used in the first lab, only that now
   it can contain all the various AST nodes that can be returned during
   parsing, not just the few token attributes set by the scanner. If that
   didn't make sense to you, take some time reading the bison manual to
   figure out exactly what this is for. */
%union {
    ast_node             *ast;
    ast_id               *id;
    ast_stmt_list        *statement_list;
    ast_statement        *statement;
    ast_expr_list        *expression_list;
    ast_expression       *expression;
    ast_elsif_list       *elsif_list;
    ast_elsif            *elsif;
    ast_lvalue           *lvalue;
    ast_functioncall     *function_call;
    ast_functionhead     *function_head;
    ast_procedurehead    *procedure_head;
    ast_integer          *integer;
    ast_real             *real;

    long                  ival;
    double                rval;
    pool_index            str;
    pool_index            pool_p;
}

/* Here are return types for all productions that return AST nodes (ie, the
   return type of the bison $$ construct). Again, reading the bison manual
   might be very helpful (hint: maybe $$ has the type YYSTYPE?). */
%type <id>                  id const_id lvar_id rvar_id array_id proc_id func_id type_id
%type <statement_list>      stmt_list comp_stmt else_part
%type <statement>           stmt
%type <expression_list>     opt_param_list param_list opt_expr_list expr_list
%type <expression>          expr term factor simple_expr rvariable
%type <elsif_list>          elsif_list
%type <elsif>               elsif
%type <lvalue>              lvariable
%type <parameter>           param
%type <function_call>       func_call
%type <function_head>       func_decl func_head
%type <procedure_head>      prog_decl prog_head proc_decl proc_head
%type <integer>             integer
%type <real>                real


/* Here are just a list of all the DIESEL tokens. A few of them are marked
   as having special fields in yylval set. */
%token T_EOF T_ERROR T_DOT T_SEMICOLON T_EQ T_COLON T_LEFTBRACKET
%token T_RIGHTBRACKET T_LEFTPAR T_RIGHTPAR T_COMMA T_LESSTHAN T_GREATERTHAN
%token T_ADD T_SUB T_MUL T_RDIV T_OF T_IF T_DO T_ASSIGN T_NOTEQ T_OR T_VAR
%token T_END T_AND T_IDIV T_MOD T_NOT T_THEN T_ELSE T_CONST T_ARRAY
%token T_BEGIN T_WHILE T_ELSIF T_RETURN
%token T_STRINGCONST
%token <pool_p> T_IDENT T_PROGRAM T_PROCEDURE T_FUNCTION
%token <ival> T_INTNUM
%token <rval> T_REALNUM

/* Associative rules for operators. */
%nonassoc T_LESSTHAN T_GREATERTHAN T_EQ T_NOTEQ
%left T_NOT
%left T_AND
%left T_OR
%left T_ADD T_SUB
%left T_MUL T_RDIV T_IDIV T_MOD


%start program



%%



program         : prog_decl subprog_part comp_stmt T_DOT
                {

                    symbol *env = sym_tab->get_symbol($1->sym_p);

                    // The status variables here depend on what flags were
                    // passed to the compiler. See the 'diesel' script for
                    // more information.
                    if (typecheck) {
                        type_checker->do_typecheck(env, $3);
                    }

                    if (print_ast) {
                        cout << "\nUnoptimized AST for global level" << endl;
                        cout << (ast_stmt_list *)$3 << endl;
                    }

                    if (optimize) {
                        optimizer->do_optimize($3);
                        if(print_ast) {
                            cout << "\nOptimized AST for global level" << endl;
                            cout << (ast_stmt_list *)$3 << endl;
                        }
                    }
                    if (error_count == 0) {
                        if (quads) {
                            quad_list *q = $1->do_quads($3);
                            if (print_quads) {
                                cout << "\nQuad list for global level" << endl;
                                cout << (quad_list *)q << endl;
                            }

                            if (assembler) {
                                cout << "Generating assembler, global level"
                                     << endl;
                                code_gen->generate_assembler(q, env);
                            }
                        }
                    } else {
                        cout << "Found " << error_count << " errors. "
                             << "Compilation aborted.\n";
                    }

                    // We close the global scope.
                    sym_tab->close_scope();
                }
                ;


prog_decl       : prog_head T_SEMICOLON const_part variable_part
                {
                    $$ = $1;
                }
                ;


prog_head       : T_PROGRAM T_IDENT
                {
                    /* Your code here */
                    sym_tab->open_scope();
                }
                ;


const_part      : T_CONST const_decls
                | error const_decls
                | /* empty */
                ;


const_decls     : const_decl
                | const_decls const_decl
                ;


const_decl      : T_IDENT T_EQ integer T_SEMICOLON
                {
                    /* Your code here */
                }
                | T_IDENT T_EQ real T_SEMICOLON
                {
                    /* Your code here */
                }
                | T_IDENT T_EQ T_STRINGCONST T_SEMICOLON
                {
                    // This isn't implemented in Diesel... Do nothing.
                }
                | T_IDENT T_EQ const_id T_SEMICOLON
                {

                    // This part of code is a bit ugly, but it's needed to
                    // allow constructions like this:
                    // constant foo = 5;
                    // constant bar = foo;
                    // ...now, why would anyone want to do that?
                    /* Your code here */
                }
                
                ;


variable_part   : T_VAR var_decls
                | /* empty */
                ;


var_decls       : var_decl
                | var_decls var_decl
                ;


var_decl        : T_IDENT T_COLON type_id T_SEMICOLON
                {
                    /* Your code here */
                }
                | T_IDENT T_COLON T_ARRAY T_LEFTBRACKET integer T_RIGHTBRACKET T_OF type_id T_SEMICOLON
                {
                    /* Your code here */
                }
                | T_IDENT T_COLON T_ARRAY T_LEFTBRACKET const_id T_RIGHTBRACKET T_OF type_id T_SEMICOLON
                {
                    // We enter an array: pool_pointer, type pointer,
                    // the id type of the constant, and the value of the
                    // constant.
                    position_information *pos =
                        new position_information(@1.first_line,
                                                 @1.first_column);

                    // Ideally we should be able to just enter the array and
                    // defer index type checking to the semantic phase.
                    // However, since a constant can represent a real value,
                    // and the enter_array() method only accepts integer
                    // values for cardinality, we have to prevent entering an
                    // array with a float index here, or the compiler would
                    // crash.
                    // However, to make the compiler install the array,
                    // so we won't get a ton of complaints about this array
                    // being undeclared later on, we cheat and install it
                    // anyway with an illegal cardinality - which we _can_
                    // defer checking until the semantic phase.

                    // Find the const_id's symbol in the symbol table.
                    symbol *tmp =
                        sym_tab->get_symbol($5->sym_p);

                    // Make sure it exists. This should belong in a later pass,
                    // but if we don't do it here and NULL is returned (which
                    // shouldn't happen if you've done everything right, but
                    // paranoia never hurts) the compiler would crash.
                    if(tmp == NULL || tmp->tag != SYM_CONST) {
                        type_error(pos) << "bad index in array declaration: "
                                        << yytext << endl << flush;
                    } else {
                        constant_symbol *con = tmp->get_constant_symbol();
                        if (con->type == integer_type) {
                            sym_tab->enter_array(pos,
                                                 $1,
                                                 $8->sym_p,
                                                 con->const_value.ival);
                        } else {
                            sym_tab->enter_array(pos,
                                                 $1,
                                                 $8->sym_p,
                                                 ILLEGAL_ARRAY_CARD);
                        }
                    }
                }
                
                ;


subprog_part    : subprog_decls
                | /* empty */
                ;


subprog_decls   : subprog_decl
                | subprog_decls subprog_decl
                ;


subprog_decl    : proc_decl subprog_part comp_stmt T_SEMICOLON
                {

                    symbol *env = sym_tab->get_symbol($1->sym_p);

                    if (typecheck) {
                        type_checker->do_typecheck(env, $3);
                    }

                    if (print_ast) {
                        cout << "\nUnoptimized AST for \""
                             << sym_tab->pool_lookup(env->id)
                             << "\"" << endl;
                        cout << (ast_stmt_list *)$3 << endl;
                    }

                    if (optimize) {
                        optimizer->do_optimize($3);
                        if (print_ast) {
                            cout << "\nOptimized AST for \""
                                 << sym_tab->pool_lookup(env->id)
                                 << "\"" << endl;
                            cout << (ast_stmt_list*)$3 << endl;
                        }
                    }

                    if (error_count == 0) {
                        if (quads) {
                            quad_list *q = $1->do_quads($3);
                            if (print_quads) {
                                cout << "\nQuad list for \""
                                     << sym_tab->pool_lookup(env->id)
                                     << "\"" << endl;
                                cout << (quad_list *)q << endl;
                            }

                            if (assembler) {
                                cout << "Generating assembler for procedure \""
                                     << sym_tab->pool_lookup(env->id)
                                     << "\"" << endl;
                                code_gen->generate_assembler(q, env);
                            }
                        }
                    }

                    // Close the current scope.
                    sym_tab->close_scope();
                }
                | func_decl subprog_part comp_stmt T_SEMICOLON
                {

                    symbol *env = sym_tab->get_symbol($1->sym_p);

                    if (typecheck) {
                        type_checker->do_typecheck(env, $3);
                    }

                    if (print_ast) {
                        cout << "\nUnoptimized AST for \""
                             << sym_tab->pool_lookup(env->id)
                             << "\"" << endl;
                        cout << (ast_stmt_list *)$3 << endl;
                    }

                    if (optimize) {
                        optimizer->do_optimize($3);
                        if (print_ast) {
                            cout << "\nOptimized AST for \""
                                 << sym_tab->pool_lookup(env->id)
                                 << "\"" << endl;
                            cout << (ast_stmt_list *)$3 << endl;
                        }
                    }

                    if (error_count == 0) {
                        if (quads) {
                            quad_list *q = $1->do_quads($3);
                            if (print_quads) {
                                cout << "\nQuad list for \""
                                     << sym_tab->pool_lookup(env->id)
                                     << "\"" << endl;
                                cout << (quad_list *)q << endl;
                            }

                            if (assembler) {
                                cout << "Generating assembler for function \""
                                     << sym_tab->pool_lookup(env->id) << "\""
                                     << endl;
                                code_gen->generate_assembler(q, env);
                            }
                        }
                    }

                    // Close the current scope.
                    sym_tab->close_scope();
                }
                ;


proc_decl       : proc_head opt_param_list T_SEMICOLON const_part variable_part
                {
                    $$ = $1;
                }
                ;


func_decl       : func_head opt_param_list T_COLON type_id T_SEMICOLON const_part variable_part
                {
                    /* Your code here */
                }
                ;


proc_head       : T_PROCEDURE T_IDENT
                {
                    position_information *pos =
                        new position_information(@1.first_line,
                                                 @1.first_column);
                    // We add the function id to the symbol table.
                    sym_index proc_loc = sym_tab->enter_procedure(pos,
                                                                  $2);
                    // Open a new scope.
                    sym_tab->open_scope();
                    // This AST node is just a temporary node which we create
                    // here in order to be able to provide the symbol table
                    // index for the procedure to the proc_decl production
                    // above. It's needed for code generation.
                    $$ = new ast_procedurehead(pos,
                                               proc_loc);
                }
                ;


func_head       : T_FUNCTION T_IDENT
                {
                    position_information *pos =
                        new position_information(@1.first_line,
                                                 @1.first_column);
                    // We add the function id to the symbol table.
                    sym_index func_loc = sym_tab->enter_function(pos,
                                                                 $2);
                    // Open a new scope.
                    sym_tab->open_scope();

                    // This AST node is just a temporary node which we create
                    // here in order to be able to provide the symbol table
                    // index for the function to the func_decl production
                    // above. We need it to be able to set the return type
                    // and also later on for code generation.
                    $$ = new ast_functionhead(pos,
                                              func_loc);
                }
                ;


opt_param_list  : T_LEFTPAR param_list T_RIGHTPAR
                {
                    /* Your code here */
                }
                | T_LEFTPAR error T_RIGHTPAR
                {
                    $$ = NULL;
                }
                | /* empty */
                {
                    /* Your code here */
                }
                ;


param_list      : param
                {
                    /* Note that we use expr_lists for parameters. This
                       is thus simply a place-holder in the grammar. */
                }
                | param_list T_SEMICOLON param
                {
                }
                ;


param           : T_IDENT T_COLON type_id
                {
                    position_information *pos =
                        new position_information(@1.first_line,
                                                 @1.first_column);

                    // Enter parameter into the symbol table. The linking of
                    // parameters and things is taken care of in the
                    // enter_parameter function, which is worth taking a
                    // second look at.
                    sym_index param_loc =
                        sym_tab->enter_parameter(pos,
                                                 $1,
                                                 $3->sym_p);
                }
                ;


comp_stmt       : T_BEGIN stmt_list T_END
                {
                    /* Your code here */
                }
                ;


stmt_list       : stmt
                {
                    /* Your code here */
                }
                | stmt_list T_SEMICOLON stmt
                {
                    /* Your code here */
                }
                ;


stmt            : T_IF expr T_THEN stmt_list elsif_list else_part T_END
                {
                    /* Your code here */
                }
                | T_WHILE expr T_DO stmt_list T_END
                {
                    /* Your code here */
                }
                | proc_id T_LEFTPAR opt_expr_list T_RIGHTPAR
                {
                    /* Your code here */
                }
                | lvariable T_ASSIGN expr
                {
                    /* Your code here */
                }
                | T_RETURN expr
                {
                    /* Your code here */
                }
                | T_RETURN
                {
                    /* Your code here */
                }
                
                | /* empty */
                {
                    /* Your code here */
                }
                ;

lvariable       : lvar_id
                {
                    $$ = $1;
                }
                | array_id T_LEFTBRACKET expr T_RIGHTBRACKET
                {
                    $$ = new ast_indexed($1->pos,
                                         $1,
                                         $3);
                }
                | array_id T_LEFTBRACKET error T_RIGHTBRACKET
                {
                    $$ = NULL;
                }
                ;


rvariable       : rvar_id
                {
                    /* Your code here */
                }
                | array_id T_LEFTBRACKET expr T_RIGHTBRACKET
                {
                    /* Your code here */
                }
                
                ;


elsif_list      : elsif_list elsif
                {
                    /* Your code here */
                }
                | /* empty */
                {
                    /* Your code here */
                }
                ;


elsif           : T_ELSIF expr T_THEN stmt_list
                {
                    /* Your code here */
                }
                ;


else_part       : T_ELSE stmt_list
                {
                    /* Your code here */
                }
                | /* empty */
                {
                    /* Your code here */
                }
                ;


opt_expr_list   : expr_list
                {
                    /* Your code here */
                }
                | /* empty */
                {
                    /* Your code here */
                }
                ;


expr_list       : expr
                {
                    /* Your code here */
                }
                | expr_list T_COMMA expr
                {
                    /* Your code here */
                }
                ;


expr            : simple_expr
                {
                    /* Your code here */
                }
                | expr T_EQ simple_expr
                {
                    /* Your code here */
                }
                | expr T_NOTEQ simple_expr
                {
                    /* Your code here */
                }
                | expr T_LESSTHAN simple_expr
                {
                    /* Your code here */
                }
                | expr T_GREATERTHAN simple_expr
                {
                    /* Your code here */
                }
                ;


simple_expr     : term
                {
                    /* Your code here */
                }
                | T_ADD term
                {
                    /* Your code here */
                }
                | T_SUB term
                {
                    /* Your code here */
                }
                | simple_expr T_OR term
                {
                    /* Your code here */
                }
                | simple_expr T_ADD term
                {
                    /* Your code here */
                }
                | simple_expr T_SUB term
                {
                    /* Your code here */
                }
                ;


term            : factor
                {
                    /* Your code here */
                }
                | term T_AND factor
                {
                    /* Your code here */
                }
                | term T_MUL factor
                {
                    /* Your code here */
                }
                | term T_RDIV factor
                {
                    /* Your code here */
                }
                | term T_IDIV factor
                {
                    /* Your code here */
                }
                | term T_MOD factor
                {
                    /* Your code here */
                }
                ;


factor          : rvariable
                {
                    $$ = $1;
                }
                | func_call
                {
                    $$ = $1;
                }
                | integer
                {
                    $$ = $1;
                }
                | real
                {
                    $$ = $1;
                }
                | T_NOT factor
                {
                    /* Your code here */
                }
                | T_LEFTPAR expr T_RIGHTPAR
                {
                    /* Your code here */
                }
                
                ;


func_call       : func_id T_LEFTPAR opt_expr_list T_RIGHTPAR
                {
                    /* Your code here */
                }
                |
                
                ;


integer         : T_INTNUM
                {
                    position_information *pos =
                        new position_information(@1.first_line,
                                                 @1.first_column);

                    // We need to pass on the value AND the position here.
                    $$ = new ast_integer(pos,
                                         $1);
                }
                ;


real            : T_REALNUM
                {
                    position_information *pos =
                        new position_information(@1.first_line,
                                                 @1.first_column);

                    // We create a new real constant.
                    $$ = new ast_real(pos,
                                      $1);
                }
                ;


type_id         : id
                {
                    // Make sure this id is really declared as a type.
                    // debug() << "type_id -> id: "
                    //       << sym_tab->get_symbol($1->sym_p) << endl;
                    if(sym_tab->get_symbol_tag($1->sym_p) != SYM_NAMETYPE) {
                        type_error($1->pos) << "not declared "
                                            << "as type: "
                                            << yytext << endl << flush;
                    }
                    $$ = $1;
                }
                ;


const_id        : id
                {
                    // Make sure this id is really declared as a constant.
                    // debug() << "const_id -> id: " << $1->sym_p << endl;
                    if(sym_tab->get_symbol_tag($1->sym_p) != SYM_CONST) {
                        type_error($1->pos) << "not declared "
                                            << "as constant: "
                                            << yytext << flush;
                    }
                    $$ = $1;
                }
                ;


lvar_id         : id
                {
                    // Make sure this id is really declared as an lvariable.
                    // debug() << "lvar_id -> id: " << $1->sym_p << endl;
                    if (sym_tab->get_symbol_tag($1->sym_p) != SYM_VAR &&
                       sym_tab->get_symbol_tag($1->sym_p) != SYM_PARAM) {
                        type_error($1->pos) << "not declared "
                                            << "as variable or parameter: "
                                            << yytext << endl << flush;
                    }
                    $$ = $1;
                }
                ;
rvar_id         : id
                {
                    // Make sure this id is really declared as an rvariable.
                    // debug() << "rvar_id -> id: " << $1->sym_p << endl;
                    if (sym_tab->get_symbol_tag($1->sym_p) != SYM_VAR &&
                       sym_tab->get_symbol_tag($1->sym_p) != SYM_PARAM &&
                       sym_tab->get_symbol_tag($1->sym_p) != SYM_CONST) {
                        type_error($1->pos) << "not declared "
                                            << "as variable, parameter or "
                                            << "constant: "
                                            << yytext << endl << flush;
                    }
                    $$ = $1;
                }
                ;


proc_id         : id
                {
                    // Make sure this id is really declared as a procedure.
                    // debug() << "proc_id -> id: " << $1->sym_p << endl;
                    if (sym_tab->get_symbol_tag($1->sym_p) != SYM_PROC) {
                        type_error($1->pos) << "not declared "
                                            << "as procedure: "
                                            << yytext << endl << flush;
                    }
                    $$ = $1;
                }
                ;


func_id         : id
                {
                    // Make sure this id is really declared as a function.
                    // debug() << "func_id -> id: " << $1->sym_p << endl;
                    if (sym_tab->get_symbol_tag($1->sym_p) != SYM_FUNC) {
                        type_error($1->pos) << "not declared "
                                            << "as function: "
                                            << yytext << endl << flush;
                    }
                    $$ = $1;
                }
                ;


array_id        : id
                {
                    // Make sure this id is really declared as an array.
                    // debug() << "array_id -> id: " << $1->sym_p << endl;
                    if (sym_tab->get_symbol_tag($1->sym_p) != SYM_ARRAY) {
                        type_error($1->pos) << "not declared "
                                            << "as array: "
                                            << yytext << endl << flush;
                    }
                    $$ = $1;
                }
                ;


id              : T_IDENT
                {
                    sym_index sym_p;    // Used to find previous use of symbol.
                    position_information *pos =
                        new position_information(@1.first_line,
                                                 @1.first_column);

                    // Make sure the symbol was declared before it is used.
                    sym_p = sym_tab->lookup_symbol($1);
                    // debug() << "id -> T_IDENT: " << sym_p << " "
                    //            << sym_tab->pool_lookup($1) << endl;
                    if (sym_p == NULL_SYM) {
                        type_error(pos) << "not declared: "
                                        << yytext << endl << flush;
                    }
                    // Create a new ast_id node with pos, symptr.
                    $$ = new ast_id(pos,
                                    sym_p);
                    $$->type = sym_tab->get_symbol_type(sym_p);
                }
                ;


%%
