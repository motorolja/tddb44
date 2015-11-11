#ifndef __AST_HH__
#define __AST_HH__

#include "symtab.hh"
#include "quads.hh"

/* This little ascii diagram describes the inheritance structure of the
   AST classes:

AST_NODE
  |
  +- AST_ELSIF
  |
  +- AST_STMT_LIST
  |
  +- AST_EXPR_LIST
  |
  +- AST_ELSIF_LIST
  |
  +- AST_FUNCTIONHEAD
  |
  +- AST_PROCEDUREHEAD
  |
  +- AST_STATEMENT
  |  |
  |  +- AST_PROCEDURECALL
  |  |
  |  +- AST_ASSIGN
  |  |
  |  +- AST_WHILE
  |  |
  |  +- AST_IF
  |  |
  |  +- AST_RETURN
  |
  +- AST_EXPRESSION
     |
     +- AST_LVALUE
     |  |
     |  +- AST_ID
     |  |
     |  +- AST_INDEXED
     |
     +- AST_BINARYRELATION
     |  |
     |  +- AST_EQUAL
     |  |
     |  +- AST_NOTEQUAL
     |  |
     |  +- AST_LESSTHAN
     |  |
     |  +- AST_GREATERTHAN
     |
     +- AST_BINARYOPERATION
     |  |
     |  +- AST_ADD
     |  |
     |  +- AST_SUB
     |  |
     |  +- AST_OR
     |  |
     |  +- AST_AND
     |  |
     |  +- AST_MULT
     |  |
     |  +- AST_DIVIDE
     |  |
     |  +- AST_IDIV
     |  |
     |  +- AST_MOD
     |
     +- AST_FUNCTIONCALL
     |
     +- AST_UMINUS
     |
     +- AST_NOT
     |
     +- AST_INTEGER
     |
     +- AST_REAL
     |
     +- AST_CAST
     |
     +- AST_PARAMETER
*/


/* The various types of AST node tags that can appear. If C++ had had an
   instanceof operator like Java, we wouldn't need this... These tags are
   set in the node constructor and should never be changed afterwards. */
enum ast_node_types {
    AST_NODE,
    AST_STATEMENT,
    AST_EXPRESSION,
    AST_BINARYOPERATION,
    AST_BINARYRELATION,
    AST_LVALUE,
    AST_EXPR_LIST,
    AST_STMT_LIST,
    AST_ELSIF_LIST,
    AST_ID,
    AST_INDEXED,
    AST_ADD,
    AST_SUB,
    AST_OR,
    AST_AND,
    AST_MULT,
    AST_DIVIDE,
    AST_IDIV,
    AST_MOD,
    AST_EQUAL,
    AST_NOTEQUAL,
    AST_LESSTHAN,
    AST_GREATERTHAN,
    AST_PROCEDURECALL,
    AST_ASSIGN,
    AST_WHILE,
    AST_IF,
    AST_RETURN,
    AST_FUNCTIONCALL,
    AST_UMINUS,
    AST_NOT,
    AST_ELSIF,
    AST_INTEGER,
    AST_REAL,
    AST_FUNCTIONHEAD,
    AST_PROCEDUREHEAD,
    AST_PARAMETER,
    AST_CAST
};
typedef enum ast_node_types ast_node_type;


// Defined in symtab.cc.
extern symbol_table *sym_tab;


/* Needed so we can refer to quad_list& as arguments. See below. */
class quad_list;


/* Class stubs to allow referencing the classes below before they're declared.
   See below. */
class ast_binaryoperation;
class ast_stmt_list;
class ast_id;
class ast_integer;
class ast_real;
class ast_cast;

class quad_list;



/*** Abstract classes ***/

/* Base class for all ast nodes. */
class ast_node
{
protected:
    // Used for AST printing.
    static int indent_level;
    static bool branches[10000];

    // All these methods are concerned with printing the AST.
    void indent(ostream &);
    
    void indent_more();

    void indent_less();

    void begin_child(ostream &);

    void end_child(ostream &);

    void last_child(ostream &);

    virtual void print(ostream &);

    virtual void xprint(ostream &, string);

public:
    // Holds line and column number for this node.
    position_information *pos;

    // Describes what kind of node this is. We need to be able to check this
    // in a convenient way during AST optimization.
    ast_node_type tag;

    // Constructor.
    ast_node(position_information *);

    // Perform type checking. See semantic.cc for the method bodies.
    // Note that it's an error to call type_check in this class. It should
    // only be called in the concrete AST nodes, see below.
    virtual sym_index type_check();

