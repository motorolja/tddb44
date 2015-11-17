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
WS [ \t]
DIGIT [0-9]
EXP [Ee](\+|-)?{DIGIT}+
REAL  ({DIGIT}+{EXP}|({DIGIT}*\.{DIGIT}+|{DIGIT}+\.{DIGIT}*){EXP}?)
INTEGER {DIGIT}+
STRING '([^\n^\r^']|'')+'

NEWLINE [\n\r]+
SINGLELINECOMMENT \/{2}[^\n]*
START_MULTILINECOMMENT \{
END_MULTILINECOMMENT \}
NO_NEWLINE [^\n^\r]

ALPHA [a-zA-Z]
IDENTIFIER {ALPHA}({ALPHA}|{DIGIT}|\_)*

FORBIDDEN [#%$]
%x c_comment
%x string
%%

{INTEGER}		{
                           yylloc.first_line = yylineno;
                           yylloc.last_line = yylineno;
                           yylloc.first_column = column;
                           yylloc.last_column = column + yyleng;
                           //printf("|%s|", yytext);
                           yylval.ival = atol(yytext);
                           column += yyleng;
			               return T_INTNUM;
                        }

{REAL}                  {
                           yylloc.first_line = yylineno;
                           yylloc.last_line = yylineno;
                           yylloc.first_column = column;
                           yylloc.last_column = column + yyleng;
                           //printf("|%s|", yytext);
                           yylval.rval = atof(yytext);
                           column += yyleng;
            			   return T_REALNUM;
                          
                        }
{STRING}                {
                           yylloc.first_line = yylineno;
                           yylloc.last_line = yylineno;
                           yylloc.first_column = column;
                           yylloc.last_column = column + yyleng;
                           //printf("|%s|", yytext);
                           // TODO cut first and last character of yytext and 
                           // replace '' with '
                           yylval.str = sym_tab->pool_install(yytext);
                           column += yyleng;
            			   return T_STRINGCONST;
                        }

\.                      {
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
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += 2;
                            return T_OF;
                         }
if                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += 2;
                            return T_IF;
                         }
do                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += 2;
                            return T_DO;
                         }
or                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += 2;
                            return T_OR;
                         }
var                      {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += 3;
                            return T_VAR;
                         }
end                      {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += 3;
                            return T_END;
                         }
and                      {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += 3;
                            return T_AND;
                         }
div                      {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += 3;
                            return T_IDIV;
                         }
mod                      {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += 3;
                            return T_MOD;
                         }
not                      {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += 3;
                            return T_NOT;
                         }
then                     {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += 4;
                            return T_THEN;
                         }
else                     {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += 4;
                            return T_ELSE;
                         }
const                    {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += 5;
                            return T_CONST;
                         }
array                    {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += 5;
                            return T_ARRAY;
                         }
begin                    {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += 5;
                            return T_BEGIN;
                         }
while                    {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += 5;
                            return T_WHILE;
                         }
elsif                    {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += 5;
                            return T_ELSIF;
                         }
return                   {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += 6;
                            return T_RETURN;
                         }
program                  {
                           yylloc.first_line = yylineno;
                           yylloc.first_column = column;
                            column += 7;
                            return T_PROGRAM;  
                         }
function                 {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += 8;
                            return T_FUNCTION;
                         }
procedure                {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += 9;
                            return T_PROCEDURE;
                         }
{IDENTIFIER}            {
                           yylloc.first_line = yylineno;
                           yylloc.last_line = yylineno;
                           yylloc.first_column = column;
                           yylloc.last_column = column + yyleng;
                           yylval.pool_p = sym_tab->pool_install(yytext);//assign mattched string
                           column += yyleng;
                           return T_IDENT;
                         }
{WS}                     {
                           //printf("WHit\n");
                           column++;
                         }
{NEWLINE}|{SINGLELINECOMMENT}    column = 0;

{START_MULTILINECOMMENT} {
                            column += yyleng;
                            BEGIN(c_comment);
                         }

<c_comment>              
{
    {END_MULTILINECOMMENT} {
                            column += 2;
                            BEGIN(INITIAL);
                         }
    {START_MULTILINECOMMENT} {
                            column += 2;
                            yyerror("Suspicious comment");
                         }
    {NO_NEWLINE}                column++; /* Skip stuff in comments */
    {NEWLINE}                   column = 0;
    <<EOF>>              {
                            yyerror("Unterminated comment");
                            yyterminate();
                         }
}
{FORBIDDEN}              {
                           yyerror("Forbidden character");
                         }
<<EOF>>                  yyterminate();
.                        {
                           //printf("|%s|",yytext);
                           yyerror("Couldnt find matching token");
                          }

