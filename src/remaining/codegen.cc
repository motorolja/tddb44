#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <string.h>

#include "symtab.hh"
#include "quads.hh"
#include "codegen.hh"

using namespace std;

// Defined in main.cc.
extern bool assembler_trace;

// Used in parser.y. Ideally the filename should be parametrized, but it's not
// _that_ important...
code_generator *code_gen = new code_generator("d.out");

// Constructor.
code_generator::code_generator(const string object_file_name)
{
    out.open(object_file_name);

    reg[RAX] = "rax";
    reg[RCX] = "rcx";
    reg[RDX] = "rdx";
}


/* Destructor. */
code_generator::~code_generator()
{
    // Make sure we close the outfile before exiting the compiler.
    out << flush;
    out.close();
}



/* This method is called from parser.y when code generation is to start.
   The argument is a quad_list representing the body of the procedure, and
   the symbol for the environment for which code is being generated. */
void code_generator::generate_assembler(quad_list *q, symbol *env)
{
    prologue(env);
    expand(q);
    epilogue(env);
}



/* This method aligns a frame size on an 8-byte boundary. Used by prologue().
 */
int code_generator::align(int frame_size)
{
    return ((frame_size + 7) / 8) * 8;
}



/* This method generates assembler code for initialisating a procedure or
   function. */
void code_generator::prologue(symbol *new_env)
{
    int ar_size;
    int label_nr;
    block_level curr_level;
    // Used to count parameters.
    parameter_symbol *last_arg;

    // Again, we need a safe downcast for a procedure/function.
    // Note that since we have already generated quads for the entire block
    // before we expand it to assembler, the size of the activation record
    // is known here (ar_size).
    if (new_env->tag == SYM_PROC) {
        procedure_symbol *proc = new_env->get_procedure_symbol();
        ar_size = align(proc->ar_size);
        label_nr = proc->label_nr;
        last_arg = proc->last_parameter;
        curr_level = proc->level;
    } else if (new_env->tag == SYM_FUNC) {
        function_symbol *func = new_env->get_function_symbol();
        /* Make sure ar_size is a multiple of eight */
        ar_size = align(func->ar_size);
        label_nr = func->label_nr;
        last_arg = func->last_parameter;
        curr_level = func->level;
    } else {
        fatal("code_generator::prologue() called for non-proc/func");
        return;
    }

    /* Print out the label number (a SYM_PROC/ SYM_FUNC attribute) */
    out << "L" << label_nr << ":" << "\t\t\t" << "# " <<
        /* Print out the function/procedure name */
        sym_tab->pool_lookup(new_env->id) << endl;

    if (assembler_trace) {
        out << "\t\t" << "# PROLOGUE (" << short_symbols << new_env
            << long_symbols << ")" << endl;
    }

    //generate code to to create activation record when calling a function
    out << "\t\t" << "push" << "\t" << "rbp" << endl;
    out << "\t\t" << "mov" << "\t" << "rcx,rsp" << endl;
    // allocate space for display
    for (int i = 1; i<= curr_level; i++) {
        // push the 8 the value of the previous block
        out << "\t\t" << "push" << "\t" << "[rbp-"<< STACK_WIDTH*i <<"]" << endl;
    }
    // push the previous RSP on the stack
    out << "\t\t" << "push" << "\t" << "rcx" << endl;
    
    // and realy make it our new RBP
    out << "\t\t" << "mov" << "\t" << "rbp,rcx" << endl;
    // allocate space for temporary storage    
    out << "\t\t" <<"sub" << "\t" << "rsp," << ar_size << endl;
    out << flush;
}



/* This method generates assembler code for leaving a procedure or function. */
void code_generator::epilogue(symbol *old_env)
{
    if (assembler_trace) {
        out << "\t" << "# EPILOGUE (" << short_symbols << old_env
            << long_symbols << ")" << endl;
    }
    // release activation record
    out << "\t\t" <<"leave" << endl;
    // return
    out << "\t\t" <<"ret" << endl;
    out << flush;
}



/* This function finds the display register level and offset for a variable,
   array or a parameter. Note the pass-by-pointer arguments. */
