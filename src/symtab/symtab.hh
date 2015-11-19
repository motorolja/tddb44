#ifndef __SYMTAB_HH__
#define __SYMTAB_HH__

#include "error.hh"

// Set this #define to 0 after the scanner works.
#define TEST_SCANNER 0

/* Just to make the code more readable. Not true new types like in Pascal. */
typedef long pool_index;
typedef long hash_index;
typedef long sym_index;
typedef int block_level;

/* Since constants can be either integers or reals, we use a union to hold the
   value for now. */
typedef union {
    long   ival;
    double rval;
} constant_value;

/* The various types of symbol tags that can appear. If C++ had had an
   instanceof operator like Java, we wouldn't need this... */
// enumerated 0-7
enum symbol_types {
    SYM_ARRAY,
    SYM_FUNC,
    SYM_PROC,
    SYM_VAR,
    SYM_PARAM,
    SYM_CONST,
    SYM_NAMETYPE,
    SYM_UNDEF
};
typedef enum symbol_types sym_type;

/* Some numerical constants we use in the symbol table. */

// Max allowed nesting levels.
const block_level MAX_BLOCK = 8;

// Max size of hash table.
const hash_index MAX_HASH = 512;

// Base size of string pool.
const pool_index BASE_POOL_SIZE = 1024;

// Max size of symbol table.
const sym_index MAX_SYM = 1024;

// Signifies 'no symbol'.
const sym_index NULL_SYM = -1;

// Signifies a non-int array size.
const int ILLEGAL_ARRAY_CARD = -1;

/* Sets a limit for max nr of temporary variables. Should never be reached
   unless someone really, really starts to dig writing huge programs in
   Diesel. See quads.cc. */
const int MAX_TEMP_VARS = 999999;
const int MAX_TEMP_VAR_LENGTH = 8;

/* The various symbol classes, predefined. */
class constant_symbol;
class variable_symbol;
class array_symbol;
class parameter_symbol;
class procedure_symbol;
class function_symbol;
class nametype_symbol;

class symbol_table;

// Declared 'for real' in symtab.cc.
extern symbol_table *sym_tab;



/* Global symbol table variables. These indexes point to symbols in the symbol
   table which represent information about types. Declared "for real" in
   symbol.cc. */
extern sym_index void_type;
extern sym_index integer_type;
extern sym_index real_type;




/**********************************
 *** THE VARIOUS SYMBOL CLASSES ***
 **********************************/

/* The symbol table consists of entries of subclasses to symbol. This class
   contains data that are common to all symbol types.
   This class is never used directly. Use the derived classes instead. */
class symbol
{
protected:
    // Every symbol must define a print method, which is called when the
    // symbol is sent to an outstream.
    virtual void print(ostream &);

    // This is used later on to control the level of detail given when printing
    // a symbol. If you're not used to C++, don't worry: You don't really need
    // to understand how this works to complete the lab course. :) */
    friend ostream &long_symbols(ostream &);

    friend ostream &summary_symbols(ostream &);

    friend ostream &short_symbols(ostream &);

    enum format_types { LONG_FORMAT, SUMMARY_FORMAT, SHORT_FORMAT };

    typedef enum format_types format_type;

    static format_type output_format;

public:
    // Index to the string_pool, ie, its name.
    pool_index id;

    // Describes what kind of symbol this is.
    sym_type tag;

    // Type: integer_type, real_type, or void_type.
    sym_index type;

    // Link to other symbols with same hash key.
    sym_index hash_link;

    // Link back to the hash table.
    sym_index back_link;

    // Current block level, ie, nesting depth.
    block_level level;

    // Offset, used in code generation.
    int offset;

    // Constructor.
    symbol(pool_index);

    // Currently lacks print method/operator.
    // Currently lacks some other needed stuff like conversions to and
    //   from strings.

    // These functions must be defined by the appropriate subclasses to
    // return a symbol of the correct type. This is to be able to ensure
    // safe downcasts in C++, since we don't want to rely on RTTI information
    // which isn't as of yet supported in all compilers.
    // Calling the method in this class is considered an error and will cause
    // the compiler to abort. It should only be called in a derived class.
    // The "return NULL;" lines are just there to avoid compiler warnings.
    virtual constant_symbol *get_constant_symbol() {
        fatal("Illegal downcasting to constant from symbol class");
        return NULL;
    }
    virtual variable_symbol *get_variable_symbol() {
        fatal("Illegal downcasting to variable from symbol class");
        return NULL;
    }
    virtual array_symbol *get_array_symbol() {
        fatal("Illegal downcasting to array from symbol class");
        return NULL;
    }
    virtual parameter_symbol *get_parameter_symbol() {
        fatal("Illegal downcasting to parameter from symbol class");
        return NULL;
    }
    virtual procedure_symbol *get_procedure_symbol() {
        fatal("Illegal downcasting to procedure from symbol class");
        return NULL;
    }
    virtual function_symbol *get_function_symbol() {
        fatal("Illegal downcasting to function from symbol class");
        return NULL;
    }
    virtual nametype_symbol *get_nametype_symbol() {
        fatal("Illegal downcasting to nametype from symbol class");
        return NULL;
    }

