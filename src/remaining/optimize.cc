#include "optimize.hh"

/*** This file contains all code pertaining to AST optimisation. It currently
     implements a simple optimisation called "constant folding". Most of the
     methods in this file are empty, or just relay optimize calls downward
     in the AST. If a more powerful AST optimization scheme were to be
     implemented, only methods in this file should need to be changed. ***/


ast_optimizer *optimizer = new ast_optimizer();


/* The optimizer's interface method. Starts a recursive optimize call down
   the AST nodes, searching for binary operators with constant children. */
void ast_optimizer::do_optimize(ast_stmt_list *body)
{
    if (body != NULL) {
        body->optimize();
    }
}


/* Returns 1 if an AST expression is a subclass of ast_binaryoperation,
   ie, eligible for constant folding. */
bool ast_optimizer::is_binop(ast_expression *node)
{
    switch (node->tag) {
    case AST_ADD:
    case AST_SUB:
    case AST_OR:
    case AST_AND:
    case AST_MULT:
    case AST_DIVIDE:
    case AST_IDIV:
    case AST_MOD:
        return true;
    default:
        return false;
    }
}

/* Returns 1 if an AST expression is a subclass of ast_binaryoperation,
   ie, eligible for constant folding. */
bool ast_optimizer::is_binrel(ast_expression *node)
{
    switch (node->tag) {
    case AST_EQUAL:
    case AST_NOTEQUAL:
    case AST_LESSTHAN:
    case AST_GREATERTHAN:
        return true;
    default:
        return false;
    }
}

bool ast_optimizer::is_const(ast_expression *node)
{
    switch (node->tag){
    case AST_INTEGER:
    case AST_REAL:
        return true;
    case AST_ID:
        return sym_tab->get_symbol_tag(node->get_ast_id()->sym_p) == SYM_CONST;
    default:
        return false;
    }
}

bool ast_optimizer::is_var(ast_expression *node)
{
    // maybe we should insert here array and param aswell
    switch (node->tag){
    case AST_FUNCTIONCALL:
        return true;
    case AST_ID:
        return sym_tab->get_symbol_tag(node->get_ast_id()->sym_p) == SYM_VAR;
    default:
        return false;
    }
}

/* We overload this method for the various ast_node subclasses that can
   appear in the AST. By use of virtual (dynamic) methods, we ensure that
   the correct method is invoked even if the pointers in the AST refer to
   one of the abstract classes such as ast_expression or ast_statement. */
void ast_node::optimize()
{
    fatal("Trying to optimize abstract class ast_node.");
}

void ast_statement::optimize()
{
    fatal("Trying to optimize abstract class ast_statement.");
}

void ast_expression::optimize()
{
    fatal("Trying to optimize abstract class ast_expression.");
}

void ast_lvalue::optimize()
{
    fatal("Trying to optimize abstract class ast_lvalue.");
}

void ast_binaryrelation::optimize()
{
    fatal("Trying to optimize abstract class ast_binaryrelation.");
}



/*** The optimize methods for the concrete AST classes. ***/

/* Optimize a statement list. */
void ast_stmt_list::optimize()
{
    if (preceding != NULL) 
        preceding->optimize();
    if (last_stmt != NULL) 
        last_stmt->optimize();
}


/* Optimize a list of expressions. */
void ast_expr_list::optimize()
{
    if(last_expr != NULL)
        last_expr->optimize();
    if(preceding != NULL)
        last_expr = optimizer->fold_constants(last_expr);
}


/* Optimize an elsif list. */
void ast_elsif_list::optimize()
{
    if(last_elsif != NULL)
        last_elsif->optimize();
    if(preceding != NULL)
        preceding->optimize();
}


/* An identifier's value can change at run-time, so we can't perform
   constant folding optimization on it unless it is a constant.
   Thus we just do nothing here. It can be treated in the fold_constants()
   method, however. */
void ast_id::optimize()
{
}

void ast_indexed::optimize()
{
    index = optimizer->fold_constants(index);
}



