
%option yylineno


ID [a-zA-Z_][0-9a-zA-Z_]*

COMMENT_1 "//"[^\n]*
COMMENT_2 "/*"(.|\r|\n)*?"*/"
COMMENT_ERR "/*"?(.|\r|\n)*?"*/"(.|\r|\n)*?"*/"

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
{COMMENT_ERR}	{printf("Error type A at line %d: Illegal comments.\n",yylineno);}
{COMMENT_1}    	{;}
{COMMENT_2} 	{;}
{ws}		{;}
\n		{;}
{FLOAT_2}       {printf("FLOAT\n");}
{FLOAT_2_ERR}   {printf("Error type A at line %d: Illegal float number '%s'\n",yylineno,yytext);}
{FLOAT_1}       {printf("FLOAT\n");}
{FLOAT_1_ERR}   {printf("Error type A at line %d: Illegal float number '%s'\n",yylineno,yytext);}
{INT} 		{printf("INT\n");}
{INT8_ERR}	{printf("Error type A at line %d: Illegal octal number '%s'\n",yylineno,yytext);}
{INT16_ERR}      {printf("Error type A at line %d: Illegal hex number '%s'\n",yylineno,yytext);}
{SEMI} 		{printf("SEMI\n");}
{COMMA} 	{printf("COMMA\n");}
{ASSIGNOP} 	{printf("ASSIGNOP\n");}
{RELOP}	 	{printf("RELOP\n");}
{PLUS} 		{printf("PLUS\n");}
{MINUS} 	{printf("MINUS\n");}
{STAR} 		{printf("STAR\n");}
{DIV} 		{printf("DIV\n");}
{AND} 		{printf("AND\n");}
{OR} 		{printf("OR\n");}
{DOT} 		{printf("DOT\n");}
{NOT} 		{printf("NOT\n");}
{TYPE} 		{printf("TYPE\n");}
{LP} 		{printf("LP\n");}
{RP} 		{printf("RP\n");}
{LB} 		{printf("LB\n");}
{RB} 		{printf("RB\n");}
{LC} 		{printf("LC\n");}
{RC} 		{printf("RC\n");}
{STRUCT} 	{printf("STRUCT\n");}
{RETURN} 	{printf("RETURN\n");}
{IF} 		{printf("IF\n");}
{ELSE} 		{printf("ELSE\n");}
{WHILE} 	{printf("WHILE\n");}
{ID}		{printf("ID\n");}
.               {printf("Error type A at Line %d: Mysterious characters \'%s\'\n",yylineno, yytext);}

%%

