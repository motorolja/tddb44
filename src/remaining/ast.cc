#include "ast.hh"


/*******************************************************
 *** The abstract AST classes - never used directly. ***
 *******************************************************/

int ast_node::indent_level = 0;
bool ast_node::branches[10000];

/* The superclass ast_node. */
ast_node::ast_node(position_information *p) :
    pos(p)
{
    tag = AST_NODE;
}


/* The ast_statement class. */
ast_statement::ast_statement(position_information *p) :
    ast_node(p)
{
    tag = AST_STATEMENT;
}



/* The ast_expression class. */
ast_expression::ast_expression(position_information *p) :
    ast_node(p)
{
    tag = AST_EXPRESSION;
    // This will be changed later, during type checking.
    // If it hasn't been, it's an error.
    type = void_type;
}

ast_expression::ast_expression(position_information *p,
                               sym_index s) :
    ast_node(p),
    type(s)
{
    tag = AST_EXPRESSION;
}


/* The ast_binaryrelation class. They all return integer values. */
ast_binaryrelation::ast_binaryrelation(position_information *p,
                                       ast_expression *l,
                                       ast_expression *r) :
    ast_expression(p, integer_type),
    left(l),
    right(r)
{
    tag = AST_BINARYRELATION;
}


/* The ast_binaryoperation class. The type of the node will be synthesized
   later, during type checking. See semantic.cc. */
ast_binaryoperation::ast_binaryoperation(position_information *p,
        ast_expression *l,
        ast_expression *r) :
    ast_expression(p),
    left(l),
    right(r)
{
    tag = AST_BINARYOPERATION;
}


/* The ast_lvalue class. */
ast_lvalue::ast_lvalue(position_information *p) :
    ast_expression(p)
{
    tag = AST_LVALUE;
}

ast_lvalue::ast_lvalue(position_information *p,
                       sym_index s) :
    ast_expression(p, s)
{
    tag = AST_LVALUE;
}




/***********************************************************
 *** The concrete AST classes - these appear in the AST. ***
 ***********************************************************/

/* The ast_elsif class. */
ast_elsif::ast_elsif(position_information *p,
                     ast_expression *c,
                     ast_stmt_list *b) :
    ast_node(p),
    condition(c),
    body(b)
{
    tag = AST_ELSIF;
}


/* The ast_expr_list class. Currently only used for parameter lists. */
ast_expr_list::ast_expr_list(position_information *p,
                             ast_expression *l) :
    ast_node(p),
    last_expr(l)
{
    tag = AST_EXPR_LIST;
    preceding = NULL;
}

ast_expr_list::ast_expr_list(position_information *p,
                             ast_expression *l,
                             ast_expr_list *prev) :
    ast_node(p),
    last_expr(l),
    preceding(prev)
{
    tag = AST_EXPR_LIST;
}


/* The ast_stmt_list class. */
ast_stmt_list::ast_stmt_list(position_information *p,
                             ast_statement *h) :
    ast_node(p),
    last_stmt(h)
{
    tag = AST_STMT_LIST;
    preceding = NULL;
}

ast_stmt_list::ast_stmt_list(position_information *p,
                             ast_statement *h,
                             ast_stmt_list *t) :
    ast_node(p),
    last_stmt(h),
    preceding(t)
{
    tag = AST_STMT_LIST;
}


/* The ast_elsif_list class. */
ast_elsif_list::ast_elsif_list(position_information *p,
                               ast_elsif *h) :
    ast_node(p),
    last_elsif(h)
{
    tag = AST_ELSIF_LIST;
    preceding = NULL;
}

ast_elsif_list::ast_elsif_list(position_information *p,
                               ast_elsif *h,
                               ast_elsif_list *t) :
    ast_node(p),
    last_elsif(h),
    preceding(t)
{
    tag = AST_ELSIF_LIST;
}


/* The ast_procedurecall class. */
ast_procedurecall::ast_procedurecall(position_information *p,
                                     ast_id *i,
                                     ast_expr_list *par) :
    ast_statement(p),
    id(i),
    parameter_list(par)
{
    tag = AST_PROCEDURECALL;
}


/* The ast_assign class. */
ast_assign::ast_assign(position_information *p,
                       ast_lvalue *l,
                       ast_expression *r) :
    ast_statement(p),
    lhs(l),
    rhs(r)
{
    tag = AST_ASSIGN;
}


/* The ast_while class. */
ast_while::ast_while(position_information *p,
                     ast_expression *c,
                     ast_stmt_list *b) :
    ast_statement(p),
    condition(c),
    body(b)
{
    tag = AST_WHILE;
}