    // Perform optimization. See optimize.cc for the method bodies.
    // Just as with type checking, it's an error to call optimize in this
    // class. It should only be called in the concrete AST nodes.
    virtual void optimize();

    // Generate quads. See quads.cc for the method bodies. Like type checking,
    // generate_quads should only be called in concrete AST nodes. See below.
    virtual sym_index generate_quads(quad_list &) = 0;

    // Allow an AST node to be sent to an outstream for printing.
    friend ostream &operator<<(ostream &, ast_node *);
};



/* Base class for nodes that don't have a return value of any kind. */
class ast_statement : public ast_node
{
protected:
    virtual void print(ostream &);
public:
    // Constructor.
    ast_statement(position_information *);

    // It's an error if these methods are called. See the derived classes.
    virtual sym_index type_check();

    virtual void optimize();

    virtual sym_index generate_quads(quad_list &) = 0;
};



/* Base class for nodes that have a return value. */
class ast_expression : public ast_node
{
protected:
    virtual void print(ostream &);
public:
    // The return type of this expression.
    sym_index type;

    // Constructors.
    ast_expression(position_information *);

    ast_expression(position_information *, sym_index);

    // It's an error if these methods are called. See the derived classes.
    virtual sym_index type_check();

    virtual void optimize();

    virtual sym_index generate_quads(quad_list &) = 0;

    // Used for safe downcasting. We could provide a mechanism to safely
    // downcast ALL ast nodes... But these ones are the only ones we'll need
    // in this lab course. They will be used during AST optimization.
    // Note: It is not illegal to call these methods in other nodes than the
    // ast_integer, ast_real - but those nodes are the only ones which will
    // return non-NULL values.
    virtual ast_integer *get_ast_integer() {
        return NULL;
    }

    virtual ast_real *get_ast_real() {
        return NULL;
    }

    virtual ast_id *get_ast_id() {
        return NULL;
    }

    virtual ast_cast *get_ast_cast() {
        return NULL;
    }

    // This, however, is very illegal. It's also only used in optimize.cc, to
    // allow us to downcast an ast_expression to an ast_binaryoperation.
    // See the comments in that file for more information.
    virtual ast_binaryoperation *get_ast_binaryoperation() {
        fatal("Illegal downcast to ast_binaryoperation from ast_expression");
        return NULL;
    }
};



/* Base class for all binary relation nodes. "a < b", etc.
   Note: the left and right operands are defined here instead of in the
   individual subclasses, making those rather trivial. */
class ast_binaryrelation : public ast_expression
{
protected:
    virtual void print(ostream &);

    virtual void xprint(ostream &, string);
public:
    // Left/right children of the relation.
    ast_expression *left;

    ast_expression *right;

    // Constructor.
    ast_binaryrelation(position_information *,
                       ast_expression *,
                       ast_expression *);

    // It's an error if this method is called. See the derived classes.
    virtual sym_index type_check();

    virtual void optimize();

    virtual sym_index generate_quads(quad_list &) = 0;
};



/* Base class for all binary operation nodes. "a + b", etc.
   Note: the left and right operands are defined here instead of in the
   individual subclasses, making those rather trivial. */
class ast_binaryoperation : public ast_expression
{
protected:
    virtual void print(ostream &);

    virtual void xprint(ostream &, string);
public:
    // Left/right children of the operation.
    ast_expression *left;

    ast_expression *right;

    // Constructor.
    ast_binaryoperation(position_information *,
                        ast_expression *,
                        ast_expression *);

    // It's an error if these methods are called. See the derived classes.
    virtual sym_index type_check();

    virtual void optimize();

    virtual sym_index generate_quads(quad_list &) = 0;

    // Needed for safe downcasting.
    virtual ast_binaryoperation *get_ast_binaryoperation() {
        fatal("Illegal downcast to ast_binaryoperation");
        return NULL;
    }
};



/* Base class for all identifiers that can appear as lvalues, currently
   ast_id and ast_indexed. */
class ast_lvalue : public ast_expression
{
protected:
    virtual void print(ostream &);
public:
    // Constructors.
    ast_lvalue(position_information *);

    ast_lvalue(position_information *, sym_index);

    // It's an error if this method is called. See the derived classes.
    virtual sym_index type_check();

    virtual void optimize();

    virtual sym_index generate_quads(quad_list &) = 0;

    virtual void generate_assignment(quad_list &, sym_index) = 0;
};




