all:
	bison -d -v  syntax.y
	flex lexical.l
	gcc  main.c objcode.c intercode.c semantic.c symboltable.c node.c -ll  -o parser

test1:
	./parser ./test/test1.cmm ./test/out1.s

test2:
	./parser ./test/test2.cmm ./test/out2.s