    // Allow us to print a symbol by sending it to an outstream.
    friend ostream &operator<<(ostream &, symbol *);
};


/* Derived symbol type, used for constants. */
class constant_symbol: public symbol
{
protected:
    virtual void print(ostream &);

public:
    // Value of constant, can be int or float.
    constant_value const_value;

    // Constructor. Arguments: Identifier.
    constant_symbol(const pool_index);

    // Used to be able to safely downcast from symbol to constant_symbol.
    virtual constant_symbol *get_constant_symbol() {
        return this;
    }
};


/* Derived symbol type, used for variables. */
class variable_symbol: public symbol
{
protected:
    virtual void print(ostream &);

public:
    // Constructor. Args: identifier.
    variable_symbol(const pool_index);

    // Used to be able to safely downcast from symbol to variable_symbol.
    virtual variable_symbol *get_variable_symbol() {
        return this;
    }
};


/* Derived symbol type, used for arrays. */
class array_symbol: public symbol
{
protected:
    virtual void print(ostream &);

public:
    // Points to the index type in the symbol table.
    sym_index index_type;

    // Note: cardinality = nr of elements,
    int array_cardinality;

    // Constructor. Args: identifier.
    array_symbol(const pool_index);

    // Used to be able to safely downcast from symbol to array_symbol.
    virtual array_symbol *get_array_symbol() {
        return this;
    }
};


/* Derived symbol type, used for parameters. */
class parameter_symbol: public symbol
{
protected:
    virtual void print(ostream &);

public:
    // Nr of bytes parameter needs.
    int size;

    // Link to preceding parameter, if any.
    parameter_symbol *preceding;

    // Constructor. Args: identifier.
    parameter_symbol(const pool_index);

    // Used to be able to safely downcast from symbol to parameter_symbol.
    virtual parameter_symbol *get_parameter_symbol() {
        return this;
    }
};


/* Derived symbol type, used for procedures. */
class procedure_symbol: public symbol
{
protected:
    virtual void print(ostream &);

public:
    // Activation record size.
    int ar_size;

    // Assembler label number.
    int label_nr;

    // List of parameters. We store them in reverse order to make type
    // checking easier later on.
    parameter_symbol *last_parameter;

    // Constructor. Args: identifier.
    procedure_symbol(const pool_index);

    // Used to be able to safely downcast from symbol to procedure_symbol.
    virtual procedure_symbol *get_procedure_symbol() {
        return this;
    }
};


/* Derived symbol type, used for functions. */
class function_symbol: public symbol
{
protected:
    virtual void print(ostream &);

public:
    // Activation record size.
    int ar_size;

    // Assembler label number.
    int label_nr;

    // List of parameters. We store them in reverse order to make type
    // checking easier later on.
    parameter_symbol *last_parameter;

    // Constructor. Args: identifier.
    function_symbol(const pool_index);

    // Used to be able to safely downcast from symbol to function_symbol.
    virtual function_symbol *get_function_symbol() {
        return this;
    }
};


/* Derived symbol type, used for nametypes. Since it contains no new data
   fields, we might as well use symbol directly. This subclass mainly exists
   for abstraction's sake. */
class nametype_symbol: public symbol
{
protected:
    virtual void print(ostream &);

public:

    // Constructor. Args: identifier.
    nametype_symbol(const pool_index);

    // Used to be able to safely downcast from symbol to nametype_symbol.
    virtual nametype_symbol *get_nametype_symbol() {
        return this;
    }
};


/* IO manipulators to control the level of detail output by sending a symbol
   to an ostream. NOTE: Do we really need these here, since they're already
   defined in the symbol class? - Yes, these are the ones that other classes
   have access to. */
ostream &short_symbols(ostream &);

ostream &summary_symbols(ostream &);

ostream &long_symbols(ostream &);

/* Allow a node to be sent to an outstream for printing. */
ostream &operator<<(ostream &, symbol *);