/*** Concrete classes - these nodes actually appear in the AST. ***/


/*** Classes derived from ast_node ***/

/* Elsif node. It's a bit special, so it inherits directly from ast_node. */
class ast_elsif : public ast_node
{
protected:
    virtual void print(ostream &);
public:
    // The test condition.
    ast_expression *condition;

    // The body.
    ast_stmt_list *body;

    // Constructor.
    ast_elsif(position_information *, ast_expression *, ast_stmt_list *);

    // Perform type checking.
    virtual sym_index type_check();

    // AST optimization.
    virtual void optimize();

    // Quad generation.
    virtual sym_index generate_quads(quad_list &);

    virtual void generate_quads_and_jump(quad_list &, int);
};



/* Contains a list of expressions. Currently only used for parameter lists.
   Note: The parameters will be stored in reverse order! This is due to how the
         grammar is written, it's easiest this way. */
class ast_expr_list : public ast_node
{
protected:
    virtual void print(ostream &);
public:
    // Points to the last (last added) expression in the list.
    ast_expression *last_expr;

    // Points to a list of the preceding expressions.
    ast_expr_list *preceding;

    // Constructors.
    ast_expr_list(position_information *, ast_expression *);

    ast_expr_list(position_information *, ast_expression *, ast_expr_list *);

    // Perform type checking.
    virtual sym_index type_check();

    // AST optimization.
    virtual void optimize();

    // Quad generation.
    virtual sym_index generate_quads(quad_list &);

    virtual void generate_parameter_list(quad_list &,
            parameter_symbol *,
            int *);
};



/* Contains a list of statements. */
class ast_stmt_list : public ast_node
{
protected:
    virtual void print(ostream &);
public:
    // Points to the last statement in the list.
    ast_statement *last_stmt;

    // Points to a list of the preceding statements.
    ast_stmt_list *preceding;

    // Constructors.
    ast_stmt_list(position_information *, ast_statement *);

    ast_stmt_list(position_information *, ast_statement *, ast_stmt_list *);

    // Perform type checking.
    virtual sym_index type_check();

    // AST optimization.
    virtual void optimize();

    // Quad generation.
    virtual sym_index generate_quads(quad_list &);
};



/* Contains a list of elsif clauses. */
class ast_elsif_list : public ast_node
{
protected:
    virtual void print(ostream &);
public:
    // Points to the last elsif clause in the list.
    ast_elsif *last_elsif;

    // Points to a list of the preceding elsif clauses.
    ast_elsif_list *preceding;

    // Constructors.
    ast_elsif_list(position_information *, ast_elsif *);

    ast_elsif_list(position_information *, ast_elsif *, ast_elsif_list *);

    // Perform type checking.
    virtual sym_index type_check();

    // AST optimization.
    virtual void optimize();

    // Quad generation.
    virtual sym_index generate_quads(quad_list &);

    virtual void generate_quads_and_jump(quad_list &, int);
};


/* A node used to transfer information about an environment. Used in parser.y
   for setting the proper return type of a function, and type checking.
   It is never part of a function body. */
class ast_functionhead : public ast_node
{
protected:
    virtual void print(ostream &);
public:
    // Pointer to the function in the symbol table.
    sym_index sym_p;

    // Constructor.
    ast_functionhead(position_information *, sym_index);

    // Only here since we're using abstract virtual methods in ast_node.
    virtual void optimize();

    virtual sym_index generate_quads(quad_list &);

    // This method is called by parser.y when quad generation is to start.
    // The argument is the function body to be generated.
    quad_list *do_quads(ast_stmt_list *);
};


/* A node used to transfer information about an environment. Used in parser.y
   for setting the proper return type of a function. It is never part of a
   procedure body. */
class ast_procedurehead : public ast_node
{
protected:
    virtual void print(ostream &);
public:
    // Pointer to the procedure in the symbol table.
    sym_index sym_p;

    // Constructor.
    ast_procedurehead(position_information *, sym_index);

    // Only here since we're using abstract virtual methods in ast_node.
    virtual void optimize();

    virtual sym_index generate_quads(quad_list &);

    // This method is called by parser.y when quad generation is to start.
    // The argument is the procedure body to be generated.
    quad_list *do_quads(ast_stmt_list *);
};



/*** Classes derived from ast_statement ***/

/* Represents a procedure call. Example: "print(foo);" */
class ast_procedurecall : public ast_statement
{
protected:
    virtual void print(ostream &);
public:
    // The procedure's id node, contains a link to the symbol table.
    ast_id *id;

