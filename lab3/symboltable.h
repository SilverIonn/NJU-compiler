#ifndef _SYMBOLTABLE_H__
#define _SYMBOLTABLE_H__

#include "node.h"
#include "assert.h"

#define TABLESIZE 32768
#define INTTYPE 0
#define FLOATTYPE 1

/*data structure of symbol table */
typedef struct Type_* Type;
typedef struct FieldList_* FieldList;
typedef struct Structure_* Structure;
typedef struct Functype_* Functype;

typedef enum {false, true}bool;

struct Type_
{
	enum { basic, array, structure, constant} kind;
	union
	{
		// 基本类型
		int basic;
		// 数组类型信息包括元素类型与数组大小构成
		struct { Type elem; int size; } array;
		// 结构体类型信息是一个链表
		Structure structure;
	} u;
};

struct Structure_
{
	char *name;          //结构的名字
	FieldList strfield;    //结构的域
};

struct FieldList_
{
	char* name;	         // 域的名字
	Type type;	         // 域的类型
	FieldList tail;	     // 下一个域
	FieldList hashEqual;  //”name“的哈希值相同的构成一个链表
};

struct Functype_
{
	char*name;           //函数的名字
	bool isDefined;      //是否已经被定义
	int row;             //位置信息
	Type ret_type;         //返回值类型
	FieldList param;     //参数链表
	Functype hashEqual;   //”name“的哈希值相同的构成一个链表
};

/*actions on symbol table*/
unsigned int hash_pjw(char* name);

void initTable();
int insertTable(FieldList f);
int insertFunc(Functype f,int type);
void insertParam(Functype f);
FieldList findSymbol(char* name);
Functype findFunc(char* name);
void checkFunc();    //最后检验是否有未定义的函数


/*judgement */
bool paramEqual(FieldList f1,FieldList f2);
bool typeEqual(Type t1,Type y2);

/*print function */
void printparam(FieldList f);
void printargs(Node *n);
void printtype(Type t);
void printNode(Node* n);

#endif