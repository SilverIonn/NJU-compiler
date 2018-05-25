#include "semantic.h"

void Program(Node *n)
{
	ExtDefList(n->children);
	checkFunc();
}

void ExtDefList(Node *n)
{
	Node *child=n->children;
	if(child!=NULL)
	{
		ExtDef(child);
		ExtDefList(child->bro);
	}
}



void ExtDef(Node *n){	
	Node*child=n->children;
	Type type;
	type=Specifier(child);	
	child=child->bro;
	if(strcmp(child->name,"SEMI")==0)
		return;
	else if(strcmp(child->name,"ExtDecList")==0)
	{
		ExtDecList(child,type);
	}
	else
	{
		Functype f=FunDec(child,type);		

		child=child->bro;
		int i=0;
		if(strcmp(child->name,"SEMI")==0)
		{
			if(f==NULL)return;
			f->isDefined=0;
			i=insertFunc(f,0);
			if(i==1)
				printf("Error type 4 at line %d: Redefined function '%s'\n",f->row,f->name);
			else if(i==2)
				printf("Error type 19 at line %d: Inconsistent declaration of function '%s'\n",f->row,f->name);
		}
		else{
			if(f!=NULL){
				f->isDefined=1;
				i=insertFunc(f,1);
				if(i==1)
					printf("Error type 4 at line %d: Redefined function '%s'\n",f->row,f->name);
				else if(i==2)
					printf("Error type 19 at line %d: Inconsistent declaration of function '%s'\n",f->row,f->name);

			}
			CompSt(child,type);			
		}

	}

}

void ExtDecList(Node *n,Type type)
{
	Node*child=n->children;
	VarDec(child,type,1);
	child=child->bro;
	if(child!=NULL)
	{
		child=child->bro;
		ExtDecList(child,type);
	}
}

/*Specifiers*/
Type Specifier(Node* n)
{
	Node *child=n->children;
	Type type;
	if(strcmp(child->name,"TYPE")==0)		
	{
		type=malloc(sizeof(struct Type_));
		type->kind=0;
		if(strcmp(child->value,"int")==0)
		{
			type->u.basic=INTTYPE;
		}
		else if(strcmp(child->value,"float")==0)
		{
			type->u.basic=FLOATTYPE;
		}
	}
	else //if(strcmp(child->name,"StructSpecifier")==0) 
	{
		type=StructSpecifier(child);
	}
	return type;
}


Type StructSpecifier(Node *n)
{
	Node *child=n->children;
	Type type=malloc(sizeof(struct Type_));
	type->kind=2;
	type->u.structure=malloc(sizeof(struct Structure_));
	type->u.structure->strfield=NULL;
	FieldList f=malloc(sizeof(struct FieldList_));
	f->type=type;
	while(child!=NULL)
	{
		if(strcmp(child->name,"OptTag")==0)
		{
			if(child->children==NULL)
			{
				type->u.structure->name=NULL;
			}
			else{
				type->u.structure->name=malloc(sizeof(child->children->value));
				strcpy(type->u.structure->name,child->children->value);
				f->name=malloc(sizeof(child->children->value));
				strcpy(f->name,child->children->value);
			}
		}
		else if(strcmp(child->name,"Tag")==0)
		{
			FieldList structf=findSymbol(child->children->value);
		
			if(structf==NULL||structf->type->kind!=2||strcmp(structf->name,structf->type->u.structure->name)!=0)
			{
				printf("Error type 17 at line %d: Undefined structure ‘%s’\n",child->lineno,child->children->value);
				return NULL;
			}
			return structf->type;
		}
		else if(strcmp(child->name,"DefList")==0)
		{
			type->u.structure->strfield=DefList(child,2);	 
			
			if(f->type->u.structure->name==NULL)return type;	
			int i=insertTable(f);
			if(i==1)
			{
				printf("Error type 16 at line %d: Duplicated name ‘%s’\n",child->lineno,f->name);
				return NULL;
			}
			return type;
		}
		child=child->bro;
	}
}