    // A list of eventual parameters. If no parameters, this list is NULL.
    ast_expr_list *parameter_list;

    // Constructor.
    ast_procedurecall(position_information *, ast_id *, ast_expr_list *);

    // Perform type checking.
    virtual sym_index type_check();

    // AST optimization.
    virtual void optimize();

    // Quad generation.
    virtual sym_index generate_quads(quad_list &);
};



/* Assignment node. a := b or a[i] = b. */
class ast_assign : public ast_statement
{
protected:
    virtual void print(ostream &);
public:
    // The left hand side (lhs), ie, the variable being assigned to.
    ast_lvalue *lhs;

    // The right hand side (rhs), ie, the value being assigned.
    ast_expression *rhs;

    // Constructor.
    ast_assign(position_information *, ast_lvalue *, ast_expression *);

    // Perform type checking.
    virtual sym_index type_check();

    // AST optimization.
    virtual void optimize();

    // Quad generation.
    virtual sym_index generate_quads(quad_list &);
};



/* A while loop. Contains a test condition and a loop body. */
class ast_while : public ast_statement
{
protected:
    virtual void print(ostream &);
public:
    // The test condition.
    ast_expression *condition;

    // The loop body.
    ast_stmt_list *body;

    // Constructor.
    ast_while(position_information *, ast_expression *, ast_stmt_list *);

    // Perform type checking.
    virtual sym_index type_check();

    // AST optimization.
    virtual void optimize();

    // Quad generation.
    virtual sym_index generate_quads(quad_list &);
};



/* An if clause. Has lots of children. if - then - elsif - else. */
class ast_if : public ast_statement
{
protected:
    virtual void print(ostream &);
public:
    // The primary if condition.
    ast_expression *condition;

    // The primary if body, executed if 'condition' evaluates to non-zero.
    ast_stmt_list *body;

    // A list of elsif clauses. Can be NULL.
    ast_elsif_list *elsif_list;

    // The else body, executed if 'condition' evaluates to zero. Can be NULL.
    ast_stmt_list *else_body;

    // Constructor.
    ast_if(position_information *,
           ast_expression *,
           ast_stmt_list *,
           ast_elsif_list *,
           ast_stmt_list *);

    // Perform type checking.
    virtual sym_index type_check();

    // AST optimization.
    virtual void optimize();

    // Quad generation.
    virtual sym_index generate_quads(quad_list &);
};



/* A return statement, used both for procedures and functions.
   If no value is returned, 'value' should be set to NULL. */
class ast_return : public ast_statement
{
protected:
    virtual void print(ostream &);
public:
    // The return value.
    ast_expression *value;

    // Constructor for no return value.
    ast_return(position_information *);

    // Constructor with a return value.
    ast_return(position_information *, ast_expression *);

    // Perform type checking.
    virtual sym_index type_check();

    // AST optimization.
    virtual void optimize();

    // Quad generation.
    virtual sym_index generate_quads(quad_list &);
};



/*** Classes derived from ast_expression ***/

/* Represents a function call. a = calc(foo); */
class ast_functioncall : public ast_expression
{
protected:
    virtual void print(ostream &);
public:
    // The function's id node, contains a link to the symbol table.
    ast_id *id;

    // A list of actual parameters. If no parameters, this list is NULL.
    ast_expr_list *parameter_list;

    // Constructor.
    ast_functioncall(position_information *, ast_id *, ast_expr_list *);

    // Perform type checking.
    virtual sym_index type_check();

    // AST optimization.
    virtual void optimize();

    // Quad generation.
    virtual sym_index generate_quads(quad_list &);
};



/* A unary minus node. */
class ast_uminus : public ast_expression
{
protected:
    virtual void print(ostream &);
public:
    // The inverted expression.
    ast_expression *expr;

    // Constructor.
    ast_uminus(position_information *, ast_expression *);

    // Perform type checking.
    virtual sym_index type_check();

    // AST optimization.
    virtual void optimize();

    // Quad generation.
    virtual sym_index generate_quads(quad_list &);
};


/* A logical negation node. */
class ast_not : public ast_expression
{
protected:
    virtual void print(ostream &);
public:
    // The expression being negated.
    ast_expression *expr;

    // Constructor.
    ast_not(position_information *, ast_expression *);

    // Perform type checking.
    virtual sym_index type_check();

    // AST optimization.
    virtual void optimize();

    // Quad generation.
    virtual sym_index generate_quads(quad_list &);
};


