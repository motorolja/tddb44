#ifndef __CODEGEN_HH__
#define __CODEGEN_HH__

#include <fstream>

#include "quads.hh"
#include "symtab.hh"

using namespace std;


/* These are the registers we will be using. */
enum register_type { RAX, RCX, RDX };


// Maximum number of formal parameters allowed.
const int MAX_PARAMETERS = 127;

// This is the width/size of a single address on the stack (in bytes).
const int STACK_WIDTH = 8;

/* This class generates assembler code for the Intel architecture. */
class code_generator
{
private:
    // Register array.
    string reg[3];

    // Output file stream.
    ofstream out;

    // Align a stack frame.
    int  align(int);

    // Initialize new env.
    void prologue(symbol *);

    // Leave env.
    void epilogue(symbol *);

    // Quadlist -> assembler.
    void expand(quad_list *q);

    // Get variable/parameter level & offset.
    void find(sym_index, int *, int *);

    // memory -> register.
    void fetch(sym_index, const register_type);

    // memory -> FPU.
    void fetch_float(sym_index);

    // register -> memory.
    void store(const register_type, sym_index);

    // FPU -> memory.
    void store_float(sym_index);

    // Get array base address.
    void array_address(sym_index, const register_type);

    // Get frame base address.
    void frame_address(int level, const register_type);
public:
    // Constructor. Arg = filename of assembler outfile.
    code_generator(const string);

    // Destructor.
    ~code_generator();

     // Interface.
    void generate_assembler(quad_list *, symbol *env);
};

#endif
