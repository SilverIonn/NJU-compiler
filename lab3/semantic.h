#ifndef _SEMANTIC_H__
#define _SEMANTIC_H__

#include "symboltable.h"


void Program(Node *n);
void ExtDefList(Node *n);
void ExtDef(Node *n);
void ExtDecList(Node *n,Type type);

Type Specifier(Node* n);
Type StructSpecifier(Node* n);

FieldList VarDec(Node *n,Type type,int from);
Functype FunDec(Node *n,Type type);
FieldList VarList(Node *n);
FieldList ParamDec(Node *n);

void CompSt(Node *n,Type ret_type);
void StmtList(Node *n,Type ret_type);
void Stmt(Node *n,Type ret_type);

FieldList DefList(Node* n,int from);
FieldList Def(Node* n,int from);
FieldList DecList(Node *n,Type type,int from);
FieldList Dec(Node *n,Type type,int from);

Type Exp(Node *n);
bool Args(Node *n,FieldList f);

#endif