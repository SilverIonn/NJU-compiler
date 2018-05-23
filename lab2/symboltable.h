#ifndef _SYMBOLTABLE_H__
#define _SYMBOLTABLE_H__

#include"node.h"

#define TABLESIZE 32768
#define INTTYPE 0
#define FLOATTYPE 1


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
	char *name;
	FieldList inList;
};

struct FieldList_
{
	char* name;	// 域的名字
	Type type;	// 域的类型
	FieldList tail;	// 下一个域
	FieldList sameHash;
};

struct Functype_
{
	char*name;
	bool isDefined;
	int row;
	Type retype;
	FieldList param;
	Functype sameHash;
};
