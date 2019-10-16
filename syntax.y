%{
#include "multiway_tree.h"
#include "lex.yy.c"
#define MIN(a,b) a<=b?a:b
char currentStr[200];
Node root;
int errorNum;
void yyerror(char* msg);

%}

%union{
	Node type_node;
	int type_int;
}

%token <type_node> INT
%token <type_node> FLOAT
%token <type_node> ID
%token <type_node> SEMI
%token <type_node> COMMA
%right <type_node> ASSIGNOP
%left <type_node> OR
%left <type_node> AND
%left <type_node> RELOP
%left <type_node> PLUS MINUS
%left <type_node> STAR DIV
%right <type_node> NOT
%left <type_node> DOT LP RP LB RB
%token <type_node> TYPE
%token <type_node> LC
%token <type_node> RC
%token <type_node> STRUCT
%token <type_node> RETURN
%token <type_node> IF
%token <type_node> ELSE
%token <type_node> WHILE

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE



/* declared non-terminals */
%type <type_node> Program ExtDefList ExtDef ExtDecList Specifier StructSpecifier OptTag Tag VarDec FunDec VarList ParamDec CompSt StmtList Stmt DefList Def DecList Dec Exp Args

%%
/* High-level Definitions */
Program : ExtDefList { 
		$$ = createNode("Program","",0,$1->row);
		$$->children=$1;
		root=$$;
	}
;
ExtDefList : ExtDef ExtDefList {
		$$=createNode("ExtDefList","",0,$1->row);
		$$->children=$1;
		$1->siblings=$2;
	}
| /* empty */ {
		$$ = createNode("ExDefList","",1,-1);
	}
;
ExtDef : Specifier ExtDecList SEMI {
		$$ = createNode("ExtDef","",0,$1->row);
		$$->children = $1;
		$1->siblings = $2;
		$2->siblings = $3;
	}
| Specifier SEMI {
		$$ = createNode("ExtDef","",0,$1->row);
		$$->children = $1;
		$1->siblings = $2;
	}
| Specifier FunDec CompSt {
		$$ = createNode("ExtDef","",0,$1->row);
		$$->children = $1;
		$1->siblings = $2;
		$2->siblings = $3;
	}
| Specifier error SEMI{
		errorNum++;
	}
;
ExtDecList : VarDec {
		$$ = createNode("ExtDecList","",0,$1->row);
		$$->children = $1;
	}
| VarDec COMMA ExtDecList {
		$$ = createNode("ExtDecList","",0,$1->row);
		$$->children = $1;
		$1->siblings = $2;
		$2->siblings = $3;
	}
;

/* Specifier */
Specifier : TYPE  {
		$$ = createNode("Specifier","",0,$1->row);
		$$->children = $1;
	}
| StructSpecifier  {
		$$ = createNode("Specifier","",0,$1->row);
		$$->children = $1;
	}
;
StructSpecifier : STRUCT OptTag LC DefList RC {
		$$ = createNode("StructSpecifier","",0,$1->row);
		$$->children = $1;
		$1->siblings = $2;
		$2->siblings = $3;
		$3->siblings = $4;
		$4->siblings = $5;
	}
| STRUCT Tag {
		$$ = createNode("StructSpecifier","",0,$1->row);
		$$->children = $1;
		$1->siblings = $2;
	}
;
OptTag : ID {
		$$ = createNode("OptTag","",0,$1->row);
		$$->children = $1;
	}
| /* empty */{
		$$ = createNode("OptTag","",1,-1);
	}
;
Tag : ID {
		$$ = createNode("Tag","",0,$1->row);
		$$->children = $1;
	}
;

/* Declarators */
VarDec : ID {
		$$ = createNode("VarDec","",0,$1->row);
		$$->children = $1;
	}
| VarDec LB INT RB {
		$$ = createNode("VarDec","",0,$1->row);
		$$->children = $1;
		$1->siblings = $2;
		$2->siblings = $3;
		$3->siblings = $4;
	}
;
FunDec : ID LP VarList RP {
		$$ = createNode("FunDec","",0,$1->row);
		$$->children = $1;
		$1->siblings = $2;
		$2->siblings = $3;
		$3->siblings = $4;
	}
| ID LP RP {
		$$ = createNode("FunDec","",0,$1->row);
		$$->children = $1;
		$1->siblings = $2;
		$2->siblings = $3;
	}
| error RP {
		errorNum++;
	}
;
VarList : ParamDec COMMA VarList {
		$$ = createNode("VarList","",0,$1->row);
		$$->children = $1;
		$1->siblings = $2;
		$2->siblings = $3;
	}
| ParamDec {
		$$ = createNode("VarList","",0,$1->row);
		$$->children = $1;
	}
;
ParamDec : Specifier VarDec {
		$$ = createNode("ParamDec","",0,$1->row);
		$$->children = $1;
		$1->siblings = $2;
	}
;

/* Statements */
CompSt : LC DefList StmtList RC {
		$$ = createNode("CompSt","",0,$1->row);
		$$->children = $1;
		$1->siblings = $2;
		$2->siblings = $3;
		$3->siblings = $4;
	}
| error RC {
		errorNum++;
	}
;
StmtList : Stmt StmtList {
		$$ = createNode("StmtList","",0,$1->row);
		$$->children = $1;
		$1->siblings = $2;
	}
| /* empty */ {
		$$ = createNode("StmtList","",1,-1);
	}
;
Stmt : Exp SEMI {
		$$ = createNode("Stmt","",0,$1->row);
		$$->children = $1;
		$1->siblings = $2;
	}
| CompSt {
		$$ = createNode("Stmt","",0,$1->row);
		$$->children = $1;
	}
