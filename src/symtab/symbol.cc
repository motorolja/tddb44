#include "symtab.hh"

/***************************************
 *** The various symbol constructors ***
 ***************************************/


/* Symbol superclass constructor. */
symbol::symbol(pool_index pool_p)
{
    id = pool_p;
    // All symbols are tagged as SYM_UNDEF at creation.
    // This is used later to check for redeclarations.
    tag = SYM_UNDEF;
}


/* Constructor for constant_symbol. Note the somewhat weird syntax for
   invoking the superclass constructor with an argument. The effect of the
   ": symbol(pool_p)" inserted between the () and the { is to call the
   constructor for the symbol class with the argument pool_p. This syntax,
   will be used a lot in this lab course, so you might as well get used
   to it right away, even if it is new to you. */
constant_symbol::constant_symbol(const pool_index pool_p) :
    symbol(pool_p)
{
    // Since the const_value is a union of int and float (since a constant can
    // be both integer and real), we just pick one of them arbitrarily and
    // decide that all uninitiated constants have the integer value 0.
    const_value.ival = 0;
}


/* Constructor for variable_symbol. */
/* NOTE: Maybe always set the type to void_type here, too? */
variable_symbol::variable_symbol(const pool_index pool_p) :
    symbol(pool_p)
{
}


/* Constructor for array_symbol. */
array_symbol::array_symbol(const pool_index pool_p) :
    symbol(pool_p)
{
    // Illegal, must be changed later.
    index_type = void_type;
    array_cardinality = 0;
}


/* Constructor for parameter_symbol. */
parameter_symbol::parameter_symbol(const pool_index pool_p) :
    symbol(pool_p)
{
    size = 0;
    preceding = NULL;
}


/* Constructor for procedure_symbol. */
procedure_symbol::procedure_symbol(const pool_index pool_p) :
    symbol(pool_p)
{
    ar_size = 0;
    label_nr = 0;
    last_parameter = NULL;
}


/* Constructor for function_symbol. */
function_symbol::function_symbol(const pool_index pool_p) :
    symbol(pool_p)
{
    ar_size = 0;
    label_nr = 0;
    last_parameter = NULL;
}


/* Constructor for nametype_symbol. */
nametype_symbol::nametype_symbol(const pool_index pool_p) :
    symbol(pool_p)
{
}




/*** Functions for printing symbol information ***/

// This is the default detail level of information given when printing a
// symbol.
symbol::format_type symbol::output_format = symbol::LONG_FORMAT;


/* Prints information common to all symbols. The various subclasses add on
   their own info to this one, see below. */
void symbol::print(ostream &o)
{
    switch (output_format) {
    case LONG_FORMAT:
        o << "symbol:" << endl;
        o << "  id:        " << sym_tab->pool_lookup(id) << endl;
        o << "  type:      " << short_symbols
          << sym_tab->get_symbol(type) << long_symbols << endl;
        o << "  level:     " << level << endl;
        o << "  hash_link: " << hash_link << endl;
        o << "  back_link: " << back_link << endl;
        o << "  offset:    " << offset << endl;
        o << "  tag:       ";
        switch (tag) {
        case SYM_UNDEF:
            o << "SYM_UNDEF ";
            break;
        case SYM_CONST:
            o << "SYM_CONST ";
            break;
        case SYM_VAR:
            o << "SYM_VAR ";
            break;
        case SYM_ARRAY:
            o << "SYM_ARRAY ";
            break;
        case SYM_PARAM:
            o << "SYM_PARAM ";
            break;
        case SYM_PROC:
            o << "SYM_PROC ";
            break;
        case SYM_FUNC:
            o << "SYM_FUNC ";
            break;
        case SYM_NAMETYPE:
            o << "SYM_NAMETYPE ";
            break;
        }
        o << endl;
        break;
    case SUMMARY_FORMAT:
        switch (tag) {
        case SYM_UNDEF:
            o << "(SYM_UNDEF) ";
            break;
        case SYM_CONST:
            o << "(SYM_CONST) ";
            break;
        case SYM_VAR:
            o << "(SYM_VAR) ";
            break;
        case SYM_ARRAY:
            o << "(SYM_ARRAY) ";
            break;
        case SYM_PARAM:
            o << "(SYM_PARAM) ";
            break;
        case SYM_PROC:
            o << "(SYM_PROC) ";
            break;
        case SYM_FUNC:
            o << "(SYM_FUNC) ";
            break;
        case SYM_NAMETYPE:
            o << "(SYM_NAMETYPE) ";
            break;
        }
        o << sym_tab->pool_lookup(id);
        break;
    case SHORT_FORMAT:
        o << sym_tab->pool_lookup(id);
        break;
    default:
        fatal("Bad output format in symbol::print()");
        break;
    }
}


/* Output stream operator for easy printing of symbol information. */
ostream &operator<<(ostream &o, symbol *sym)
{
    if (sym == NULL) {
        return o << "(null)" << endl;
    }

    sym->print(o);
    return o;
}


/* Print data about a constant. Helps debugging a lot. See the stream operator
   though, that's the easiest way to get this info printed. Note that a
   constant can be integer _or_ real. */
void constant_symbol::print(ostream &o)
{
    symbol::print(o);
    switch (output_format) {
    case LONG_FORMAT:
        o << "  class:     constant_symbol" << endl;
        if (type == integer_type) {
            o << "  const_value.ival:" << const_value.ival << endl;
        } else {
            o << "  const_value.rval:" << const_value.rval << endl;
        }
        break;
    case SUMMARY_FORMAT:
    case SHORT_FORMAT:
        break;
    default:
        fatal("Bad output format in constant_symbol::print()");
        break;
    }
}