/* The ast_if class. */
ast_if::ast_if(position_information *p,
               ast_expression *c,
               ast_stmt_list *b,
               ast_elsif_list *eil,
               ast_stmt_list *eb) :
    ast_statement(p),
    condition(c),
    body(b),
    elsif_list(eil),
    else_body(eb)
{
    tag = AST_IF;
}


/* The ast_return class. */
ast_return::ast_return(position_information *p) :
    ast_statement(p)
{
    tag = AST_RETURN;
    value = NULL;
}

ast_return::ast_return(position_information *p,
                       ast_expression *v) :
    ast_statement(p),
    value(v)
{
    tag = AST_RETURN;
}


/* The ast_functioncall class. */
ast_functioncall::ast_functioncall(position_information *p,
                                   ast_id *i,
                                   ast_expr_list *par) :
    ast_expression(p, i->type),
    id(i),
    parameter_list(par)
{
    tag = AST_FUNCTIONCALL;
}



/*** Unary operator nodes: ast_uminus, ast_not. */

/* The ast_uminus class. */
ast_uminus::ast_uminus(position_information *p,
                       ast_expression *e) :
    ast_expression(p, e->type),
    expr(e)
{
    tag = AST_UMINUS;
}

/* The ast_not class. Logical negation. */
ast_not::ast_not(position_information *p,
                 ast_expression *e) :
    ast_expression(p, integer_type),
    expr(e)
{
    tag = AST_NOT;
}



/*** Classes derived from ast_binaryrelation. ***/

/* The ast_equal class. */
ast_equal::ast_equal(position_information *p,
                     ast_expression *l,
                     ast_expression *r) :
    ast_binaryrelation(p, l, r)
{
    tag = AST_BINARYRELATION;
}

/* The ast_notequal class. */
ast_notequal::ast_notequal(position_information *p,
                           ast_expression *l,
                           ast_expression *r) :
    ast_binaryrelation(p, l, r)
{
    tag = AST_NOTEQUAL;
}


/* The ast_lessthan class. */
ast_lessthan::ast_lessthan(position_information *p,
                           ast_expression *l,
                           ast_expression *r) :
    ast_binaryrelation(p, l, r)
{
    tag = AST_LESSTHAN;
}

/* The ast_greaterthan class. */
ast_greaterthan::ast_greaterthan(position_information *p,
                                 ast_expression *l,
                                 ast_expression *r) :
    ast_binaryrelation(p, l, r)
{
    tag = AST_GREATERTHAN;
}



/*** Classes derived from ast_binaryoperation. ***/

/* The ast_add class. */
ast_add::ast_add(position_information *p,
                 ast_expression *l,
                 ast_expression *r) :
    ast_binaryoperation(p, l, r)
{
    tag = AST_ADD;
}

/* The ast_sub class. */
ast_sub::ast_sub(position_information *p,
                 ast_expression *l,
                 ast_expression *r) :
    ast_binaryoperation(p, l, r)
{
    tag = AST_SUB;
}

/* The ast_or class. */
ast_or::ast_or(position_information *p,
               ast_expression *l,
               ast_expression *r) :
    ast_binaryoperation(p, l, r)
{
    tag = AST_OR;
}

/* The ast_and class. */
ast_and::ast_and(position_information *p,
                 ast_expression *l,
                 ast_expression *r) :
    ast_binaryoperation(p, l, r)
{
    tag = AST_AND;
}

/* The ast_mult class. */
ast_mult::ast_mult(position_information *p,
                   ast_expression *l,
                   ast_expression *r) :
    ast_binaryoperation(p, l, r)
{
    tag = AST_MULT;
}

/* The ast_divide class. */
ast_divide::ast_divide(position_information *p,
                       ast_expression *l,
                       ast_expression *r) :
    ast_binaryoperation(p, l, r)
{
    tag = AST_DIVIDE;
}

/* The ast_idiv class. */
ast_idiv::ast_idiv(position_information *p,
                   ast_expression *l,
                   ast_expression *r) :
    ast_binaryoperation(p, l, r)
{
    tag = AST_IDIV;
}

/* The ast_mod class. */
ast_mod::ast_mod(position_information *p,
                 ast_expression *l,
                 ast_expression *r) :
    ast_binaryoperation(p, l, r)
{
    tag = AST_MOD;
}



/*** Nodes that function as lvalues: ast_id and ast_indexed ***/

/* The ast_id class. */
ast_id::ast_id(position_information *p,
               sym_index s) :
    ast_lvalue(p),
    sym_p(s)
{
    tag = AST_ID;
}


