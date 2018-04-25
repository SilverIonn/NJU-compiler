%{
	#include "node.h"
	#include "syntax.tab.h"
	int errorCount = 0;
%}

%option yylineno


ID [a-zA-Z_][0-9a-zA-Z_]*

COMMENT_1 "//"[^\n]*
COMMENT_2 "/*"(([^\*]*(\*[^\/])?)*)"*/"
COMMENT_ERR "/*"(([^\*]*(\*[^\/])?)*)({COMMENT_2}(([^\*]*(\*[^\/])?)*))+"*/"



ws [ \t]+

INT10 ([1-9][0-9]*)|0
INT8  0[0-7]*
INT8_ERR 0[0-9]*
INT16 ("0x"|"0X")[0-9a-fA-F]*
INT16_ERR ("0x"|"0X")[0-9a-zA-Z]*
INT {INT10}|{INT8}|{INT16}

FLOAT_1 {INT10}"."[0-9]+
FLOAT_2 [0-9]*"."[0-9]*[Ee][+-]?[0-9]+
FLOAT_1_ERR ({INT10}".")|("."{INT10})
FLOAT_2_ERR [0-9]*"."[0-9]*[Ee][+-]?  

SEMI ";"
COMMA ","
ASSIGNOP "="
RELOP ">"|"<"|">="|"<="|"=="|"!="
PLUS "+"
MINUS "-"
STAR "*"
DIV "/"
AND "&&"
OR "||"
DOT "."
NOT "!"
TYPE "int"|"float"
LP "("
RP ")"
LB "["
RB "]"
LC "{"
RC "}"
STRUCT "struct"
RETURN "return"
IF "if"
ELSE "else"
WHILE "while"


%%
{COMMENT_ERR} 	{errorCount++; printf("Error type A at line %d: Illegal comments.\n",yylineno);}
{COMMENT_1}    	{;}
{COMMENT_2} 	{;}
{ws}		{;}
\n		{;}
{FLOAT_2}       {yylval.node=initNode("FLOAT",yytext);return FLOAT;}
{FLOAT_2_ERR}   {errorCount++; printf("Error type A at line %d: Illegal floating point number '%s'\n",yylineno,yytext);return FLOAT;}
{FLOAT_1}       {yylval.node=initNode("FLOAT",yytext);return FLOAT;}
{FLOAT_1_ERR}   {errorCount++; printf("Error type A at line %d: Illegal floating point number '%s'\n",yylineno,yytext);return FLOAT;}
{INT} 		{yylval.node=initNode("INT",yytext);return INT;}
{INT8_ERR}	{errorCount++; printf("Error type A at line %d: Illegal octal number '%s'\n",yylineno,yytext);return INT;}
{INT16_ERR}      {errorCount++; printf("Error type A at line %d: Illegal hexadecimal number '%s'\n",yylineno,yytext);return INT;}
{SEMI} 		{yylval.node=initNode("SEMI",yytext);return SEMI;}
{COMMA} 	{yylval.node=initNode("COMMA",yytext);return COMMA;}
{ASSIGNOP} 	{yylval.node=initNode("ASSIGNOP",yytext);return ASSIGNOP;}
{RELOP}	 	{yylval.node=initNode("RELOP",yytext);return RELOP;}
{PLUS} 		{yylval.node=initNode("PLUS",yytext);return PLUS;}
{MINUS} 	{yylval.node=initNode("MINUS",yytext);return MINUS;}
{STAR} 		{yylval.node=initNode("STAR",yytext);return STAR;}
{DIV} 		{yylval.node=initNode("DIV",yytext);return DIV;}
{AND} 		{yylval.node=initNode("AND",yytext);return AND;}
{OR} 		{yylval.node=initNode("OR",yytext);return OR;}
{DOT} 		{yylval.node=initNode("DOT",yytext);return DOT;}
{NOT} 		{yylval.node=initNode("NOT",yytext);return NOT;}
{TYPE} 		{yylval.node=initNode("TYPE",yytext);return TYPE;}
{LP} 		{yylval.node=initNode("LP",yytext);return LP;}
{RP} 		{yylval.node=initNode("RP",yytext);return RP;}
{LB} 		{yylval.node=initNode("LB",yytext);return LB;}
{RB} 		{yylval.node=initNode("RB",yytext);return RB;}
{LC} 		{yylval.node=initNode("LC",yytext);return LC;}
{RC} 		{yylval.node=initNode("RC",yytext);return RC;}
{STRUCT} 	{yylval.node=initNode("STRUCT",yytext);return STRUCT;}
{RETURN} 	{yylval.node=initNode("RETURN",yytext);return RETURN;}
{IF} 		{yylval.node=initNode("IF",yytext);return IF;}
{ELSE} 		{yylval.node=initNode("ELSE",yytext);return ELSE;}
{WHILE} 	{yylval.node=initNode("WHILE",yytext);return WHILE;}
{ID}		{yylval.node=initNode("ID",yytext);return ID;}
.               {errorCount++;printf("Error type A at line %d: Mysterious character '%s'\n",yylineno,yytext);}

%%