/* An integer node. Represents an integer number, like '5'. */
class ast_integer : public ast_expression
{
protected:
    virtual void print(ostream &);
public:
    // The integer value of the node.
    long value;

    // Constructor.
    ast_integer(position_information *, long);

    // Perform type checking.
    virtual sym_index type_check();

    // AST optimization.
    virtual void optimize();

    // Quad generation.
    virtual sym_index generate_quads(quad_list &);

    // Safe downcasting.
    virtual ast_integer *get_ast_integer() {
        return this;
    }
};


/* A real node. Represents a real number, like '2.5'. */
class ast_real : public ast_expression
{
protected:
    virtual void print(ostream &);
public:
    // The real value of the node.
    double value;

    // Constructor.
    ast_real(position_information *, double);

    // Perform type checking.
    virtual sym_index type_check();

    // AST optimization.
    virtual void optimize();

    // Quad generation.
    virtual sym_index generate_quads(quad_list &);

    // Safe downcasting.
    virtual ast_real *get_ast_real() {
        return this;
    }
};



/* A cast node. Inserted into the AST at appropriate places during type
   checking. See semantic.cc. */
class ast_cast : public ast_expression
{
protected:
    virtual void print(ostream &);
public:
    // The expression we've casted to real.
    ast_expression *expr;

    // Constructor.
    ast_cast(position_information *, ast_expression *);

    // AST optimization.
    virtual void optimize();

    // Quad generation.
    virtual sym_index generate_quads(quad_list &);

    // Safe downcasting.
    virtual ast_cast *get_ast_cast() {
        return this;
    }
};



/*** Classes derived from ast_binaryrelation ***/

/* Equality operator. a = b. */
class ast_equal : public ast_binaryrelation
{
protected:
    virtual void print(ostream &);
public:
    // Constructor.
    ast_equal(position_information *, ast_expression *, ast_expression *);

    // Perform type checking.
    virtual sym_index type_check();

    // AST optimization.
    virtual void optimize();

    // Quad generation.
    virtual sym_index generate_quads(quad_list &);
};


/* Not-equal operator. a <> b. */
class ast_notequal : public ast_binaryrelation
{
protected:
    virtual void print(ostream &);
public:
    // Constructor.
    ast_notequal(position_information *, ast_expression *, ast_expression *);

    // Perform type checking.
    virtual sym_index type_check();

    // AST optimization.
    virtual void optimize();

    // Quad generation.
    virtual sym_index generate_quads(quad_list &);
};


/* Less than operator. a < b. */
class ast_lessthan : public ast_binaryrelation
{
protected:
    virtual void print(ostream &);
public:
    // Constructor.
    ast_lessthan(position_information *, ast_expression *, ast_expression *);

    // Perform type checking.
    virtual sym_index type_check();

    // AST optimization.
    virtual void optimize();

    // Quad generation.
    virtual sym_index generate_quads(quad_list &);
};


/* Greater than operator. a > b. */
class ast_greaterthan : public ast_binaryrelation
{
protected:
    virtual void print(ostream &);
public:
    // Constructor.
    ast_greaterthan(position_information *, ast_expression *, ast_expression *);

    // Perform type checking.
    virtual sym_index type_check();

    // AST optimization.
    virtual void optimize();

    // Quad generation.
    virtual sym_index generate_quads(quad_list &);
};




/*** Classes derived from ast_binaryoperation ***/

/* Plus node. a + b. */
class ast_add : public ast_binaryoperation
{
protected:
    virtual void print(ostream &);
public:
    // Constructor.
    ast_add(position_information *, ast_expression *, ast_expression *);

    // Perform type checking.
    virtual sym_index type_check();

    // AST optimization.
    virtual void optimize();

    // Quad generation.
    virtual sym_index generate_quads(quad_list &);

    // Safe downcasts.
    virtual ast_add *get_ast_binaryoperation() {
        return this;
    }
};


/* Minus node. a - b. */
class ast_sub : public ast_binaryoperation
{
protected:
    virtual void print(ostream &);
public:
    // Constructor.
    ast_sub(position_information *, ast_expression *, ast_expression *);

    // Perform type checking.
    virtual sym_index type_check();

    // AST optimization.
    virtual void optimize();

    // Quad generation.
    virtual sym_index generate_quads(quad_list &);

    // Safe downcasts.
    virtual ast_sub *get_ast_binaryoperation() {
        return this;
    }
};


/* Logical OR node. a OR b. */
class ast_or : public ast_binaryoperation
{
protected:
    virtual void print(ostream &);
public:
    // Constructor.
    ast_or(position_information *, ast_expression *, ast_expression *);

