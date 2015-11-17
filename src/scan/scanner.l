%{

#include <stdlib.h>
#include <string.h>


#include "scanner.hh"
// This is where you put #include directives as needed for later labs.
// include "ast.hh", parser.hh" in that order

int column = 0;

extern YYLTYPE yylloc; // Used for position information, see below.
extern YYSTYPE yylval;
%}

%option yylineno
%option 8bit
%option noyywrap
%option caseless
%option outfile = "scanner.cc"

/* If you want to include any flex declarations, this is where to do it. */

/* Match whitespaces (maybe include carriage return \r or newline \n) */
WS [ \t]+

/* A single digit */
DIGIT [0-9]

/* Float */
REAL {DIGIT}+\.{DIGIT}+

/* Match any one standard alpha numeric ascii letter */
ALPHA [a-zA-Z]

IDENTIFIER {ALPHA}({ALPHA}|{DIGIT}|\_)+

/* Match the start of a (\") string to the ending (\"), allows empty strings */
STRING1 \"[^\"^\n]*\"
STRING2 \'[^\"^\n]*\'
STRING {STRING1}|{STRING2}
/* Match illegal characters */
FORBIDDEN [#%$]

/* Match comments, multi and single line */
COMMENT \/\*[^]*?\*\/
SINGLELINECOMMENT \/{2}[^\n]*\n

OF of
%x c_comment
/*%DIGIT [0-9]*/
/* {DIGIT}+"."{DIGIT}*
 ID [a-z][a-z0-9]* */

/* Position information is entered into the yylloc variable, so we will be
   able to generate good error messages later in later phases. */
/* Your code should be entered below the %%. Expressions to handle the
   following: Diesel comments, Diesel string constants, Diesel
   identifiers, integers, reals, and whitespace. */
%%

{WS}                       {
                           column++;
                         }  


\.                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_DOT;
                         }
;                        {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_SEMICOLON;
                         }
=                        {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_EQ;
                         }
\:                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_COLON;
                         }
\(                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_LEFTPAR;
                         }
\)                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_RIGHTPAR;
                         }
\[                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_LEFTBRACKET;
                         }
\]                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_RIGHTBRACKET;
                         }
,                        {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_COMMA;
                         }
\<                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_LESSTHAN;
                         }
\>                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_GREATERTHAN;
                         }
\+                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_ADD;
                         }
\-                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_SUB;
                         }
\*                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_MUL;
                         }
\/                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_RDIV;
                         }
":="                     {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_ASSIGN;
                         }
"<>"                     {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_NOTEQ;
                         }
of                 	 {
                            column += 2;
                            return T_OF;
                         }
if                       {
                            column += 2;
                            return T_IF;
                         }
do                       {
                            column += 2;
                            return T_DO;
                         }
or                       {
                            column += 2;
                            return T_OR;
                         }
var                      {
                            column += 3;
                            return T_VAR;
                         }
end                      {
                            column += 3;
                            return T_END;
                         }
and                      {
                            column += 3;
                            return T_AND;
                         }
div                      {
                            column += 3;
                            return T_IDIV;
                         }
mod                      {
                            column += 3;
                            return T_MOD;
                         }
not                      {
                            column += 3;
                            return T_NOT;
                         }
then                     {
                            column += 4;
                            return T_THEN;
                         }
else                     {
                            column += 4;
                            return T_ELSE;
                         }
const                    {
                            column += 5;
                            return T_CONST;
                         }
array                    {
                            column += 5;
                            return T_ARRAY;
                         }
begin                    {
                            column += 5;
                            return T_BEGIN;
                         }
while                    {
                            column += 5;
                            return T_WHILE;
                         }
elsif                    {
                            column += 5;
                            return T_ELSIF;
                         }
return                   {
                            column += 6;
                            return T_RETURN;
                         }
program                  {
                            column += 7;
                            return T_PROGRAM;  
                         }
function                 {
                            column += 8;
                            return T_FUNCTION;
                         }
procedure                {
                            column += 9;
                            return T_PROCEDURE;
                         }

{INTEGER}		{
				
                           yylloc.first_line = yylineno;
                           yylloc.last_line = yylineno;
                           yylloc.first_column = column;
                           yylloc.last_column = column + yyleng;
                           yylval.pool_p = sym_tab->pool_install(yytext);//assign mattched string
                           column += yyleng;
			   return T_INTNUM;
			}
{IDENTIFIER}               {
                           yylloc.first_line = yylineno;
                           yylloc.last_line = yylineno;
                           yylloc.first_column = column;
                           yylloc.last_column = column + yyleng;
                           yylval.pool_p = sym_tab->pool_install(yytext);//assign mattched string
                           column += yyleng;
                           return T_IDENT;
                         }
{REAL}                   {
			   
                           yylloc.first_line = yylineno;
                           yylloc.last_line = yylineno;
                           yylloc.first_column = column;
                           yylloc.last_column = column + yyleng;
                           yylval.pool_p = sym_tab->pool_install(yytext);//assign mattched string
                           column += yyleng;
                           return T_REALNUM;
                         }
{SINGLELINECOMMENT}        column = 0; /* Skip single-line comment */
"/\*"                    {
                           column += yyleng;
                           BEGIN(c_comment);
                         }

<c_comment>
{
    "\*/"                {
                            column += 2;
                            BEGIN(INITIAL);
                         }
    "/\*"                {
                            column += 2;
                            yyerror("Suspicious comment");
                         }
    [^\n^\r]                column++; /* Skip stuff in comments */
    [\n\r]                   column = 0;
    <<EOF>>              {
                            yyerror("Unterminated comment");
                            yyterminate();
                         }
}

 

<<EOF>>                  yyterminate();
.                        yyerror("Illegal character");
