#ifndef _INTERCODE_H
#define _INTERCODE_H

enum{
TEMPVAR , VARIABLE , CONSTANT , VADDRESS , LABEL , FUNCTION , TADDRESS
};
//VADDRESS:variable address     TADDRESS:temp variable address

enum{ASSIGN_K , ADD_K , SUB_K , MUL_K , DIV_K , RETURN_K , LABEL_K  , GOTO_K  , IFGOTO_K , READ_K, WRITE_K 
, CALL_K, ARG_K	, FUNCTION_K , PARAM_K	, DEC_K , RIGHTAT_K };

typedef struct Operand_* Operand;
typedef struct InterCode_* InterCode;
typedef struct Label_No_* Label_No;

//intermediate code struct
struct Operand_
{
	enum {
		tempvar,variable,cons,vaddress,label,
		function,taddress
	}kind;

	union{
		int var_no;		
		char* value;	
		Operand name;  
	}u;
	Operand next;		
};

struct InterCode_
{
	enum{
		assign_k,add_k,sub_k,mul_k,div_k,return_k,label_k,goto_k,
		ifgoto_k,read_k,write_k,call_k,arg_k,function_k,param_k,dec_k,
		rightat_k
	}kind;

	union{
		struct{			    //return ,label,goto,read,write,arg, function
			Operand op;		//param
		}one;
		struct{
			Operand left,right;	//call 
		}assign;	//assign	rightat

		struct{
			Operand result,op1,op2;	
		}binop;			//add sub mul div 

		struct{
			Operand t1;
			char *op;
			Operand t2,label;
		}triop;			//if_goto

		struct{
			Operand op;
			int size;
		}dec;           //dec
	}u;

	InterCode pre,next;
};

struct Label_No_
{
	int no;
	Label_No next;
};

/*code node functions*/
void insertCode(InterCode c);
void deleteCode(InterCode c);
void fwriteAllCode(char *fname);
void fwriteOp(Operand op,FILE *fp);

extern InterCode code_h,code_t;
extern int varCount;
extern int labCount;

int opEqual(Operand op1,Operand op2);

/*optimize code*/
void optGotoCode();
void deleteLabel();
void remove_dead_code();
void same_subexp();
void figure_const();

#endif