/* Print data about a variable. Helps debugging a lot. See the stream operator
   though, that's the easiest way to get this info printed. */
void variable_symbol::print(ostream &o)
{
    symbol::print(o);
    switch (output_format) {
    case LONG_FORMAT:
        o << "  class:     variable_symbol" << endl;
        break;
    case SUMMARY_FORMAT:
    case SHORT_FORMAT:
        break;
    default:
        fatal("Bad output format in variable_symbol::print()");
        break;
    }
}



/* Print data about an array. Helps debugging a lot. See the stream operator
   though, that's the easiest way to get this info printed. */
void array_symbol::print(ostream &o)
{
    symbol::print(o);
    switch (output_format) {
    case LONG_FORMAT:
        o << "  class:     array_symbol" << endl;
        o << "  index_type: integer" << endl;
        o << "  cardinality: " << array_cardinality << endl;
        break;
    case SUMMARY_FORMAT:
        o << " [" << array_cardinality << "]";
        break;
    case SHORT_FORMAT:
        break;
    default:
        fatal("Bad output format in array_symbol::print()");
        break;
    }
}



/* Print data about a parameter. Helps debugging a lot. See the stream operator
   though, that's the easiest way to get this info printed. */
void parameter_symbol::print(ostream &o)
{
    symbol::print(o);
    switch (output_format) {
    case LONG_FORMAT:
        o << "  class:     parameter_symbol" << endl;
        o << "  size:      " << size << endl;
        if (preceding == NULL) {
            o << "  preceding: NULL" << endl;
        } else
            o << "  preceding: " << sym_tab->pool_lookup(preceding->id)
              << endl;
        break;
    case SUMMARY_FORMAT:
        o << " <-- " << sym_tab->pool_lookup(preceding->id);
        break;
    case SHORT_FORMAT:
        break;
    default:
        fatal("Bad output format in parameter_symbol::print()");
        break;
    }
}



/* Print data about a procedure. Helps debugging a lot. See the stream operator
   though, that's the easiest way to get this info printed. */
void procedure_symbol::print(ostream &o)
{
    symbol::print(o);

    switch (output_format) {
    case LONG_FORMAT:
        o << "  class:     procedure_symbol" << endl;
        o << "  ar_size:   " << ar_size << endl;
        o << "  label_nr:  " << label_nr << endl;
        o << "  params:    ";

        if (last_parameter == NULL) {
            o << "None" << endl;
        } else {
            parameter_symbol *tmp = last_parameter;
            o << short_symbols;
            while (tmp != NULL) {
                o << tmp << " ";
                tmp = tmp->preceding;
            }
            o << long_symbols << endl;
        }
        break;
    case SUMMARY_FORMAT: {
        o << "(";
        parameter_symbol *tmp = last_parameter;
        o << short_symbols;
        while (tmp != NULL) {
            o << tmp;
            tmp = tmp->preceding;
            if (tmp != NULL) {
                o << ", ";
            }
        }
        o << summary_symbols;
        o << ")";
        break;
    }
    case SHORT_FORMAT:
        break;
    default:
        fatal("Bad output format in procedure_symbol::print()");
        break;
    }
}



/* Print data about a function. Helps debugging a lot. See the stream operator
   though, that's the easiest way to get this info printed. */
void function_symbol::print(ostream &o)
{
    symbol::print(o);

    switch (output_format) {
    case LONG_FORMAT:
        o << "  class:     function_symbol" << endl;
        o << "  ar_size:   " << ar_size << endl;
        o << "  label_nr:  " << label_nr << endl;
        o << "  params:    ";

        if (last_parameter == NULL) {
            o << "None" << endl;
        } else {
            parameter_symbol *tmp = last_parameter;
            o << short_symbols;
            while (tmp != NULL) {
                o << tmp << " ";
                tmp = tmp->preceding;
            }
            o << long_symbols << endl;
        }
        break;
    case SUMMARY_FORMAT: {
        o << "(";
        parameter_symbol *tmp = last_parameter;
        o << short_symbols;
        while (tmp != NULL) {
            o << tmp;
            tmp = tmp->preceding;
            if (tmp != NULL) {
                o << ", ";
            }
        }
        o << ") returns " << sym_tab->get_symbol(type)
          << summary_symbols;
        break;
    }
    case SHORT_FORMAT:
        break;
    default:
        fatal("Bad output format in function_symbol::print()");
        break;
    }
}



/* Print data about a nametype. Helps debugging a lot. See the stream operator
   though, that's the easiest way to get this info printed. */
void nametype_symbol::print(ostream &o)
{
    symbol::print(o);
    switch (output_format) {
    case LONG_FORMAT:
        o << "  class:     nametype_symbol" << endl;
        break;
    case SUMMARY_FORMAT:
    case SHORT_FORMAT:
        break;
    default:
        fatal("Bad output format in nametype_symbol::print()");
        break;
    }
}


/* Io manipulators to control the level of detail given about symbols. */
ostream &short_symbols(ostream &o)
{
    symbol::output_format = symbol::SHORT_FORMAT;
    return o;
}


ostream &summary_symbols(ostream &o)
{
    symbol::output_format = symbol::SUMMARY_FORMAT;
    return o;
}


ostream &long_symbols(ostream &o)
{
    symbol::output_format = symbol::LONG_FORMAT;
    return o;
}