/*Declarators*/
FieldList VarDec(Node *n,Type type,int from)		//from=3 func; from=2 struct; from=1 other
{
	
	Node *child=n->children;
	FieldList f;
	if(strcmp(child->name,"ID")==0)
	{
		f=malloc(sizeof(struct FieldList_));
		f->name=malloc(sizeof(child->value));
		strcpy(f->name,child->value);
		f->type=type;
		f->tail=NULL;
		f->hashEqual=NULL;
		if(from==3)return f;	//do not insert this  param now
		int i=insertTable(f);		//insert this ID
		
		if(i==1)
		{
			if(from==1)
				printf("Error type 3 at line %d: Redefined variable'%s'\n",child->lineno,f->name);	
			else
				printf("Error type 15 at line %d: Redefined field ‘%s’\n",child->lineno,f->name);
			return NULL;
		}
		return f;
	}
	else
	{							 
		f=VarDec(child,type,from);
		if(f==NULL)return NULL;
		Type temp=f->type;
		child=child->bro;
		child=child->bro;	//INT
		Type t=malloc(sizeof(struct Type_));
		t->kind=1;		//array
		t->u.array.size=atoi(child->value);
		t->u.array.elem=type;
		if(temp->kind!=1)
		{
			f->type=t;
			return f;
		}
		while(temp->u.array.elem->kind==1)	//find the end of this array
		{
			temp=temp->u.array.elem;
		}
		temp->u.array.elem=t;
		
		return f;
	}
}

Functype FunDec(Node* n,Type type)
{
	Node* child=n->children;
	Functype f=malloc(sizeof(struct Functype_));
	f->name=malloc(sizeof(child->value));
	strcpy(f->name,child->value);
	f->isDefined=0;
	f->row=child->lineno;
	f->ret_type=type;
	f->hashEqual=NULL;
	f->param=NULL;
	child=child->bro;
	child=child->bro;
	if(strcmp(child->name,"VarList")==0)
	{
		f->param=VarList(child);
	}
	return f;
}

FieldList VarList(Node *n)
{
	Node* child=n->children;
	FieldList f;
	f=ParamDec(child);
	child=child->bro;
	if(child!=NULL)
	{
		FieldList p=f;
		child=child->bro;
		if(p==NULL)f=VarList(child);
		else{
			while(p->tail!=NULL)p=p->tail;
			p->tail=VarList(child);
		}
	}
	return f;
}

FieldList ParamDec(Node*n)
{
	Node *child=n->children;
	FieldList f;
	Type type;
	type=Specifier(child);
	f=VarDec(child->bro,type,3);		//from function param
	return f;
}

/*Statements*/
void CompSt(Node * n,Type ret_type)
{
	
	Node *child=n->children;
	child=child->bro;
	DefList(child,1);
	child=child->bro;
	StmtList(child,ret_type);
}

void StmtList(Node *n,Type ret_type)
{
	
	Node *child=n->children;
	if(child==NULL)return;
	Stmt(child,ret_type);
	child=child->bro;
	StmtList(child,ret_type);
}

void Stmt(Node *n,Type ret_type)		
	
{
	
	Node*child=n->children;
	while(child!=NULL)
	{
		if(strcmp(child->name,"RETURN")==0)
		{
			child=child->bro;
			Type t=Exp(child);
			if(ret_type==NULL||t==NULL)return;
			if(!typeEqual(ret_type,t))
			{
				printf("Error type 8 at line %d: Type mismatched for return\n",child->lineno);
			}
			return;
		}
		else if(strcmp(child->name,"LP")==0)
		{
			child=child->bro;
			Type t=Exp(child);
			if(t!=NULL&&!((t->kind==0||t->kind==3)&&t->u.basic==INTTYPE))
			{
				printf("Error type ? conditional statement wrong type\n");
			}
		}
		else if(strcmp(child->name,"Exp")==0)
		{
			Exp(child);
		}
		else if(strcmp(child->name,"Stmt")==0)
		{
			Stmt(child,ret_type);
		}
		child=child->bro;
	}
}

