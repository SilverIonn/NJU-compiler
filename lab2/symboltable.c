#include "symboltable.h"

FieldList hashTable[TABLESIZE];         
Functype funcTable[TABLESIZE];


unsigned int hash_pjw(char* name)
{
	unsigned int val = 0, i;
	for (; *name; ++name)
	{
		val = (val << 2) + *name;
		if (i = val & ~0x3fff) val = (val ^ (i >> 12)) & 0x3fff;	
	}
	return val;
}

void initTable() 
{
	int i;
	for(i=0;i<TABLESIZE;i++)
	{
		hashTable[i]=NULL;
		funcTable[i]=NULL;
	}
}

int insertTable(FieldList f)
{
	if(f->name==NULL)return 0;	
	unsigned int no= hash_pjw(f->name);
	if(hashTable[no]==NULL)
	{
		hashTable[no]=f;
	}
	else 
	{
		FieldList q=hashTable[no];
		if(strcmp(q->name,f->name)==0)return 1;	     //检测到发生重定义 返回值为1
		while(q->sameHash!=NULL)
		{
			q=q->sameHash;
			if(strcmp(q->name,f->name)==0)return 1;	 //检测到发生重定义 返回值为1
		}
		q->sameHash=f;
	}
	return 0;
}

int insertFunc(Functype f,int type)		//type=0: 函数声明   type=1:函数定义
{
	if(f->name==NULL)return 0;		
	unsigned int no= hash_pjw(f->name);
	if(funcTable[no]==NULL)
	{
		funcTable[no]=f;
		insertParam(f);
	}
	else 
	{
		Functype q=funcTable[no];
		if(strcmp(q->name,f->name)==0)
		{
			if(q->isDefined)
				return 1;	              //检测到函数已经定义，返回值为1
			else
			{
				if(!typeEqual(q->retype,f->retype)||!paramEqual(q->param,f->param))
					return 2;	          //检测到函数声明不一致，返回值为2
				q->isDefined=type;
				return 0;
			}
		}
		while(q->sameHash!=NULL)
		{
			q=q->sameHash;
			if(strcmp(q->name,f->name)==0)
			{
				if(q->isDefined)
					return 1;	          //检测到函数已经定义，返回值为1
				else
				{
					if(!typeEqual(q->retype,f->retype)||!paramEqual(q->param,f->param))
						return 2;	      //检测到函数声明不一致，返回值为2
					q->isDefined=type;
					return 0;
				}
			}
		}
		q->sameHash=f;
		insertParam(f);
	}
	return 0;
}

void insertParam(Functype f)
{
	FieldList param=f->param;
	int i=0;
	while(param!=NULL)
	{
		i=insertTable(param);
		if(i==1)
		{
			printf("Error type 3 at line %d: Redefined variable'%s'\n",f->row,param->name);
		}
		param=param->tail;
	}
}




FieldList findSymbol(char *name){	          
	unsigned int no=hash_pjw(name);
	if(hashTable[no]==NULL)return NULL;
	FieldList p=hashTable[no];
	while(p!=NULL){
		if(strcmp(p->name,name)==0)return p;
		p=p->sameHash;
	}
	return NULL;
}

Functype findFunc(char *name)
{
	unsigned int no=hash_pjw(name);
	if(funcTable[no]==NULL)return NULL;
	Functype p=funcTable[no];
	while(p!=NULL){
		if(strcmp(p->name,name)==0)return p;
		p=p->sameHash;
	}
	return NULL;
}

void checkFunc()
{
	int i;
	for(i=0;i<TABLESIZE;i++)
	{
		if(funcTable[i]!=NULL)
		{
			Functype p=funcTable[i];
			while(p!=NULL)
			{
				if(!p->isDefined)
					printf("Error type 18 at line %d: Undefined function '%s'\n",p->row,p->name);
				p=p->sameHash;
			}
		}
	}
}

//比较函数的参数或struct的域
bool paramEqual(FieldList f1,FieldList f2)
{
	if(f1==NULL&&f2==NULL)
		return true;
	if(f1==NULL||f2==NULL)
		return false;
	if(typeEqual(f1->type,f2->type))return paramEqual(f1->tail,f2->tail);
	else
		return false;
}

//比较两个类型
bool typeEqual(Type t1,Type t2)
{
	if(t1->kind==0&&t2->kind==3)     
	{
		if(t1->u.basic!=t2->u.basic)
		{
			return false;
		}
	}
	else if(t1->kind!=t2->kind)
	{
		return false;
	}
	else
	{
		if(t1->kind==0)	        //basic
		{
			if(t1->u.basic!=t2->u.basic)
				return false;
		}
		else if(t1->kind==2)	//struct
		{
			//if a struct do not has  a name 
			if(t1->u.structure->name==NULL||t2->u.structure->name==NULL)
			{
				return paramEqual(t1->u.structure->inList,t2->u.structure->inList);								
			}
			if(strcmp(t1->u.structure->name,t2->u.structure->name)!=0)
				return false;
		}
		else if(t1->kind==1)        //array
		{
			return typeEqual(t1->u.array.elem,t2->u.array.elem);
		}
	}
	return true;
}


void printparam(FieldList f)
{
	while(f!=NULL)
	{
		printtype(f->type);
		f=f->tail;
	}
}

void printargs(Node *n)
{
	Node *child=n->children;
	Type t=Exp(child);
	if(t==NULL)return;
	printtype(t);
	child=child->next;
	if(child==NULL)return;
	child=child->next;
	printargs(child);
}

void printtype(Type t){
	if((t->kind==0||t->kind==3)&&t->u.basic==INTTYPE)
		printf(" int ");
	else if((t->kind==0||t->kind==3)&&t->u.basic==FLOATTYPE)
		printf(" float ");
	else if(t->kind==2)
		printf("struct %s ",t->u.structure->name);
	else if(t->kind==1){
		printtype(t->u.array.elem);
		printf("[]");
	}
}

void printNode(Node *n)
{
	Node *child=n->children;
	if(child==NULL)
	{
		printf(" %s",n->value);
		return;
	}
	while(child!=NULL)
	{
		printNode(child);
		child=child->next;
	}
}