/******************************
 *** THE SYMBOL TABLE CLASS ***
 ******************************/


/* The symbol table. Presents an interfaced used by parser.y.
   The idea is that in parser.y, the code will look something like this:
   NOTE: Fix this comment.
   parameter   :   id ':' type
            {
		// Some way to check non-null, anyway.
                if (sym_tab->lookup_symbol($1->get_sym_ptr()) != NULL_SYM) {
		    error() << $1 << " already defined\n" << flush;
                }
		sym_tab->add_parameter($1->get_pos(),
		                       $1->get_id(),
				       $3->get_type());
            }
            ;
*/
class symbol_table
{
private:
    // --- String pool variables. ---

    // The actual string pool.
    char *string_pool;

    // Keep track of dynamic pool size.
    long pool_length;

    // Points to end of string pool
    long pool_pos;

    // --- Hash table variables. ---

    // The actual hash table.
    sym_index *hash_table;

    // --- Display variables. ---

    // Current nesting depth.
    block_level current_level;

    // Table of level sym_index pointers. They point at
    // the start of a new scope/block.
    sym_index *block_table;

    // --- Symbol table variables. ---

    // The actual symbol table.
    symbol **sym_table;

    // Points to last symbol entered in the table.
    sym_index sym_pos;

    // Assembler label counter.
    int label_nr;

    // Temp variable counter.
    long temp_nr;

public:
    // NOTE: Some of these methods should be made private.

    symbol_table();

    // --- Utility methods. ---

    // Convert a double to ieee 64-bit represented as a long
    long ieee(double);

    // Return type byte size.
    int get_size(const sym_index);

    // --- String pool methods. ---

    // Install a string in the pool.
    pool_index pool_install(char *);

    char *pool_lookup(const pool_index);

     // Compare strings
    bool pool_compare(const pool_index, const pool_index);

    // Remove last entry from  string pool.
    pool_index pool_forget(const pool_index);

    // Remove double '' in strings constants.
    char *fix_string(const char *);

    // Returns capitalized version  of the argument string.
    char *capitalize(const char *);

    // --- Hash table methods. ---

    // Get hash value for a string.
    hash_index hash(const pool_index);

    // --- Display methods. ---

    // Return sym_index to current procedure or function
    // (ie, start of current block).
    sym_index current_environment();

    void open_scope();

    sym_index close_scope();

    // --- Symbol table methods. ---

    sym_index lookup_symbol(const pool_index);

    symbol *get_symbol(const sym_index);

    // Installs new or returns pointer to old one.
    sym_index install_symbol(const pool_index, const sym_type tag);

    // Dump symtab content. arg 0 = detailed, arg 1 = one line per symbol.
    void  print(int);

    pool_index get_symbol_id(const sym_index);

    sym_index get_symbol_type(const sym_index);

    sym_type get_symbol_tag(const sym_index);

    // Set the type of a symbol (void_type, integer_type, or real_type).
    // Args: Index to the symbol to be changed, index to the type symbol.
    void set_symbol_type(const sym_index, const sym_index);

    // These two methods are used in quads.cc.

    // Generate next asm label.
    long get_next_label();

    // Generate, install and return sym_index to next temp var.
    sym_index gen_temp_var(sym_index);

    // These functions are used to enter identifiers into the symbol table,
    // depending on their context (function, constant, etc).

    // Args: Position information, identifier, type pointer, value. Note that
    // the value can either be an long or a double, and therefore there are two
    // versions of this method.
    sym_index enter_constant(position_information *,
                             const pool_index,
                             const sym_index,
                             const long);

    sym_index enter_constant(position_information *,
                             const pool_index,
                             const sym_index,
                             const double);

    // Args: Position information, identifier, type pointer.
    sym_index enter_variable(position_information *,
                             const pool_index,
                             const sym_index);

    // This convenience method is used for installing temporary variables for
    // which position information is not relevant. See quads.cc.
    sym_index enter_variable(const pool_index, const sym_index);

    // Args: Position information, identifier, array type pointer,
    //       index type pointer, cardinality.
    sym_index enter_array(position_information *,
                          const pool_index,
                          const sym_index,
                          const int);

    // Args: Position information, identifier.
    sym_index enter_function(position_information *, const pool_index);

    sym_index enter_procedure(position_information *, const pool_index);

    // Args: Position information, identifier, type pointer.
    sym_index enter_parameter(position_information *,
                              const pool_index,
                              const sym_index);

    // Args: Position information, identifier. NOTE: Maybe should be private?
    sym_index enter_nametype(position_information *, const pool_index);
};


#endif