/*Local Definitions*/
FieldList DefList(Node* n,int from)
{
	if(n->children==NULL)return NULL;
	FieldList f;
	Node *child=n->children;
	f=Def(child,from);
	FieldList t=f;
	child=child->bro;
	if(t!=NULL){
		while(t->tail!=NULL)	//find the last field
		{
			t=t->tail;
		}
		t->tail=DefList(child,from);
	}
	else f=DefList(child,from);
	return f;
}

FieldList Def(Node *n,int from)
{
	Node* child=n->children;
	FieldList f;	
	Type type=Specifier(child);
	child=child->bro;
	f=DecList(child,type,from);	
	return f;
}

FieldList DecList(Node *n,Type type,int from)
{
	Node *child=n->children;
	FieldList f;
	f=Dec(child,type,from);
	child=child->bro;
	if(child!=NULL){
		child=child->bro;
		FieldList p=f;
		if(p!=NULL)
		{
			while(p->tail!=NULL)p=p->tail;
			p->tail=DecList(child,type,from);
		}
		else 
			f=DecList(child,type,from);
	}
	return f;
}

FieldList Dec(Node *n,Type type,int from)	
{
	Node *child=n->children;
	FieldList f;
	f=VarDec(child,type,from);
	child=child->bro;
	if(child!=NULL){		
		if(from==2){	//struct cannot be initialized
			printf("Error type 15 at line %d: be initialized field ‘%s’\n",child->lineno,f->name);
			return f;
		}
		child=child->bro;
		Type t=Exp(child);
		if(t!=NULL&&type!=NULL&&!typeEqual(type,t))
		{
			printf("Error type 5 at line %d: Type mismatched for assignment\n",child->lineno);
		}
	}
	return f;
}