void code_generator::find(sym_index sym_p, int *level, int *offset)
{
    symbol *sym = sym_tab->get_symbol(sym_p);
    //the question is from where do we count the offset
    if(sym->tag == SYM_PARAM){
        // TODO understand this
        parameter_symbol *par_sym = sym->get_parameter_symbol();
        
        *offset = STACK_WIDTH + par_sym->offset + par_sym->size;
    }else
    if(sym->tag == SYM_VAR || sym->tag == SYM_ARRAY){
        // display area (number of levels + the basepointer)
        int display_area = STACK_WIDTH * (sym->level + 1);
        // just take the offset from symbol
        int offset_area = sym->offset;
        // don't get why its negativ
        *offset = - (display_area + offset_area);
    }else
        fatal("couldnt find strange symbol");
    *level = sym->level;

}

/*
 * Generates code for getting the address of a frame for the specified scope level.
 */
void code_generator::frame_address(int level, const register_type dest)
{
    // store base address of the corresponding address
    out << "\t\t" << "mov" << "\t"
    << reg[dest] << ",[rbp-"<< STACK_WIDTH * level <<"]"<<endl;
}

/* This function fetches the value of a variable or a constant into a
   register. */
void code_generator::fetch(sym_index sym_p, register_type dest)
{
    symbol *sym = sym_tab->get_symbol(sym_p);
    int level, offset;
    find(sym_p,&level, &offset);

    if(sym->tag == SYM_PARAM){
        out << "\t\t" <<"mov" << "\t"<< reg[dest]
        // has positiv offset
        <<",[rbp+" << offset <<"]" << endl;
    }else
    if(sym->tag == SYM_VAR || sym->tag == SYM_ARRAY){
        // # mov rcx,[rbp-level]
        frame_address(level, RCX);
        // # mov rax,[rcx offset]  
        out << "\t\t" << "mov" << "\t"<< reg[dest]  <<",["<< reg[RCX] << offset << "]" <<endl;
    }else
    if(sym->tag == SYM_CONST){
        fatal("after optimsation there shouldn't exit any constants");
    }
}

// take the variable  and push  to the fpu stack 
void code_generator::fetch_float(sym_index sym_p)
{
  // fetch the symbol and do some type checking
  symbol* sym = sym_tab->get_symbol(sym_p);
    int level,offset;
    find(sym_p,&level,&offset);

  if (sym->type == real_type && sym->tag == SYM_VAR) {
    frame_address(level,RCX);
    out << "\t\t" << "fld" << "\t" << "qword ptr [" << reg[RCX] << offset << "]" << endl;
  }else
  if(sym->type == real_type && sym->tag == SYM_PARAM)
  {
    frame_address(level,RCX);
    out << "\t\t" << "fld" << "\t" << "qword ptr [" << reg[RCX] << "+" << offset << "]" << endl;
  }else
    fatal("In fetch_float(): Invalid symbol tag or type, tag needs to be SYM_VAR and type real_type.");

}



/* This function stores the value of a register into a variable. */
void code_generator::store(register_type src, sym_index sym_p)
{
    int level, offset;
    // here we have to use the level
    find(sym_p,&level, &offset);
    symbol *sym = sym_tab->get_symbol(sym_p);
    if(sym->tag == SYM_PARAM){
        out << "\t\t" << "mov" << "\t"
        <<"[rbp+" << offset <<"],"
        << reg[src] << endl;
    }else
    if(sym->tag == SYM_VAR){

        // # mov rcx,[rbp-level]
        frame_address(level, RCX);       
        // output right register

        out <<"\t\t" << "mov" <<"\t"<< "[rcx" << offset << "],"
        << reg[src] << endl;
    }
}

void code_generator::store_float(sym_index sym_p)
{
  // fetch the symbol and do some type checking
  symbol* sym = sym_tab->get_symbol(sym_p);
  if (sym->type == real_type && sym->tag == SYM_VAR) {
    int level,offset;
    find(sym_p,&level,&offset);
    frame_address(level,RCX);
    out << "\t\t" << "fstp" << "\t" << "qword ptr [" << reg[RCX] << offset << "]" << endl;
  }
  else
    {
     fatal("In store_float(): Invalid symbol tag or type, tag needs to be SYM_VAR and type real_type.");
   }
}


