all:
	bison -d  syntax.y
	flex lexical.l
	gcc  main.c node.c -ll  -o parser

test1:
	./parser ./test/test_1.cmm

test2:
	./parser ./test/test_2.cmm

test3:
	./parser ./test/test_3.cmm

test4:
	./parser ./test/test_4.cmm

test5:
	./parser ./test/test_5.cmm

test6:
	./parser ./test/test_6.cmm

test7:
	./parser ./test/test_7.cmm

test8:
	./parser ./test/test_8.cmm

test9:
	./parser ./test/test_9.cmm

test10:
	./parser ./test/test_10.cmm

