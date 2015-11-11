#ifndef __SCANNER_HH__
#define __SCANNER_HH__

#include "symtab.hh"

typedef union {
    long          ival;       /* Value of a T_INTNUM */
    double        rval;       /* Value of a T_REALNUM */
    pool_index    str;        /* Value of a T_STRINGCONST */
    pool_index    pool_p;     /* Value of a T_IDENT */
} YYSTYPE;

typedef struct {
    int first_line;
    int first_column;
    int last_line;
    int last_column;
} YYLTYPE;

#define	T_EOF		0	// end of file - must be 0 for bison
#define	T_ERROR		258	// invalid token
#define	T_DOT		260	// .
#define	T_SEMICOLON	261	// ;
#define	T_EQ		262	// =
#define	T_COLON		263	// :
#define	T_LEFTBRACKET	264	// [
#define	T_RIGHTBRACKET	265	// ]
#define	T_LEFTPAR	266	// (
#define	T_RIGHTPAR	267	// )

#define	T_COMMA		268	// ,
#define	T_LESSTHAN	269	// <
#define	T_GREATERTHAN	270	// >
#define	T_ADD		271	// +
#define	T_SUB		272	// -
#define	T_MUL		273	// *
#define	T_RDIV		274	// /
#define	T_OF		275	// of
#define	T_IF		276	// if
#define	T_DO		277	// do

#define	T_ASSIGN	278	// :=
#define	T_NOTEQ		279	// <>
#define	T_OR		280	// or
#define	T_VAR		281	// var
#define	T_END		282	// end
#define	T_AND		283	// and
#define	T_IDIV		284	// div
#define	T_MOD		285	// mod
#define	T_NOT		286	// not
#define	T_THEN		287	// then

#define	T_ELSE		288	// else
#define	T_IDENT		289	// foo
#define	T_CONST		290	// const
#define	T_ARRAY		291	// array
#define	T_BEGIN		292	// begin
#define	T_WHILE		293	// while
#define	T_ELSIF		294	// elsif
#define	T_INTNUM	295	// 666
#define	T_RETURN	296	// return
#define	T_PROGRAM	297	// program

#define	T_REALNUM	298	// 3.14159
#define	T_FUNCTION	299	// function
#define	T_PROCEDURE	300	// procedure
#define	T_STRINGCONST	301	// 'diesel'


#define NR_SYMS		83	// Total no. of grammar symbols

extern YYSTYPE yylval;
extern YYLTYPE yylloc;
extern symbol_table *sym_tab;

#endif