/* This function fetches the base address of an array. */
void code_generator::array_address(sym_index sym_p, register_type dest)
{
    // we know that is a variable
    int level, offset;
    find(sym_p,&level, &offset);
    frame_address(level, RCX);
    out << "\t\t" << "sub" << "\t" << reg[RCX] << "," << -offset << endl;
    out << "\t\t" << "mov" << "\t" << reg[dest] << "," << reg[RCX] << endl;
}

/* This method expands a quad_list into assembler code, quad for quad. */
void code_generator::expand(quad_list *q_list)
{
    long quad_nr = 0;       // Just to make debug output easier to read.
    

    // We use this iterator to loop through the quad list.
    quad_list_iterator *ql_iterator = new quad_list_iterator(q_list);

    quadruple *q = ql_iterator->get_current(); // This is the head of the list.

    while (q != NULL) {
        quad_nr++;

        // We always do labels here so that a branch doesn't miss the
        // trace code.
        if (q->op_code == q_labl) {
            out << "L" << q->int1 << ":" << endl;
        }

        // Debug output.
        if (assembler_trace) {
            out << "\t" << "# QUAD " << quad_nr << ": (" 
                << short_symbols << q << long_symbols << endl;
        }

        // The main switch on quad type. This is where code is actually
        // generated.
        switch (q->op_code) {
        case q_rload:
        case q_iload:
            out << "\t\t" << "mov" << "\t" << "rax, " << q->int1 << endl;
            store(RAX, q->sym3);
            break;

        case q_inot: {
            int label = sym_tab->get_next_label();
            int label2 = sym_tab->get_next_label();

            fetch(q->sym1, RAX);
            out << "\t\t" << "cmp" << "\t" << "rax, 0" << endl;
            out << "\t\t" << "je" << "\t" << "L" << label << endl;
            // Not equal branch
            out << "\t\t" << "mov" << "\t" << "rax, 0" << endl;
            out << "\t\t" << "jmp" << "\t" << "L" << label2 << endl;
            // Equal branch
            out << "\t\t" << "L" << label << ":" << endl;
            out << "\t\t" << "mov" << "\t" << "rax, 1" << endl;

            out << "\t\t" << "L" << label2 << ":" <<  endl;
            store(RAX, q->sym3);
            break;
        }
        case q_ruminus:
            fetch_float(q->sym1);
            out << "\t\t" << "fchs" << endl;
            store_float(q->sym3);
            break;

        case q_iuminus:
            fetch(q->sym1, RAX);
            out << "\t\t" << "neg" << "\t" << "rax" << endl;
            store(RAX, q->sym3);
            break;

        case q_rplus:
            fetch_float(q->sym1);
            fetch_float(q->sym2);
            out << "\t\t" << "faddp" << endl;
            store_float(q->sym3);
            break;

        case q_iplus:
            fetch(q->sym1, RAX);
            fetch(q->sym2, RCX);
            out << "\t\t" << "add" << "\t" << "rax, rcx" << endl;
            store(RAX, q->sym3);
            break;

        case q_rminus:
            fetch_float(q->sym1);
            fetch_float(q->sym2);
            out << "\t\t" << "fsubp" << endl;
            store_float(q->sym3);
            break;

        case q_iminus:
            fetch(q->sym1, RAX);
            fetch(q->sym2, RCX);
            out << "\t\t" << "sub" << "\t" << "rax, rcx" << endl;
            store(RAX, q->sym3);
            break;

        case q_ior: {
            int label = sym_tab->get_next_label();
            int label2 = sym_tab->get_next_label();

            fetch(q->sym1, RAX);
            out << "\t\t" << "cmp" << "\t" << "rax, 0" << endl;
            out << "\t\t" << "jne" << "\t" << "L" << label << endl;
            fetch(q->sym2, RAX);
            out << "\t\t" << "cmp" << "\t" << "rax, 0" << endl;
            out << "\t\t" << "jne" << "\t" << "L" << label << endl;
            // False branch
            out << "\t\t" << "mov" << "\t" << "rax, 0" << endl;
            out << "\t\t" << "jmp" << "\t" << "L" << label2 << endl;
            // True branch
            out << "\t\t" << "L" << label << ":" << endl;
            out << "\t\t" << "mov" << "\t" << "rax, 1" << endl;

            out << "\t\t" << "L" << label2 << ":" << endl;
            store(RAX, q->sym3);
            break;
        }
        case q_iand: {
            int label = sym_tab->get_next_label();
            int label2 = sym_tab->get_next_label();

            fetch(q->sym1, RAX);
            out << "\t\t" << "cmp" << "\t" << "rax, 0" << endl;
            out << "\t\t" << "je" << "\t" << "L" << label << endl;
            fetch(q->sym2, RAX);
            out << "\t\t" << "cmp" << "\t" << "rax, 0" << endl;
            out << "\t\t" << "je" << "\t" << "L" << label << endl;
            // True branch
            out << "\t\t" << "mov" << "\t" << "rax, 1" << endl;
            out << "\t\t" << "jmp" << "\t" << "L" << label2 << endl;
            // False branch
            out << "\t\t" << "L" << label << ":" << endl;
            out << "\t\t" << "mov" << "\t" << "rax, 0" << endl;

            out << "\t\t" << "L" << label2 << ":" << endl;
            store(RAX, q->sym3);
            break;
        }
        case q_rmult:
            fetch_float(q->sym1);
            fetch_float(q->sym2);
            out << "\t\t" << "fmulp" << endl;
            store_float(q->sym3);
            break;

        case q_imult:
            fetch(q->sym1, RAX);
            fetch(q->sym2, RCX);
            out << "\t\t" << "imul" << "\t" << "rax, rcx" << endl;
            store(RAX, q->sym3);
            break;

        case q_rdivide:
            fetch_float(q->sym1);
            fetch_float(q->sym2);
            out << "\t\t" << "fdivp" << endl;
            store_float(q->sym3);
            break;

        case q_idivide:
            fetch(q->sym1, RAX);
            fetch(q->sym2, RCX);
            out << "\t\t" << "cqo" << endl;
            out << "\t\t" << "idiv" << "\t" << "rax, rcx" << endl;
            store(RAX, q->sym3);
            break;

        case q_imod:
            fetch(q->sym1, RAX);
            fetch(q->sym2, RCX);
            out << "\t\t" << "cqo" << endl;
            out << "\t\t" << "idiv" << "\t" << "rax, rcx" << endl;
            store(RDX, q->sym3);
            break;

        case q_req: {
            int label = sym_tab->get_next_label();
            int label2 = sym_tab->get_next_label();

            fetch_float(q->sym1);
            fetch_float(q->sym2);
            out << "\t\t" << "fcomip" << "\t" << "ST(0), ST(1)" << endl;
            // Clear the stack
            out << "\t\t" << "fstp" << "\t" << "ST(0)" << endl;
            out << "\t\t" << "je" << "\t" << "L" << label << endl;
            // False branch
            out << "\t\t" << "mov" << "\t" << "rax, 0" << endl;
            out << "\t\t" << "jmp" << "\t" << "L" << label2 << endl;
            // True branch
            out << "\t\t" << "L" << label << ":" << endl;
            out << "\t\t" << "mov" << "\t" << "rax, 1" << endl;

            out << "\t\t" << "L" << label2 << ":" << endl;
            store(RAX, q->sym3);
            break;
        }
        case q_ieq: {
            int label = sym_tab->get_next_label();
            int label2 = sym_tab->get_next_label();

            fetch(q->sym1, RAX);
            fetch(q->sym2, RCX);
            out << "\t\t" << "cmp" << "\t" << "rax, rcx" << endl;
            out << "\t\t" << "je" << "\t" << "L" << label << endl;
            // False branch
            out << "\t\t" << "mov" << "\t" << "rax, 0" << endl;
            out << "\t\t" << "jmp" << "\t" << "L" << label2 << endl;
            // True branch
            out << "\t\t" << "L" << label << ":" << endl;
            out << "\t\t" << "mov" << "\t" << "rax, 1" << endl;

            out << "\t\t" << "L" << label2 << ":" << endl;
            store(RAX, q->sym3);
            break;
        }
        case q_rne: {
            int label = sym_tab->get_next_label();
            int label2 = sym_tab->get_next_label();

            fetch_float(q->sym1);
            fetch_float(q->sym2);
            out << "\t\t" << "fcomip" << "\t" << "ST(0), ST(1)" << endl;
            // Clear the stack
            out << "\t\t" << "fstp" << "\t" << "ST(0)" << endl;
            out << "\t\t" << "jne" << "\t" << "L" << label << endl;
            // False branch
            out << "\t\t" << "mov" << "\t" << "rax, 0" << endl;
            out << "\t\t" << "jmp" << "\t" << "L" << label2 << endl;
            // True branch
            out << "\t\t" << "L" << label << ":" << endl;
            out << "\t\t" << "mov" << "\t" << "rax, 1" << endl;

            out << "\t\t" << "L" << label2 << ":" << endl;
            store(RAX, q->sym3);
            break;
        }
        case q_ine: {
            int label = sym_tab->get_next_label();
            int label2 = sym_tab->get_next_label();

            fetch(q->sym1, RAX);
            fetch(q->sym2, RCX);
            out << "\t\t" << "cmp" << "\t" << "rax, rcx" << endl;
            out << "\t\t" << "jne" << "\t" << "L" << label << endl;
            // False branch
            out << "\t\t" << "mov" << "\t" << "rax, 0" << endl;
            out << "\t\t" << "jmp" << "\t" << "L" << label2 << endl;
            // True branch
            out << "\t\t" << "L" << label << ":" << endl;
            out << "\t\t" << "mov" << "\t" << "rax, 1" << endl;

            out << "\t\t" << "L" << label2 << ":" << endl;
            store(RAX, q->sym3);
            break;
        }
        case q_rlt: {
            int label = sym_tab->get_next_label();
            int label2 = sym_tab->get_next_label();

            // We need to push in reverse order for this to work
            fetch_float(q->sym2);
            fetch_float(q->sym1);
            out << "\t\t" << "fcomip" << "\t" << "ST(0), ST(1)" << endl;
            // Clear the stack
            out << "\t\t" << "fstp" << "\t" << "ST(0)" << endl;
            out << "\t\t" << "jb" << "\t" << "L" << label << endl;
            // False branch
            out << "\t\t" << "mov" << "\t" << "rax, 0" << endl;
            out << "\t\t" << "jmp" << "\t" << "L" << label2 << endl;
            // True branch
            out << "\t\t" << "L" << label << ":" << endl;
            out << "\t\t" << "mov" << "\t" << "rax, 1" << endl;

            out << "\t\t" << "L" << label2 << ":" << endl;
            store(RAX, q->sym3);
            break;
        }
        case q_ilt: {
            int label = sym_tab->get_next_label();
            int label2 = sym_tab->get_next_label();

            fetch(q->sym1, RAX);
            fetch(q->sym2, RCX);
            out << "\t\t" << "cmp" << "\t" << "rax, rcx" << endl;
            out << "\t\t" << "jl" << "\t" << "L" << label << endl;
            // False branch
            out << "\t\t" << "mov" << "\t" << "rax, 0" << endl;
            out << "\t\t" << "jmp" << "\t" << "L" << label2 << endl;
            // True branch
            out << "\t\t" << "L" << label << ":" << endl;
            out << "\t\t" << "mov" << "\t" << "rax, 1" << endl;

            out << "\t\t" << "L" << label2 << ":" << endl;
            store(RAX, q->sym3);
            break;
        }
        case q_rgt: {
            int label = sym_tab->get_next_label();
            int label2 = sym_tab->get_next_label();

            // We need to push in reverse order for this to work
            fetch_float(q->sym2);
            fetch_float(q->sym1);
            out << "\t\t" << "fcomip" << "\t" << "ST(0), ST(1)" << endl;
            // Clear the stack
            out << "\t\t" << "fstp" << "\t" << "ST(0)" << endl;
            out << "\t\t" << "ja" << "\t" << "L" << label << endl;
            // False branch
            out << "\t\t" << "mov" << "\t" << "rax, 0" << endl;
            out << "\t\t" << "jmp" << "\t" << "L" << label2 << endl;
            // True branch
            out << "\t\t" << "L" << label << ":" << endl;
            out << "\t\t" << "mov" << "\t" << "rax, 1" << endl;

            out << "\t\t" << "L" << label2 << ":" << endl;
            store(RAX, q->sym3);
            break;
        }
        case q_igt: {
            int label = sym_tab->get_next_label();
            int label2 = sym_tab->get_next_label();

            fetch(q->sym1, RAX);
            fetch(q->sym2, RCX);
            out << "\t\t" << "cmp" << "\t" << "rax, rcx" << endl;
            out << "\t\t" << "jg" << "\t" << "L" << label << endl;
            // False branch
            out << "\t\t" << "mov" << "\t" << "rax, 0" << endl;
            out << "\t\t" << "jmp" << "\t" << "L" << label2 << endl;
            // True branch
            out << "\t\t" << "L" << label << ":" << endl;
            out << "\t\t" << "mov" << "\t" << "rax, 1" << endl;

            out << "\t\t" << "L" << label2 << ":" << endl;
            store(RAX, q->sym3);
            break;
        }
        case q_rstore:
        case q_istore:
            fetch(q->sym1, RAX);
            fetch(q->sym3, RCX);
            out << "\t\t" << "mov" << "\t" << "[rcx], rax" << endl;
            break;

        case q_rassign:
        case q_iassign:
            fetch(q->sym1, RAX);
            store(RAX, q->sym3);
            break;

        case q_param:
            // calculate the expr and to rax
            fetch(q->sym1, RAX);
            // push rax to the stack
            out << "\t\t" << "push" << "\t" << "rax" << endl;
            break;

        case q_call: {
            symbol *sym = sym_tab->get_symbol(q->sym1);
            if(sym->tag == SYM_FUNC){
                function_symbol *func_sym = sym->get_function_symbol(); 
                out <<"\t\t" << "call"<< "\t" << "L" << func_sym->label_nr<< endl;
                // save the 
                store(RAX, q->sym3);
            }else
            if(sym->tag == SYM_PROC){
                procedure_symbol *proc_sym = sym->get_procedure_symbol(); 
                out <<"\t\t" << "call"<< "\t" << "L" << proc_sym->label_nr<< endl;
            }else
                fatal("called not a function");

            // delete the parameter
            out << "\t\t" << "add" << "\t" << "rsp," << STACK_WIDTH * q->sym2 << endl;
            break;
        }
        case q_rreturn:
        case q_ireturn:
            fetch(q->sym2, RAX);
            out << "\t\t" << "jmp" << "\t" << "L" << q->int1 << endl;
            break;

        case q_lindex:
            array_address(q->sym1, RAX);
            fetch(q->sym2, RCX);
            out << "\t\t" << "imul" << "\t" << "rcx, " << STACK_WIDTH << endl;
            out << "\t\t" << "sub" << "\t" << "rax, rcx" << endl;
            store(RAX, q->sym3);
            break;

        case q_rrindex:
        case q_irindex:
            array_address(q->sym1, RAX);
            fetch(q->sym2, RCX);
            out << "\t\t" << "imul" << "\t" << "rcx, " << STACK_WIDTH << endl;
            out << "\t\t" << "sub" << "\t" << "rax, rcx" << endl;
            out << "\t\t" << "mov" << "\t" << "rax, [rax]" << endl;
            store(RAX, q->sym3);
            break;

        case q_itor: {
            block_level level;      // Current scope level.
            int offset;             // Offset within current activation record.

            find(q->sym1, &level, &offset);
            frame_address(level, RCX);
            out << "\t\t" << "fild" << "\t" << "qword ptr [rcx";
            if (offset >= 0) {
                out << "+" << offset;
            } else {
                out << offset; // Implicit "-"
            }
            out << "]" << endl;
            store_float(q->sym3);
        }
        break;

        case q_jmp:
            out << "\t\t" << "jmp" << "\t" << "L" << q->int1 << endl;
            break;

        case q_jmpf:
            fetch(q->sym2, RAX);
            out << "\t\t" << "cmp" << "\t" << "rax, 0" << endl;
            out << "\t\t" << "je" << "\t" << "L" << q->int1 << endl;
            break;

        case q_labl:
            // We handled this one above already.
            break;

        case q_nop:
            // q_nop quads should never be generated.
            fatal("code_generator::expand(): q_nop quadruple produced.");
            return;
        }

        // Get the next quad from the list.
        q = ql_iterator->get_next();
    }

    // Flush the generated code to file.
    out << flush;
}