/* The ast_indexed class. */
ast_indexed::ast_indexed(position_information *p,
                         ast_id *i,
                         ast_expression *n) :
    ast_lvalue(p),
    id(i),
    index(n)
{
    tag = AST_INDEXED;
}


/*** Nodes for representing integer/real constants, '5' or '2.5', or so. */

/* The ast_integer class. */
ast_integer::ast_integer(position_information *p,
                         long i) :
    ast_expression(p, integer_type),
    value(i)
{
    tag = AST_INTEGER;
}


/* The ast_real class. Note: the value is stored in ieee 64-bit format. */
ast_real::ast_real(position_information *p,
                   double r) :
    ast_expression(p, real_type),
    value(r)
{
    tag = AST_REAL;
}


/* The ast_cast class. Used to convert integers to reals. Note: the value is
   stored in ieee 64-bit format. Cast nodes are always of real type. */
ast_cast::ast_cast(position_information *p,
                   ast_expression *n) :
    ast_expression(p, real_type),
    expr(n)
{
    tag = AST_CAST;
}


/* The ast_functionhead class. */
ast_functionhead::ast_functionhead(position_information *p,
                                   sym_index s) :
    ast_node(p),
    sym_p(s)
{
    tag = AST_FUNCTIONHEAD;
}


/* The ast_procedurehead class. */
ast_procedurehead::ast_procedurehead(position_information *p,
                                     sym_index s) :
    ast_node(p),
    sym_p(s)
{
    tag = AST_PROCEDUREHEAD;
}



/*****************************************
 *** METHODS FOR PRINTING OF AST NODES ***
 *****************************************/

/* ast_node print handling. This is where all the indenting of the
   visualized AST is taken care of, the other derived classes just have to
   define their own print method. */
void ast_node::begin_child(ostream &o)
{
    indent(o);
    o << "+-";
    branches[indent_level] = true;
    indent_more();
}

void ast_node::end_child(ostream &o)
{
    o << "";
    indent_less();
    if (branches[indent_level]) {
        branches[indent_level] = false;
    }
}

void ast_node::last_child(ostream &o)
{
    indent(o);
    o << "+-";
    branches[indent_level] = false;
    indent_more();
}

void ast_node::indent(ostream &o)
{
    for (int i = 0; i < indent_level; i++) {
        if (branches[i]) {
            o << '|';
        } else {
            o << ' ';
        }
    }
}

void ast_node::indent_more()
{
    indent_level += 2;
}

void ast_node::indent_less()
{
    indent_level -= 2;
}

void ast_node::print(ostream &o)
{
    o << "ast_node";
}

void ast_node::xprint(ostream &o, string s)
{
    o << "ast_node (" << s << ")";
}


void ast_statement::print(ostream &o)
{
    o << "Statement";
}


void ast_expression::print(ostream &o)
{
    o << "Expression [" << short_symbols
      << sym_tab->get_symbol(type) << long_symbols << "]\n";
}


void ast_binaryrelation::print(ostream &o)
{
    o << "Binary relation";
}

void ast_binaryrelation::xprint(ostream &o, string s)
{
    o << s << " (left, right) ["
      << short_symbols << sym_tab->get_symbol(type) << long_symbols << "]\n";
    begin_child(o);
    o << left << endl;
    end_child(o);
    last_child(o);
    o << right;
    end_child(o);
}


void ast_binaryoperation::print(ostream &o)
{
    o << "Binary operation";
}

void ast_binaryoperation::xprint(ostream &o, string s)
{
    o << s << " (left, right) ["
      << short_symbols << sym_tab->get_symbol(type) << long_symbols << "]\n";
    begin_child(o);
    o << left << endl;
    end_child(o);
    last_child(o);
    o << right;
    end_child(o);
}


void ast_lvalue::print(ostream &o)
{
    o << "Lvalue";
}


void ast_elsif::print(ostream &o)
{
    o << "Elsif (condition, body)\n";
    begin_child(o);
    o << condition << endl;
    end_child(o);
    last_child(o);
    o << body;
    end_child(o);
}


void ast_expr_list::print(ostream &o)
{
    o << "Expression list (preceding, last_expr)\n";
    begin_child(o);
    o << preceding << endl;
    // o << last_expr << endl;
    end_child(o);
    last_child(o);
    o << last_expr;
    // o << preceding;
    end_child(o);
}

void ast_stmt_list::print(ostream &o)
{
    o << "Statement list (preceding, last_stmt)\n";
    begin_child(o);
    o << preceding << endl;
    // o << last_stmt << endl;
    end_child(o);
    last_child(o);
    // o << preceding;
    o << last_stmt;
    end_child(o);
}

