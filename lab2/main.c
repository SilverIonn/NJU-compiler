#include <stdio.h>
#include <stdlib.h>
#include "syntax.tab.c"

int main(int argc, char* argv[])
{
 	if (argc <= 1) return 1;
	FILE* fp = fopen(argv[1],"r");
	if (!fp)
	{
		perror(argv[1]);
		return 1;
	}	
	root=NULL;
	yylineno=1;
	yyrestart(fp);
	yyparse();
	if(errorCount == 0){
		//printTree(root,0);
		initTable();
		Program(root);
	}
	return 0;
}
