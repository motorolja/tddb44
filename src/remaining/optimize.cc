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
long ast_binaryrelation::calculate_int(long arg1, long arg2)
{
    fatal("Trying to calculate abstract class ast_binrelation.");
    return 0;
}
long ast_binaryrelation::calculate_real(double arg1, double arg2)
{
    fatal("Trying to calculate abstract class ast_binrelation.");
    return 0;
}
long ast_binaryoperation::calculate_int(long arg1, long arg2)
{
    fatal("Trying to calculate abstract class ast_binoperation.");
    return 0;
}
double ast_binaryoperation::calculate_real(double arg1, double arg2)
{
    fatal("Trying to calculate abstract class ast_binoperation.");
    return 0;
}

long ast_unaryoperation::calculate_int(long arg)
{
    fatal("Trying to calculate abstract class ast_unaryoperation.");
    return 0;
}
double ast_unaryoperation::calculate_real(double arg)
{
    fatal("Trying to calculate abstract class ast_unaryoperation.");
    return 0;
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
    switch(node->tag){
        case AST_INDEXED:
        case AST_FUNCTIONCALL:
        case AST_INTEGER:
        case AST_REAL:
            return node;
        case AST_ID:{
            symbol *id_sym = sym_tab->get_symbol(node->get_ast_id()->sym_p);
            if (id_sym->tag == SYM_CONST){
                if(node->type == integer_type){
                    long val_i = id_sym->get_constant_symbol()->const_value.ival;
                    return new ast_integer(node->pos, val_i);
                }else
                if(node->type == real_type){
                    double val_r = id_sym->get_constant_symbol()->const_value.rval;
                    return new ast_real(node->pos, val_r);
                }else
                    fatal("strange id type");
                    return NULL;
            }else
                // could not optimize
                return node;
        }
        case AST_CAST:{
            ast_cast *node_cast = node->get_ast_cast();
            ast_integer *node_num_int = node_cast->expr->get_ast_integer();
            if(node_num_int != NULL)
                return new ast_real(node->pos, node_num_int->value);
            else
                // could not optimze
                return node;
        }
        case AST_ADD:
        case AST_SUB:
        case AST_MULT:
        case AST_DIVIDE:
        case AST_OR:
        case AST_AND:
        case AST_IDIV:
        case AST_MOD: {
            ast_binaryoperation *binop = node->get_ast_binaryoperation();
            if(binop->type == integer_type){
                ast_integer *left_int  = binop->left->get_ast_integer();
                ast_integer *right_int = binop->right->get_ast_integer();
                if(left_int != NULL && right_int != NULL){
                    return new ast_integer(node->pos, binop->calculate_int(
                                                            left_int->value,
                                                            right_int->value));
                }else
                    // could not optimize
                    return node;
            }else
            if(binop->type == real_type){
                ast_real *left_real  = binop->left->get_ast_real();
                ast_real *right_real = binop->right->get_ast_real();
                if(left_real != NULL && right_real != NULL){
                    return new ast_real(node->pos, binop->calculate_real(
                                                            left_real->value,
                                                            right_real->value));
                }else
                    // could not optimize
                    return node;
            }else
                fatal("strange binary operation type");
                return NULL;
        }
        case AST_EQUAL:
        case AST_NOTEQUAL:
        case AST_LESSTHAN:
        case AST_GREATERTHAN: {
            ast_binaryrelation *binrel = node->get_ast_binaryrelation();
            if(binrel->type == integer_type){
                ast_integer *left_int  = binrel->left->get_ast_integer();
                ast_integer *right_int = binrel->right->get_ast_integer();
                if(left_int != NULL && right_int != NULL){
                    return new ast_integer(node->pos, binrel->calculate_int(
                                                            left_int->value,
                                                            right_int->value));
                }else
                    // could not optimize
                    return node;
            }else
            if(binrel->type == real_type){
                ast_real *left_real  = binrel->left->get_ast_real();
                ast_real *right_real = binrel->right->get_ast_real();
                if(left_real != NULL && right_real != NULL){
                    return new ast_integer(node->pos, binrel->calculate_real(
                                                            left_real->value,
                                                            right_real->value));
                }else
                    // could not optimize
                    return node;
            }else
                //something went wrong
                fatal("strange binary operation type");
                return NULL;
        }
        case AST_UMINUS:
        case AST_NOT:{
            ast_unaryoperation *unop = node->get_ast_unaryoperation();
            if(unop->type == integer_type){
                ast_integer *expr_int  = unop->expr->get_ast_integer();
                if(expr_int != NULL){
                    return new ast_integer(node->pos, unop->calculate_int(
                                                            expr_int->value));
                }else
                    // could not optimize
                    return node;
            }else
            if(unop->type == real_type){
                ast_real *expr_real = unop->expr->get_ast_real();
                if(expr_real != NULL){
                    return new ast_integer(node->pos, unop->calculate_real(
                                                            expr_real->value));
                }else
                    // could not optimize
                    return node;
            }else
                //something went wrong
                fatal("strange binary operation type");
                return NULL;
        }
        default:{
            fatal("what is this");
            return NULL;
        }
    }
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

void ast_binaryrelation::optimize()
{
    left  = optimizer->fold_constants(left);
    right = optimizer->fold_constants(right);
}

void ast_unaryoperation::optimize()
{
    expr  = optimizer->fold_constants(expr);
}

/* We can apply constant folding to binary relations as well. */

long ast_add::calculate_int(long arg1, long arg2)
{
    return arg1 + arg2;
}
double ast_add::calculate_real(double arg1, double arg2)
{
    return arg1 + arg2;
}

long ast_sub::calculate_int(long arg1, long arg2)
{
    return arg1 - arg2;
}
double ast_sub::calculate_real(double arg1, double arg2)
{
    return arg1 - arg2;
}

long ast_or::calculate_int(long arg1, long arg2)
{
    return arg1 || arg2;
}

long ast_and::calculate_int(long arg1, long arg2)
{
    return arg1 && arg2;
}

long ast_mult::calculate_int(long arg1, long arg2)
{
    return arg1 * arg2;
}
double ast_mult::calculate_real(double arg1, double arg2)
{
    return arg1 * arg2;
}

long ast_divide::calculate_int(long arg1, long arg2)
{
    return arg1 / arg2;
}
double ast_divide::calculate_real(double arg1, double arg2)
{
    return arg1 / arg2;
}

long ast_idiv::calculate_int(long arg1, long arg2)
{
    return arg1 / arg2;
}

long ast_mod::calculate_int(long arg1, long arg2)
{
    return arg1 % arg2;
}

// relations
long ast_equal::calculate_int(long arg1, long arg2)
{
    return arg1 == arg2;
}
long ast_equal::calculate_real(double arg1, double arg2)
{
    return arg1 == arg2;
}

long ast_notequal::calculate_int(long arg1, long arg2)
{
    return (arg1 != arg2);
}
long ast_notequal::calculate_real(double arg1, double arg2)
{
    return arg1 != arg2;
}

long ast_lessthan::calculate_int(long arg1, long arg2)
{
    return arg1 < arg2;
}
long ast_lessthan::calculate_real(double arg1, double arg2)
{
    return arg1 < arg2;
}

long ast_greaterthan::calculate_int(long arg1, long arg2)
{
    return arg1 > arg2;
}
long ast_greaterthan::calculate_real(double arg1, double arg2)
{
    return arg1 > arg2;
}

double ast_uminus::calculate_real(double arg){
    return (-1)* arg;
}
long ast_uminus::calculate_int(long arg){
    return (-1)* arg;
}

long ast_not::calculate_int(long arg){
    return (! arg);
}




/*** The various classes derived from ast_statement. ***/

void ast_procedurecall::optimize()
{
    parameter_list->optimize();
}


void ast_assign::optimize()
{
    rhs = optimizer->fold_constants(rhs);
}


void ast_while::optimize()
{
    condition = optimizer->fold_constants(condition);
    body->optimize();
}


void ast_if::optimize()
{
    condition = optimizer->fold_constants(condition);
    if(body != NULL)
        body->optimize();
    if(elsif_list != NULL)
        elsif_list->optimize();
    if(else_body != NULL)
        else_body->optimize();
}


void ast_return::optimize()
{
    value = optimizer->fold_constants(value);
}


void ast_functioncall::optimize()
{
    parameter_list->optimize();
}



void ast_elsif::optimize()
{
    condition = optimizer->fold_constants(condition);
    body->optimize();
}



void ast_integer::optimize()
{
    // nothing todo
}

void ast_real::optimize()
{
    // nothing todo
}

/* Note: See the comment in fold_constants() about casts and folding. */
void ast_cast::optimize()
{
    expr = optimizer->fold_constants(expr);
}

void ast_procedurehead::optimize()
{
    fatal("Trying to call ast_procedurehead::optimize()");
}


void ast_functionhead::optimize()
{
    fatal("Trying to call ast_functionhead::optimize()");
}