/* This convenience method is used to apply constant folding to all
   binary operations. It returns either the resulting optimized node or the
   original node if no optimization could be performed. */
ast_expression *ast_optimizer::fold_constants(ast_expression *node)
{
    node->optimize();
    if(optimizer->is_var(node)){
        return  node;
    }else
    if(optimizer->is_const(node)){
        
    }else
    if(optimzer->is_binop(node)){
        ast_binaryoperation *binop = node->get_ast_binop();
        if(binop->type == integer_type){
            ast_integer *left_int  = ast_binop->left->get_ast_integer();
            ast_integer *right_int = ast_binop->right->get_ast_integer();
            if(left_int != NULL && right_int != NULL){
                return new ast_integer(node->pos, ast_binop->calculate(
                                                        right_int->alue,
                                                        left_int->value)); 
            }else
                // could not optimize
                return node;
        }else
        if(binop->type == real_type){
            ast_real *left_real  = ast_binop->left->get_ast_real();
            ast_real *right_real = ast_binop->right->get_ast_real();
            if(left_int != NULL && right_int != NULL){
                return new ast_real(node->pos, ast_binop(
                                                        right_int->value,
                                                        left_int->value)); 
            }else
                // could not optimize
                return node;
        }else
            //something went wront        
            fatal("strange binary operation type");
            return NULL        
    }else
    if (optimzer->is_binrel){
        ast_binaryrelation *binrel = node_get_ast_binrel();
        if(binrel->type == integer_type){
            ast_integer *left_int  = ast_binrel->left->get_ast_integer();
            ast_integer *right_int = ast_binrel->right->get_ast_integer();
            if(left_int != NULL && right_int != NULL){
                return new ast_integer(node->pos, ast_binrel->calculate(
                                                        right_int->alue,
                                                        left_int->value)); 
            }else
                // could not optimize
                return node;
        }else
        if(binrel->type == real_type){
            ast_real *left_real  = ast_binrel->left->get_ast_real();
            ast_real *right_real = ast_binrel->right->get_ast_real();
            if(left_int != NULL && right_int != NULL){
                return new ast_integer(node->pos, ast_binrel(
                                                        right_int->value,
                                                        left_int->value)); 
            }else
                // could not optimize
                return node;
        }else
            //something went wront        
            fatal("strange binary operation type");
            return NULL        
        
    }else
        fatal("what is this");
        return NULL;
}

/* All the binary operations should already have been detected in their parent
   nodes, so we don't need to do anything at all here. */
void ast_binaryoperation::optimize()
{
    left  = optimizer->fold_constants(left);
    right = optimizer->fold_constants(right);
}




/* We can apply constant folding to binary relations as well. */
void ast_equal::optimize()
{
    /* Your code here */
}

void ast_notequal::optimize()
{
    /* Your code here */
}

void ast_lessthan::optimize()
{
    /* Your code here */
}

void ast_greaterthan::optimize()
{
    /* Your code here */
}



/*** The various classes derived from ast_statement. ***/

void ast_procedurecall::optimize()
{
    /* Your code here */
}


void ast_assign::optimize()
{
    /* Your code here */
}


void ast_while::optimize()
{
    /* Your code here */
}


void ast_if::optimize()
{
    /* Your code here */
}


void ast_return::optimize()
{
    /* Your code here */
}


void ast_functioncall::optimize()
{
    /* Your code here */
}

void ast_uminus::optimize()
{
    /* Your code here */
}

void ast_not::optimize()
{
    /* Your code here */
}


void ast_elsif::optimize()
{
    /* Your code here */
}



void ast_integer::optimize()
{
    /* Your code here */
}

void ast_real::optimize()
{
    /* Your code here */
}

/* Note: See the comment in fold_constants() about casts and folding. */
void ast_cast::optimize()
{
    /* Your code here */
}



void ast_procedurehead::optimize()
{
    fatal("Trying to call ast_procedurehead::optimize()");
}


void ast_functionhead::optimize()
{
    fatal("Trying to call ast_functionhead::optimize()");
}