/*Expressions*/
Type Exp(Node *n)
{
	Node *child=n->children;
	Type type;
	if(strcmp(child->name,"Exp")==0)
	{	
		Node *child2=child->bro;
		
		if(strcmp(child2->name,"ASSIGNOP")==0)			
		{
			//left value
			Node *leftChild=child->children;
			Type leftType=NULL;
			
			if(strcmp(leftChild->name,"ID")==0&&leftChild->bro==NULL)
				leftType=Exp(child);
			else if(strcmp(leftChild->name,"Exp")==0&&leftChild->bro!=NULL&&strcmp(leftChild->bro->name,"LB")==0)	//array
				leftType=Exp(child);
			else if(strcmp(leftChild->name,"Exp")==0&&leftChild->bro!=NULL&&strcmp(leftChild->bro->name,"DOT")==0)	//struct
			{
				leftType=Exp(child);

			}
			else
				printf("Error type 6 at line %d: The left-hand side of an assignment must be a variable\n",child->lineno);
			child2=child2->bro;
			Type rightType=Exp(child2);
			if(leftType==NULL||rightType==NULL)return NULL;
			if(typeEqual(leftType,rightType))return leftType;
			else
			{
				printf("Error type 5 at line %d: Type mismatched for assignment\n",child->lineno);
				return NULL;
			}
		}
		else if(strcmp(child2->name,"PLUS")==0||strcmp(child2->name,"MINUS")==0||strcmp(child2->name,"STAR")==0||strcmp(child2->name,"DIV")==0||strcmp(child2->name,"RELOP")==0)		//+ - * /
		{
			Type t=Exp(child);
			child2=child2->bro;
			Type t2=Exp(child2);
			if(t==NULL||t2==NULL)return NULL;
			else if((t->kind==0||t->kind==3)&&(t2->kind==0||t2->kind==3)&&t->u.basic==t2->u.basic)return t;
			else
			{
				printf("Error type 7 at line %d: Type mismatched for operands\n",child->lineno);
				return NULL;
			}
		}
		else if(strcmp(child2->name,"LB")==0)	//array
		{
			Type t1=Exp(child);
			
			if(t1==NULL)return NULL;
			if(t1->kind!=1)
			{
				printf("Error type 10 at line %d: '",child->lineno);
				printNode(child);
				printf("' is not an array\n");
				return NULL;
			}
			child2=child2->bro;
			Type t2=Exp(child2);
			if(t2==NULL)return NULL;
			if(!((t2->kind==0||t2->kind==3)&&t2->u.basic==INTTYPE))
			{
				printf("Error type 12 at line %d: '%s' is not an integer\n",child2->lineno,child2->children->value);
				return NULL;
			}
			return t1->u.array.elem;
		}
		else if(strcmp(child2->name,"DOT")==0)	
		{
			Type t1=Exp(child);
			if(t1==NULL)return NULL;
			if(t1->kind!=2)
			{
				printf("Error type 13 at line %d: Illegal use of '.'\n",child->lineno);
				return NULL;
			}
			FieldList fl=t1->u.structure->strfield;
			child2=child2->bro;
			while(fl!=NULL)
			{
				if(strcmp(fl->name,child2->value)==0)
					return fl->type;
				fl=fl->tail;
			}
			printf("Error type 14 at line %d: Non-existent field '%s‘\n",child2->lineno,child2->value);
			return NULL;
		}

	}
	else if(strcmp(child->name,"LP")==0)	
	{
		child=child->bro;
		return Exp(child);
	}
	else if(strcmp(child->name,"MINUS")==0)	
	{
		child=child->bro;
		Type t=Exp(child);
		if(t==NULL)return NULL;
		if(t->kind!=0)
		{
			printf("Error type 7 at line %d: Type mismatched for operands\n",child->lineno);
			return NULL;
		}
		return t;
	}
	else if(strcmp(child->name,"NOT")==0)	
	{
		child=child->bro;
		Type t=Exp(child);
		if(t==NULL)return NULL;
		if(t->kind==0&&t->u.basic==INTTYPE)return t;
		printf("Error type 7 at line %d: Type mismatched for operands\n",child->lineno);
		return NULL;
	}
	else if(strcmp(child->name,"ID")==0&&child->bro!=NULL)	
	{

		FieldList f1=findSymbol(child->value);
		Functype f=findFunc(child->value);
		if(f1!=NULL&&f==NULL)
		{
			printf("Error type 11 at line %d: '%s' is not a function\n",child->lineno,child->value);
			return NULL;
		}
		if(f==NULL||!f->isDefined){
			printf("Error type 2 at line %d: Undefined function '%s'\n",child->lineno,child->value);
			return NULL;
		}
		FieldList param=f->param;
		child=child->bro;
		child=child->bro;
		if(strcmp(child->name,"RP")==0)
		{
			if(param!=NULL)
			{
				printf("Error type 9 at line %d: The method '%s(",child->lineno,f->name);
				printparam(param);
				printf(")'is not applicable for the arguments '()'\n");
			}
		}
		else
		{
			if(!Args(child,param)){
				printf("Error type 9 at line %d: The method '%s(",child->lineno,f->name);
				printparam(param);
				printf(")'is not applicable for the arguments '(");
				printargs(child);
				printf(")'\n");
			}
		}
		return f->ret_type;
	}
	else if(strcmp(child->name,"ID")==0)
	{
		FieldList f=findSymbol(child->value);
		if(f==NULL)
		{
			printf("Error type 1 at line %d: Undefined variable '%s'\n",child->lineno,child->value);	
			return NULL;
		}
		return f->type;
	}
	else if(strcmp(child->name,"INT")==0)
	{
		Type t=malloc(sizeof(struct Type_));
		t->kind=3;
		t->u.basic=INTTYPE;
		return t;
	}
	else if(strcmp(child->name,"FLOAT")==0)
	{
		Type t=malloc(sizeof(struct Type_));
		t->kind=3;
		t->u.basic=FLOATTYPE;
		return t;
	}

}

bool Args(Node* n,FieldList f)
{
	if(n==NULL&&f==NULL)return true;
	else if(n==NULL||f==NULL)return false;
	Node *child=n->children;
	Type t=Exp(child);
	if(t==NULL)return true;		
	if(!typeEqual(t,f->type))return false;
	if(child->bro==NULL&&f->tail==NULL)return true;
	else if(child->bro==NULL||f->tail==NULL)return false;
	return Args(child->bro->bro,f->tail);
}
