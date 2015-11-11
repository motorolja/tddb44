# diesel_glue.s
# Assembly code for some DIESEL run-time support.
# (this should be included by d.out)
# Handwritten file.

# This file is partially linking to some GCC-compiled functions.
# That means the argument has to be passed in with rdi.

.intel_syntax noprefix

.align    8
.global   main

main: # this is where the process starts

    # As we only use truncation and no other rounding
    # we just set the rounding mode to truncate here.
    # We need to keep the previous control word so we have
    # to or in the new data we want. Note that the constant
    # sets the two bits that we want so or is enough.
    enter 8, 0
    fnstcw word ptr [rbp-8]
    or word ptr [rbp-8], 3072 # from FE_TOWARDZERO
    fldcw word ptr [rbp-8]
    leave

    enter 0, 0
    call    L3        # L3 is the DIESEL main program label
    leave
    # Exit in a normal way.
    mov rax, 0
    ret

L0: # read function
    # Return value is in RAX
    call    getchar
    ret

L1: # write procedure
    mov rdi, qword ptr [rsp+8]
    call    myputchar    # in diesel_rts.o
    ret

L2: # trunc function
    # This very cryptic instruction
    # ConVerTs with Truncation a Signed Double TO a Signed Integer
    # cvttsd2si rax, [rsp+8]
    enter 8, 0
    fld qword ptr [rbp+16]
    fistp qword ptr [rbp-8]
    mov rax, qword ptr [rbp-8]
    leave
    ret