void ast_elsif_list::print(ostream &o)
{
    o << "Elsif list (preceding, last_elsif)\n";
    begin_child(o);
    o << preceding << endl;
    // o << last_elsif << endl;
    end_child(o);
    last_child(o);
    // o << preceding;
    o << last_elsif;
    end_child(o);
}


void ast_procedurecall::print(ostream &o)
{
    o << "Procedure call (procedure, arguments)\n";
    begin_child(o);
    o << id << long_symbols << endl;
    end_child(o);
    last_child(o);
    o << parameter_list;
    end_child(o);
}

void ast_assign::print(ostream &o)
{
    o << "Assignment (left, right)\n";
    begin_child(o);
    o << lhs << endl;
    end_child(o);
    last_child(o);
    o << rhs;
    end_child(o);
}

void ast_while::print(ostream &o)
{
    o << "While (condition, body)\n";
    begin_child(o);
    o << condition << endl;
    end_child(o);
    last_child(o);
    o << body;
    end_child(o);
}

void ast_if::print(ostream &o)
{
    o << "If (condition, then, elsif, else)\n";
    begin_child(o);
    o << condition << endl;
    end_child(o);
    begin_child(o);
    o << body << endl;
    end_child(o);
    begin_child(o);
    o << elsif_list << endl;
    end_child(o);
    last_child(o);
    o << else_body;
    end_child(o);
}

void ast_return::print(ostream &o)
{
    o << "Return (value)\n";
    last_child(o);
    o << value;
    end_child(o);
}


void ast_functioncall::print(ostream &o)
{
    o << "Function call (function, arguments) ["
      << short_symbols << sym_tab->get_symbol(type) << "]\n";
    begin_child(o);
    o << id << long_symbols << endl;
    end_child(o);
    last_child(o);
    o << parameter_list;
    end_child(o);
}


void ast_uminus::print(ostream &o)
{
    o << "Unary minus (expr) ["
      << short_symbols << sym_tab->get_symbol(type) << long_symbols << "]\n";
    last_child(o);
    o << expr;
    end_child(o);
}


void ast_not::print(ostream &o)
{
    o << "Not (expr) ["
      << short_symbols << sym_tab->get_symbol(type) << long_symbols << "]\n";
    last_child(o);
    o << expr;
    end_child(o);
}


void ast_equal::print(ostream &o)
{
    xprint(o, "Equal");
}

void ast_notequal::print(ostream &o)
{
    xprint(o, "Not equal");
}

void ast_lessthan::print(ostream &o)
{
    xprint(o, "Less than");
}

void ast_greaterthan::print(ostream &o)
{
    xprint(o, "Greater than");
}


void ast_add::print(ostream &o)
{
    xprint(o, "Add");
}

void ast_sub::print(ostream &o)
{
    xprint(o, "Sub");
}

void ast_or::print(ostream &o)
{
    xprint(o, "Or");
}

void ast_and::print(ostream &o)
{
    xprint(o, "And");
}

void ast_mult::print(ostream &o)
{
    xprint(o, "Mult");
}

void ast_divide::print(ostream &o)
{
    xprint(o, "Divide");
}

void ast_idiv::print(ostream &o)
{
    xprint(o, "Idiv");
}

void ast_mod::print(ostream &o)
{
    xprint(o, "Mod");
}


void ast_id::print(ostream &o)
{
    o << "Id (" << short_symbols << sym_tab->get_symbol(sym_p)
      << ") " << "[" << sym_tab->get_symbol(type) << "]" << long_symbols;
}

void ast_indexed::print(ostream &o)
{
    o << "Indexed (id, index)\n";
    begin_child(o);
    o << short_symbols << id << long_symbols << endl;
    end_child(o);
    last_child(o);
    o << index;
    end_child(o);
}


void ast_integer::print(ostream &o)
{
    o << "Integer [" << value << "]";
}

void ast_real::print(ostream &o)
{
    o << "Real [" << value << "]";
}

void ast_cast::print(ostream &o)
{
    o << "Cast [" << short_symbols << sym_tab->get_symbol(type)
      << long_symbols << "]\n";
    last_child(o);
    o << expr;
    end_child(o);
}


void ast_functionhead::print(ostream &o)
{
    o << "Function head (" << short_symbols << sym_tab->get_symbol(sym_p)
      << long_symbols << ")";
}

void ast_procedurehead::print(ostream &o)
{
    o << "Procedure head (" << short_symbols << sym_tab->get_symbol(sym_p)
      << long_symbols << ")";
}

/* ostream operator for easy printing of AST nodes. */
ostream &operator<<(ostream &o, ast_node *node)
{
    if (node == NULL) {
        o << "NULL" << flush;
    } else {
        node->print(o);
    }
    return o;
}