| RETURN Exp SEMI {
		$$ = createNode("Stmt","",0,$1->row);
		$$->children = $1;
		$1->siblings = $2;
		$2->siblings = $3;
	}
| IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {
		$$ = createNode("Stmt","",0,$1->row);
		$$->children = $1;
		$1->siblings = $2;
		$2->siblings = $3;
		$3->siblings = $4;
		$4->siblings = $5;
	}
| IF LP Exp RP Stmt ELSE Stmt {
		$$ = createNode("Stmt","",0,$1->row);
		$$->children = $1;
		$1->siblings = $2;
		$2->siblings = $3;
		$3->siblings = $4;
		$4->siblings = $5;
		$5->siblings = $6;
		$6->siblings = $7;
	}
| WHILE LP Exp RP Stmt {
		$$ = createNode("Stmt","",0,$1->row);
		$$->children = $1;
		$1->siblings = $2;
		$2->siblings = $3;
		$3->siblings = $4;
		$4->siblings = $5;
	}
| error SEMI {
		errorNum++;
	}
| error RP {
		errorNum++;
	}
;

/* Local Definitions */
DefList : Def DefList {
		$$ = createNode("DefList","",0,$1->row);
		$$->children = $1;
		$1->siblings = $2;
	}
| /* empty */  {
		$$ = createNode("DefList","",1,-1);
	}
;
Def : Specifier DecList SEMI {
		$$ = createNode("Def","",0,$1->row);
		$$->children = $1;
		$1->siblings = $2;
		$2->siblings = $3;
	}
| Specifier error SEMI {
		errorNum++;
	}
;
DecList : Dec {
		$$ = createNode("DecList","",0,$1->row);
		$$->children = $1;
	}
| Dec COMMA DecList {
		$$ = createNode("DecList","",0,$1->row);
		$$->children = $1;
		$1->siblings = $2;
		$2->siblings = $3;
	}
;
Dec : VarDec {
		$$ = createNode("Dec","",0,$1->row);
		$$->children = $1;
	}
| VarDec ASSIGNOP Exp {
		$$ = createNode("Dec","",0,$1->row);
		$$->children = $1;
		$1->siblings = $2;
		$2->siblings = $3;
	}
;

/* Expressions */
Exp : Exp ASSIGNOP Exp {
		$$ = createNode("Exp","",0,$1->row);
		$$->children = $1;
		$1->siblings = $2;
		$2->siblings = $3;
	}
| Exp AND Exp {
		$$ = createNode("Exp","",0,$1->row);
		$$->children = $1;
		$1->siblings = $2;
		$2->siblings = $3;
	}
| Exp OR Exp {
		$$ = createNode("Exp","",0,$1->row);
		$$->children = $1;
		$1->siblings = $2;
		$2->siblings = $3;
	}
| Exp RELOP Exp {
		$$ = createNode("Exp","",0,$1->row);
		$$->children = $1;
		$1->siblings = $2;
		$2->siblings = $3;
	}
| Exp PLUS Exp {
		$$ = createNode("Exp","",0,$1->row);
		$$->children = $1;
		$1->siblings = $2;
		$2->siblings = $3;
	}
| Exp MINUS Exp {
		$$ = createNode("Exp","",0,$1->row);
		$$->children = $1;
		$1->siblings = $2;
		$2->siblings = $3;
	}
| Exp STAR Exp {
		$$ = createNode("Exp","",0,$1->row);
		$$->children = $1;
		$1->siblings = $2;
		$2->siblings = $3;
	}
| Exp DIV Exp {
		$$ = createNode("Exp","",0,$1->row);
		$$->children = $1;
		$1->siblings = $2;
		$2->siblings = $3;
	}
| LP Exp RP {
		$$ = createNode("Exp","",0,$1->row);
		$$->children = $1;
		$1->siblings = $2;
		$2->siblings = $3;
	}
| MINUS Exp {
		$$ = createNode("Exp","",0,$1->row);
		$$->children = $1;
		$1->siblings = $2;
	}
| NOT Exp {
		$$ = createNode("Exp","",0,$1->row);
		$$->children = $1;
		$1->siblings = $2;
	}
| ID LP Args RP {
		$$ = createNode("Exp","",0,$1->row);
		$$->children = $1;
		$1->siblings = $2;
		$2->siblings = $3;
		$3->siblings = $4;
	}
| ID LP RP {
		$$ = createNode("Exp","",0,$1->row);
		$$->children = $1;
		$1->siblings = $2;
		$2->siblings = $3;
	}
| Exp LB Exp RB {
		$$ = createNode("Exp","",0,$1->row);
		$$->children = $1;
		$1->siblings = $2;
		$2->siblings = $3;
		$3->siblings = $4;
	}
| Exp DOT ID {
		$$ = createNode("Exp","",0,$1->row);
		$$->children = $1;
		$1->siblings = $2;
		$2->siblings = $3;
	}
| ID {
		$$ = createNode("Exp","",0,$1->row);
		$$->children = $1;
	}
| INT {
		$$ = createNode("Exp","",0,$1->row);
		$$->children = $1;
	}
| FLOAT {
		$$ = createNode("Exp","",0,$1->row);
		$$->children = $1;
	}
| Exp LB error RB {
		errorNum++;
}
;

Args : Exp COMMA Args {
		$$ = createNode("Args","",0,$1->row);
		$$->children = $1;
		$1->siblings = $2;
		$2->siblings = $3;
	}
| Exp {
		$$ = createNode("Args","",0,$1->row);
		$$->children = $1;
	}
;

%%
void yyerror(char* msg){
		fprintf(stderr,"Error type B at Line %d: syntax error, near \"%s\"\n",yylineno,currentStr);
}