    // Perform type checking.
    virtual sym_index type_check();

    // AST optimization.
    virtual void optimize();

    // Quad generation.
    virtual sym_index generate_quads(quad_list &);

    // Safe downcasts.
    virtual ast_or *get_ast_binaryoperation() {
        return this;
    }
};


/* Logical AND node. a AND b. */
class ast_and : public ast_binaryoperation
{
protected:
    virtual void print(ostream &);
public:
    // Constructor.
    ast_and(position_information *, ast_expression *, ast_expression *);

    // Perform type checking.
    virtual sym_index type_check();

    // AST optimization.
    virtual void optimize();

    // Quad generation.
    virtual sym_index generate_quads(quad_list &);

    // Safe downcasts.
    virtual ast_and *get_ast_binaryoperation() {
        return this;
    }
};


/* Multiplication node. a * b. */
class ast_mult : public ast_binaryoperation
{
protected:
    virtual void print(ostream &);
public:
    // Constructor.
    ast_mult(position_information *, ast_expression *, ast_expression *);

    // Perform type checking.
    virtual sym_index type_check();

    // AST optimization.
    virtual void optimize();

    // Quad generation.
    virtual sym_index generate_quads(quad_list &);

    // Safe downcasts.
    virtual ast_mult *get_ast_binaryoperation() {
        return this;
    }
};


/* Real division node. a / b, where at least one of a and b have real type. */
class ast_divide : public ast_binaryoperation
{
protected:
    virtual void print(ostream &);
public:
    // Constructor.
    ast_divide(position_information *, ast_expression *, ast_expression *);

    // Perform type checking.
    virtual sym_index type_check();

    // AST optimization.
    virtual void optimize();

    // Quad generation.
    virtual sym_index generate_quads(quad_list &);

    // Safe downcasts.
    virtual ast_divide *get_ast_binaryoperation() {
        return this;
    }
};


/* Integer division node. a div b, where both operands have integer type. */
class ast_idiv : public ast_binaryoperation
{
protected:
    virtual void print(ostream &);
public:
    // Constructor.
    ast_idiv(position_information *, ast_expression *, ast_expression *);

    // Perform type checking.
    virtual sym_index type_check();

    // AST optimization.
    virtual void optimize();

    // Quad generation.
    virtual sym_index generate_quads(quad_list &);

    // Safe downcasts.
    virtual ast_idiv *get_ast_binaryoperation() {
        return this;
    }
};


/* Integer mod node. a mod b, where both operands have integer type. */
class ast_mod : public ast_binaryoperation
{
protected:
    virtual void print(ostream &);
public:
    // Constructor.
    ast_mod(position_information *, ast_expression *, ast_expression *);

    // Perform type checking.
    virtual sym_index type_check();

    // AST optimization.
    virtual void optimize();

    // Quad generation.
    virtual sym_index generate_quads(quad_list &);

    // Safe downcasts.
    virtual ast_mod *get_ast_binaryoperation() {
        return this;
    }
};



/* An identifier node. Can be the name of a variable, function, constant,
   etc... */
class ast_id : public ast_lvalue
{
protected:
    virtual void print(ostream &);
public:
    // A symbol table index for this symbol.
    sym_index sym_p;

    // Constructors.
    ast_id(position_information *);

    ast_id(position_information *, sym_index);

    // Perform type checking.
    virtual sym_index type_check();

    // AST optimization.
    virtual void optimize();

    // Quad generation.
    virtual sym_index generate_quads(quad_list &);

    virtual void generate_assignment(quad_list &, sym_index);

    // Safe downcasting.
    virtual ast_id *get_ast_id() {
        return this;
    }
};



/* An array identifier node. Index must be of integer type. */
class ast_indexed : public ast_lvalue
{
protected:
    virtual void print(ostream &);
public:
    // The array's id node, which contains a link into the symbol table.
    ast_id *id;

    // The index expression. Must be of type integer.
    ast_expression *index;

    // Constructor.
    ast_indexed(position_information *, ast_id *, ast_expression *);

    // Perform type checking.
    virtual sym_index type_check();

    // AST optimization.
    virtual void optimize();

    // Quad generation.
    virtual sym_index generate_quads(quad_list &);

    virtual void generate_assignment(quad_list &, sym_index);
};



/* Allow a node to be sent to an outstream for printing. */
ostream &operator<<(ostream &, ast_node *);


#endif
