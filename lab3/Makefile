all:
	bison -d -v  syntax.y
	flex lexical.l
	gcc  main.c intercode.c semantic.c symboltable.c node.c -ll  -o parser

test1:
    ./parser  ./test/test1.cmm   ./test/out1.ir

test2:
	./parser  ./test/test2.cmm   ./test/out2.ir

test3:
	./parser  ./test/test3.cmm   ./test/out3.ir

test4:
	./parser  ./test/test4.cmm   ./test/out4.ir



