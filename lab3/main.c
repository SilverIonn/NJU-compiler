#include <stdio.h>
#include <stdlib.h>
#include "syntax.tab.c"

int main(int argc, char* argv[])
{
 	if (argc <= 1) return -1;
	FILE* f = fopen(argv[1],"r");
	if (!f)
	{
		perror(argv[1]);
		return 1;
	}	
	root=NULL;
	yylineno=1;
	yyrestart(f);
	yyparse();
	if(errorCount==0){
		//printTree(root,0);	
		initTable();
		Program(root);
		//optimize inter code
		optIF();	//label
		rmLabel();
		lookCon();		//temp
		rddCode();		//variable
		sameRight();
		if(argc<=2)	return 1;
		printCode(argv[2]);
	}
	return 0;
}
