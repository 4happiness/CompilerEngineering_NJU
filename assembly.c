#include "assembly.h"


static asmSymtabNode* asmSymtab;
static int frameSize;
static int ptr_T;
static int ptr_S;
static int regT[10][2];
static int regS[8][2];
int param_no;
int param_pos;
int arg_no;


static void partition(InterCodes intercodes);
static InterCode findLabel(int no,InterCodes intercodes);

static void init_reg();
static void init_asmSymtab();
static void init_frame();
static void init();

static void free_asmSymtab();
static void free_asmSymtabNode(asmSymtabNode node);
static void clear_asmSymtab();

static void commonAsm(FILE* fp);
static void asmLabel(InterCode code,FILE* fp);
static void asmFunction(InterCode code,FILE* fp);
static void asmAssign(InterCode code,FILE* fp);
static void asmPlus(InterCode code,FILE* fp);
static void asmMinus(InterCode code,FILE* fp);
static void asmStar(InterCode code,FILE* fp);
static void asmDiv(InterCode code,FILE* fp);
static void asmGoto(InterCode code,FILE* fp);
static void asmCond(InterCode code,FILE* fp);
static void asmReturn(InterCode code,FILE* fp);
static void asmDec(InterCode code,FILE* fp);
static void asmArg(InterCode code,FILE* fp);
static void asmCall(InterCode code,FILE* fp);
static void asmParam(InterCode code,FILE* fp);
static void asmWrite(InterCode code,FILE* fp);
static void asmRead(InterCode code,FILE* fp);

static void pushRa(FILE* fp);
static void popRa(FILE* fp);
static void leave(FILE* fp);

static int getAddr(int no,int kind);
static void addAddr(int no,int kind);
static void add2mem(Operand op,FILE* fp);
static void renewAddr(int num);
static int getT(int no);
static int getS(int no);
static int add2regT(int no,FILE* fp);
static int add2regS(int no,FILE* fp);

static void saveDec(FILE* fp);
static void clearDec();

void generateASM(InterCodes intercodes){
	init();
	partition(intercodes);
	FILE* fp = fopen(path,"w");
	commonAsm(fp);
	
	for(InterCodes cur = intercodes;cur!=NULL;cur=cur->next){
		InterCode code = cur->code;
		InterCode temp=NULL;
		if(cur->next!=NULL)
			temp = cur->next->code;
		switch (code->kind)
		{
			case irLABEL:asmLabel(code,fp);if(temp!=NULL&&temp->blockbeg==1){saveDec(fp);clearDec();}break;
			case irFUNCTION:asmFunction(code,fp);if(temp!=NULL&&temp->blockbeg==1){saveDec(fp);clearDec();}break;
			case irASSIGN:asmAssign(code,fp);if(temp!=NULL&&temp->blockbeg==1){saveDec(fp);clearDec();}break;
			case irPLUS:asmPlus(code,fp);if(temp!=NULL&&temp->blockbeg==1){saveDec(fp);clearDec();}break;
			case irMINUS:asmMinus(code,fp);if(temp!=NULL&&temp->blockbeg==1){saveDec(fp);clearDec();}break;
			case irSTAR:asmStar(code,fp);if(temp!=NULL&&temp->blockbeg==1){saveDec(fp);clearDec();}break;   
			case irDIV:asmDiv(code,fp);if(temp!=NULL&&temp->blockbeg==1){saveDec(fp);clearDec();}break;
			case irGOTO:asmGoto(code,fp);if(temp!=NULL&&temp->blockbeg==1){saveDec(fp);clearDec();}break;
			case irCOND:asmCond(code,fp);if(temp!=NULL&&temp->blockbeg==1){saveDec(fp);clearDec();}break;
			case irRETURN:asmReturn(code,fp);if(temp!=NULL&&temp->blockbeg==1){saveDec(fp);clearDec();}break;
			case irDEC:asmDec(code,fp);if(temp!=NULL&&temp->blockbeg==1){saveDec(fp);clearDec();}break;
			case irARG:asmArg(code,fp);break;
			case irCALL:asmCall(code,fp);if(temp!=NULL&&temp->blockbeg==1){saveDec(fp);clearDec();}break;
			case irPARAM:
				if(param_no==-1){
					for(InterCodes temp = cur;;temp = temp->next){
						InterCode tc = temp->code;
						if(tc->kind==irPARAM)
							param_no++;
						else
							break;
					}
				}
				asmParam(code,fp);
				if(temp!=NULL&&temp->blockbeg==1){saveDec(fp);clearDec();}
				break;
			case irWRITE:asmWrite(code,fp);if(temp!=NULL&&temp->blockbeg==1){saveDec(fp);clearDec();}break;
			case irREAD:asmRead(code,fp);if(temp!=NULL&&temp->blockbeg==1){saveDec(fp);clearDec();}break;
			default:break;
		}
	}
	fclose(fp);
	return;
}

static void init_reg(){
	for(int i=0;i<8;i++){
		regS[i][0] = 0;
		regT[i][0] = 0;
	}
	regT[8][0] = 0;
	regT[9][0] = 0;
	ptr_T = 0;
	ptr_S = 0;
}
static void init_asmSymtab(){
	asmSymtab = (asmSymtabNode*)malloc(3*sizeof(asmSymtabNode));
	for(int i=0;i<3;i++){
		asmSymtab[i]=(asmSymtabNode)malloc(sizeof(struct asmSymtabNode_));
		asmSymtab[i]->no = -1;
		asmSymtab[i]->relAddr = -1;
		asmSymtab[i]->next = NULL;
	}
	/*  i=0 variable
	    i=1 temp
	    i=2 alloc
	 */
}
static void init_frame(){
	frameSize=0;
}
static void init(){
	init_reg();
	init_asmSymtab();
	init_frame();
	param_no=-1;
	arg_no=-1;
	param_pos = 0;
}

static void free_asmSymtab(){
	for(int i=0;i<3;i++)
		free_asmSymtabNode(asmSymtab[i]);
	free(asmSymtab);
}


static void free_asmSymtabNode(asmSymtabNode node){
	if(node==NULL)
		return;
	if(node->next!=NULL)
		free_asmSymtabNode(node->next);

	free(node);
}

static void clear_asmSymtab(){
	for(int i=0;i<3;i++){
		free_asmSymtabNode(asmSymtab[i]->next);
		asmSymtab[i]->next = NULL;
	}		
}

static void commonAsm(FILE* fp){
	fprintf(fp,".data\n");
	fprintf(fp,"_prompt: .asciiz \"Enter an integer:\"\n");
	fprintf(fp,"_ret: .asciiz \"\\n\"\n");
	fprintf(fp,".globl main\n.text\n");
	fprintf(fp,"read:\n  li $v0, 4\n  la $a0, _prompt\n  syscall\n  li $v0, 5\n  syscall\n  jr $ra\n");
	fprintf(fp,"\n");
	fprintf(fp,"write:\n  li $v0, 1\n  syscall\n  li $v0, 4\n  la $a0, _ret\n  syscall\n  move $v0, $0\n  jr $ra\n");

}
static void asmLabel(InterCode code,FILE* fp){
	fprintf(fp,"label%d:\n",code->u.label_no);
}
static void asmFunction(InterCode code,FILE* fp){
	fprintf(fp,"\n%s:\n",code->u.id);
	fprintf(fp,"  addi $sp, $sp, -4\n");
	fprintf(fp,"  sw $fp, 0($sp)\n");
	fprintf(fp,"  move $fp, $sp\n");
	clear_asmSymtab();
	clearDec();
	frameSize = 0;
}
static void asmAssign(InterCode code,FILE* fp){
	Operand result = code->u.assign.left;
	Operand op1 = code->u.assign.right;
	int reg1,reg2;
	int relAddr2;

	add2mem(result,fp);
	add2mem(op1,fp);

	if(op1->kind == irCONSTANT){
		if(result->kind == irVARIABLE){
			reg1 = getS(result->u.value);
			if(reg1 == -1)
				reg1 = add2regS(result->u.value,fp);
			if(result->which==irSELF)
				fprintf(fp,"  li $s%d, %d\n",reg1,op1->u.value);
			else{
				fprintf(fp,"  li $v1,  %d\n",op1->u.value);
				fprintf(fp,"  sw $v1, 0($s%d)\n",reg1);
			}


		}
		else if(result->kind == irTempVAR){
			reg1 = getT(result->u.value);
			if(reg1 == -1)
				reg1 = add2regT(result->u.value,fp);
			if(result->which==irSELF)
				fprintf(fp,"  li $t%d, %d\n",reg1,op1->u.value);
			else{
				fprintf(fp,"  li $v1,  %d\n",op1->u.value);
				fprintf(fp,"  sw $v1, 0($t%d)\n",reg1);
			}
		}
		else;
	}
	else if(op1->kind==irVARIABLE && result->kind==irVARIABLE){
		reg1 = getS(result->u.value);
		if(reg1==-1)
			reg1 = add2regS(result->u.value,fp);

		reg2 = getS(op1->u.value);
		if(reg2==-1)
			reg2 = add2regS(op1->u.value,fp);

		if(reg1==reg2)
			reg1 == add2regS(result->u.value,fp);

		if(op1->which == irSELF){
			if(result->which==irSELF)
				fprintf(fp,"  move $s%d, $s%d\n",reg1,reg2);
			else
				fprintf(fp,"  sw $s%d, 0($s%d)\n",reg2,reg1);
		}
		else if(op1->which == irVALUE){
			fprintf(fp,"  lw $s%d, 0($s%d)\n",reg1,reg2);
		}
		else;
	}
	else if(op1->kind==irVARIABLE && result->kind==irTempVAR){
		reg1 = getT(result->u.value);
		if(reg1==-1)
			reg1 = add2regT(result->u.value,fp);

		reg2 = getS(op1->u.value);
		if(reg2==-1)
			reg2 = add2regS(op1->u.value,fp);


		if(op1->which == irSELF){
			if(result->which==irSELF)
				fprintf(fp,"  move $t%d, $s%d\n",reg1,reg2);
			else
				fprintf(fp,"  sw $s%d, 0($t%d)\n",reg2,reg1);
		}
		else if(op1->which == irVALUE){
			fprintf(fp,"  lw $t%d, 0($s%d)\n",reg1,reg2);
		}
		else;
	}
	else if(op1->kind==irTempVAR && result->kind==irVARIABLE){

		reg1 = getS(result->u.value);
		if(reg1==-1)
			reg1 = add2regS(result->u.value,fp);

		reg2 = getT(op1->u.value);
		if(reg2==-1)
			reg2 = add2regT(op1->u.value,fp);


		if(op1->which == irSELF){
			if(result->which==irSELF)
				fprintf(fp,"  move $s%d, $t%d\n",reg1,reg2);
			else
				fprintf(fp,"  sw $t%d, 0($s%d)\n",reg2,reg1);
		}
		else if(op1->which == irVALUE){
			fprintf(fp,"  lw $s%d, 0($t%d)\n",reg1,reg2);
		}
		else;
	}
	else if(op1->kind==irTempVAR && result->kind==irTempVAR){
		reg1 = getT(result->u.value);
		if(reg1==-1)
			reg1 = add2regT(result->u.value,fp);

		reg2 = getT(op1->u.value);
		if(reg2==-1)
			reg2 = add2regT(op1->u.value,fp);

		if(reg1==reg2)
			reg1 == add2regT(result->u.value,fp);

		if(op1->which == irSELF){
			if(result->which==irSELF)
				fprintf(fp,"  move $t%d, $t%d\n",reg1,reg2);
			else
				fprintf(fp,"  sw $t%d, 0($t%d)\n",reg2,reg1);
		}
		else if(op1->which == irVALUE){
			fprintf(fp,"  lw $t%d, 0($t%d)\n",reg1,reg2);
		}
		else;
	}
	else{   
		reg1=getS(result->u.value);
		if(reg1==-1)
			reg1=add2regS(result->u.value,fp);

		relAddr2 = getAddr(op1->u.value,asmALLOC);
		fprintf(fp,"  la $s%d, -%d($fp)\n", reg1,relAddr2);
	}
}
static void asmPlus(InterCode code,FILE* fp){
	Operand result=code->u.binop.result;
	Operand op1 = code->u.binop.op1;
	Operand op2 = code->u.binop.op2;
	int reg1,reg2,reg3;

	add2mem(result,fp);
	add2mem(op1,fp);
	add2mem(op2,fp);
	if(result->kind==irVARIABLE && op1->kind==irVARIABLE && op2->kind==irVARIABLE){
		reg1 = getS(result->u.value);
		if(reg1==-1)
			reg1 = add2regS(result->u.value,fp);

		reg2 = getS(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regS(op1->u.value,fp);

		if(reg1==reg2)
			reg1 = add2regS(result->u.value,fp);

		reg3 = getS(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regS(op2->u.value,fp);

		if(reg1==reg3){
			reg1 = add2regS(result->u.value,fp);
			if(reg1 == reg2)
				reg2 = add2regS(op1->u.value,fp);
		}
		else if(reg2==reg3){
			reg2 = add2regS(op1->u.value,fp);
			if(reg1==reg2)
				reg1 = add2regS(result->u.value,fp);
		}
		else;

		fprintf(fp,"  add $s%d, $s%d, $s%d\n",reg1,reg2,reg3);
	}
	else if(result->kind==irVARIABLE && op1->kind==irVARIABLE && op2->kind==irTempVAR){
		reg1 = getS(result->u.value);
		if(reg1==-1)
			reg1 = add2regS(result->u.value,fp);

		reg2 = getS(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regS(op1->u.value,fp);

		if(reg1==reg2)
			reg1 = add2regS(result->u.value,fp);

		reg3 = getT(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regT(op2->u.value,fp);

		fprintf(fp,"  add $s%d, $s%d, $t%d\n",reg1,reg2,reg3);
	}
	else if(result->kind==irVARIABLE && op1->kind==irVARIABLE && op2->kind==irCONSTANT){
		reg1 = getS(result->u.value);
		if(reg1==-1)
			reg1 = add2regS(result->u.value,fp);

		reg2 = getS(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regS(op1->u.value,fp);

		if(reg1==reg2)
			reg1 = add2regS(result->u.value,fp);

		fprintf(fp,"  addi $s%d, $s%d, %d\n",reg1,reg2,op2->u.value);
	}
	else if(result->kind==irVARIABLE && op1->kind==irTempVAR && op2->kind==irVARIABLE){
		reg1 = getS(result->u.value);
		if(reg1==-1)
			reg1 = add2regS(result->u.value,fp);

		reg2 = getT(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regT(op1->u.value,fp);

		reg3 = getS(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regS(op2->u.value,fp);

		if(reg1==reg3)
			reg1 = add2regS(result->u.value,fp);

		fprintf(fp,"  add $s%d, $t%d, $s%d\n",reg1,reg2,reg3);
	}
	else if(result->kind==irVARIABLE && op1->kind==irTempVAR && op2->kind==irTempVAR){
		reg1 = getS(result->u.value);
		if(reg1==-1)
			reg1 = add2regS(result->u.value,fp);

		reg2 = getT(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regT(op1->u.value,fp);

		reg3 = getT(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regT(op2->u.value,fp);


		if(reg2==reg3)
			reg2 = add2regT(op1->u.value,fp);

		fprintf(fp,"  add $s%d, $t%d, $t%d\n",reg1,reg2,reg3);
	}
	else if(result->kind==irVARIABLE && op1->kind==irTempVAR && op2->kind==irCONSTANT){
		reg1 = getS(result->u.value);
		if(reg1==-1)
			reg1 = add2regS(result->u.value,fp);

		reg2 = getT(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regT(op1->u.value,fp);


		fprintf(fp,"  addi $s%d, $t%d, %d\n",reg1,reg2,op2->u.value);
	}
	else if(result->kind==irVARIABLE && op1->kind==irCONSTANT && op2->kind==irVARIABLE){
		reg1 = getS(result->u.value);
		if(reg1==-1)
			reg1 = add2regS(result->u.value,fp);

		reg3 = getS(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regS(op2->u.value,fp);

		if(reg1==reg3)
			reg1 = add2regS(result->u.value,fp);

		fprintf(fp,"  addi $s%d, $s%d, %d\n",reg1,reg3,op1->u.value);
	}
	else if(result->kind==irVARIABLE && op1->kind==irCONSTANT && op2->kind==irTempVAR){
		reg1 = getS(result->u.value);
		if(reg1==-1)
			reg1 = add2regS(result->u.value,fp);


		reg3 = getT(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regT(op2->u.value,fp);


		fprintf(fp,"  addi $s%d, $t%d, %d\n",reg1,reg3,op1->u.value);
	}
	else if(result->kind==irVARIABLE && op1->kind==irCONSTANT && op2->kind==irCONSTANT){
		reg1 = getS(result->u.value);
		if(reg1==-1)
			reg1 = add2regS(result->u.value,fp);

		fprintf(fp,"  li $v1, %d\n",op1->u.value);
		fprintf(fp,"  addi $s%d, $v1, %d\n",reg1,op2->u.value);
	}
	else if(result->kind==irTempVAR && op1->kind==irVARIABLE && op2->kind==irVARIABLE){
		reg1 = getT(result->u.value);
		if(reg1==-1)
			reg1 = add2regT(result->u.value,fp);

		reg2 = getS(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regS(op1->u.value,fp);

		reg3 = getS(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regS(op2->u.value,fp);

		if(reg2==reg3){
			reg2 = add2regS(op1->u.value,fp);
		}
		else;

		fprintf(fp,"  add $t%d, $s%d, $s%d\n",reg1,reg2,reg3);
	}
	else if(result->kind==irTempVAR && op1->kind==irVARIABLE && op2->kind==irTempVAR){
		reg1 = getT(result->u.value);
		if(reg1==-1)
			reg1 = add2regT(result->u.value,fp);

		reg2 = getS(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regS(op1->u.value,fp);

		reg3 = getT(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regT(op2->u.value,fp);

		if(reg1==reg3)
			reg1 = add2regT(result->u.value,fp);

		fprintf(fp,"  add $t%d, $s%d, $t%d\n",reg1,reg2,reg3);
	}
	else if(result->kind==irTempVAR && op1->kind==irVARIABLE && op2->kind==irCONSTANT){
		reg1 = getT(result->u.value);
		if(reg1==-1)
			reg1 = add2regT(result->u.value,fp);

		reg2 = getS(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regS(op1->u.value,fp);


		fprintf(fp,"  addi $t%d, $s%d, %d\n",reg1,reg2,op2->u.value);
	}
	else if(result->kind==irTempVAR && op1->kind==irTempVAR && op2->kind==irVARIABLE){
		reg1 = getT(result->u.value);
		if(reg1==-1)
			reg1 = add2regT(result->u.value,fp);

		reg2 = getT(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regT(op1->u.value,fp);

		if(reg1==reg2)
			reg1 = add2regT(result->u.value,fp);

		reg3 = getS(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regS(op2->u.value,fp);

		fprintf(fp,"  add $t%d, $t%d, $s%d\n",reg1,reg2,reg3);
	}
	else if(result->kind==irTempVAR && op1->kind==irTempVAR && op2->kind==irTempVAR){

		reg1 = getT(result->u.value);
		if(reg1==-1)
			reg1 = add2regT(result->u.value,fp);


		reg2 = getT(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regT(op1->u.value,fp);


		if(reg1==reg2){

			reg1 = add2regT(result->u.value,fp);
		}
		reg3 = getT(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regT(op2->u.value,fp);

		if(reg1==reg3){
			reg1 = add2regT(result->u.value,fp);
			if(reg1 == reg2)
				reg2 = add2regT(op1->u.value,fp);
		}
		else if(reg2==reg3){
			reg2 = add2regT(op1->u.value,fp);
			if(reg1==reg2)
				reg1 = add2regT(result->u.value,fp);
		}
		else;

		fprintf(fp,"  add $t%d, $t%d, $t%d\n",reg1,reg2,reg3);
	}
	else if(result->kind==irTempVAR && op1->kind==irTempVAR && op2->kind==irCONSTANT){
		reg1 = getT(result->u.value);
		if(reg1==-1)
			reg1 = add2regT(result->u.value,fp);

		reg2 = getT(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regT(op1->u.value,fp);

		if(reg1==reg2)
			reg1 = add2regT(result->u.value,fp);

		fprintf(fp,"  addi $t%d, $t%d, %d\n",reg1,reg2,op2->u.value);
	}
	else if(result->kind==irTempVAR && op1->kind==irCONSTANT && op2->kind==irVARIABLE){
		reg1 = getT(result->u.value);
		if(reg1==-1)
			reg1 = add2regT(result->u.value,fp);

		reg3 = getS(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regS(op2->u.value,fp);

		fprintf(fp,"  addi $t%d, $s%d, %d\n",reg1,reg3,op1->u.value);
	}
	else if(result->kind==irTempVAR && op1->kind==irCONSTANT && op2->kind==irTempVAR){
		reg1 = getT(result->u.value);
		if(reg1==-1)
			reg1 = add2regT(result->u.value,fp);

		reg3 = getT(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regT(op2->u.value,fp);

		if(reg1==reg3)
			reg1 = add2regT(result->u.value,fp);

		fprintf(fp,"  addi $t%d, $t%d, %d\n",reg1,reg3,op1->u.value);
	}
	else if(result->kind==irTempVAR && op1->kind==irCONSTANT && op2->kind==irCONSTANT){
		reg1 = getT(result->u.value);
		if(reg1==-1)
			reg1 = add2regT(result->u.value,fp);

		fprintf(fp,"  li $v1, %d\n",op1->u.value);
		fprintf(fp,"  addi $t%d, $v1, %d\n",reg1,op2->u.value);
	}
	else;
}
static void asmMinus(InterCode code,FILE* fp){
	Operand result=code->u.binop.result;
	Operand op1 = code->u.binop.op1;
	Operand op2 = code->u.binop.op2;
	int reg1,reg2,reg3;

	add2mem(result,fp);
	add2mem(op1,fp);
	add2mem(op2,fp);
	if(result->kind==irVARIABLE && op1->kind==irVARIABLE && op2->kind==irVARIABLE){
		reg1 = getS(result->u.value);
		if(reg1==-1)
			reg1 = add2regS(result->u.value,fp);

		reg2 = getS(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regS(op1->u.value,fp);

		if(reg1==reg2)
			reg1 = add2regS(result->u.value,fp);

		reg3 = getS(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regS(op2->u.value,fp);

		if(reg1==reg3){
			reg1 = add2regS(result->u.value,fp);
			if(reg1 == reg2)
				reg2 = add2regS(op1->u.value,fp);
		}
		else if(reg2==reg3){
			reg2 = add2regS(op1->u.value,fp);
			if(reg1==reg2)
				reg1 = add2regS(result->u.value,fp);
		}
		else;

		fprintf(fp,"  sub $s%d, $s%d, $s%d\n",reg1,reg2,reg3);
	}
	else if(result->kind==irVARIABLE && op1->kind==irVARIABLE && op2->kind==irTempVAR){
		reg1 = getS(result->u.value);
		if(reg1==-1)
			reg1 = add2regS(result->u.value,fp);

		reg2 = getS(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regS(op1->u.value,fp);

		if(reg1==reg2)
			reg1 = add2regS(result->u.value,fp);

		reg3 = getT(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regT(op2->u.value,fp);

		fprintf(fp,"  sub $s%d, $s%d, $t%d\n",reg1,reg2,reg3);
	}
	else if(result->kind==irVARIABLE && op1->kind==irVARIABLE && op2->kind==irCONSTANT){
		reg1 = getS(result->u.value);
		if(reg1==-1)
			reg1 = add2regS(result->u.value,fp);

		reg2 = getS(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regS(op1->u.value,fp);

		if(reg1==reg2)
			reg1 = add2regS(result->u.value,fp);

		fprintf(fp,"  addi $s%d, $s%d, %d\n",reg1,reg2,-op2->u.value);
	}
	else if(result->kind==irVARIABLE && op1->kind==irTempVAR && op2->kind==irVARIABLE){
		reg1 = getS(result->u.value);
		if(reg1==-1)
			reg1 = add2regS(result->u.value,fp);

		reg2 = getT(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regT(op1->u.value,fp);

		reg3 = getS(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regS(op2->u.value,fp);

		if(reg1==reg3)
			reg1 = add2regS(result->u.value,fp);

		fprintf(fp,"  sub $s%d, $t%d, $s%d\n",reg1,reg2,reg3);
	}
	else if(result->kind==irVARIABLE && op1->kind==irTempVAR && op2->kind==irTempVAR){
		reg1 = getS(result->u.value);
		if(reg1==-1)
			reg1 = add2regS(result->u.value,fp);

		reg2 = getT(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regT(op1->u.value,fp);

		reg3 = getT(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regT(op2->u.value,fp);


		if(reg2==reg3)
			reg2 = add2regT(op1->u.value,fp);

		fprintf(fp,"  sub $s%d, $t%d, $t%d\n",reg1,reg2,reg3);
	}
	else if(result->kind==irVARIABLE && op1->kind==irTempVAR && op2->kind==irCONSTANT){
		reg1 = getS(result->u.value);
		if(reg1==-1)
			reg1 = add2regS(result->u.value,fp);

		reg2 = getT(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regT(op1->u.value,fp);


		fprintf(fp,"  addi $s%d, $t%d, %d\n",reg1,reg2,-op2->u.value);
	}
	else if(result->kind==irVARIABLE && op1->kind==irCONSTANT && op2->kind==irVARIABLE){
		reg1 = getS(result->u.value);
		if(reg1==-1)
			reg1 = add2regS(result->u.value,fp);

		reg3 = getS(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regS(op2->u.value,fp);

		if(reg1==reg3)
			reg1 = add2regS(result->u.value,fp);

		fprintf(fp,"  li $v1, %d\n",op1->u.value);
		fprintf(fp,"  sub $s%d, $v1, $s%d\n",reg1,reg3);
	}
	else if(result->kind==irVARIABLE && op1->kind==irCONSTANT && op2->kind==irTempVAR){
		reg1 = getS(result->u.value);
		if(reg1==-1)
			reg1 = add2regS(result->u.value,fp);


		reg3 = getT(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regT(op2->u.value,fp);

		fprintf(fp,"  li $v1, %d\n",op1->u.value);
		fprintf(fp,"  sub $s%d, $v1, $t%d\n",reg1,reg3);
	}
	else if(result->kind==irVARIABLE && op1->kind==irCONSTANT && op2->kind==irCONSTANT){
		reg1 = getS(result->u.value);
		if(reg1==-1)
			reg1 = add2regS(result->u.value,fp);

		fprintf(fp,"  li $v1, %d\n",op1->u.value);
		fprintf(fp,"  addi $s%d, $v1, %d\n",reg1,-op2->u.value);
	}
	else if(result->kind==irTempVAR && op1->kind==irVARIABLE && op2->kind==irVARIABLE){
		reg1 = getT(result->u.value);
		if(reg1==-1)
			reg1 = add2regT(result->u.value,fp);

		reg2 = getS(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regS(op1->u.value,fp);

		reg3 = getS(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regS(op2->u.value,fp);

		if(reg2==reg3){
			reg2 = add2regS(op1->u.value,fp);
		}
		else;

		fprintf(fp,"  sub $t%d, $s%d, $s%d\n",reg1,reg2,reg3);
	}
	else if(result->kind==irTempVAR && op1->kind==irVARIABLE && op2->kind==irTempVAR){
		reg1 = getT(result->u.value);
		if(reg1==-1)
			reg1 = add2regT(result->u.value,fp);

		reg2 = getS(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regS(op1->u.value,fp);

		reg3 = getT(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regT(op2->u.value,fp);

		if(reg1==reg3)
			reg1 = add2regT(result->u.value,fp);

		fprintf(fp,"  sub $t%d, $s%d, $t%d\n",reg1,reg2,reg3);
	}
	else if(result->kind==irTempVAR && op1->kind==irVARIABLE && op2->kind==irCONSTANT){
		reg1 = getT(result->u.value);
		if(reg1==-1)
			reg1 = add2regT(result->u.value,fp);

		reg2 = getS(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regS(op1->u.value,fp);


		fprintf(fp,"  addi $t%d, $s%d, %d\n",reg1,reg2,-op2->u.value);
	}
	else if(result->kind==irTempVAR && op1->kind==irTempVAR && op2->kind==irVARIABLE){
		reg1 = getT(result->u.value);
		if(reg1==-1)
			reg1 = add2regT(result->u.value,fp);

		reg2 = getT(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regT(op1->u.value,fp);

		if(reg1==reg2)
			reg1 = add2regT(result->u.value,fp);

		reg3 = getS(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regS(op2->u.value,fp);

		fprintf(fp,"  sub $t%d, $t%d, $s%d\n",reg1,reg2,reg3);
	}
	else if(result->kind==irTempVAR && op1->kind==irTempVAR && op2->kind==irTempVAR){

		reg1 = getT(result->u.value);
		if(reg1==-1)
			reg1 = add2regT(result->u.value,fp);


		reg2 = getT(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regT(op1->u.value,fp);


		if(reg1==reg2){

			reg1 = add2regT(result->u.value,fp);
		}
		reg3 = getT(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regT(op2->u.value,fp);

		if(reg1==reg3){
			reg1 = add2regT(result->u.value,fp);
			if(reg1 == reg2)
				reg2 = add2regT(op1->u.value,fp);
		}
		else if(reg2==reg3){
			reg2 = add2regT(op1->u.value,fp);
			if(reg1==reg2)
				reg1 = add2regT(result->u.value,fp);
		}
		else;

		fprintf(fp,"  sub $t%d, $t%d, $t%d\n",reg1,reg2,reg3);
	}
	else if(result->kind==irTempVAR && op1->kind==irTempVAR && op2->kind==irCONSTANT){
		reg1 = getT(result->u.value);
		if(reg1==-1)
			reg1 = add2regT(result->u.value,fp);

		reg2 = getT(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regT(op1->u.value,fp);

		if(reg1==reg2)
			reg1 = add2regT(result->u.value,fp);

		fprintf(fp,"  addi $t%d, $t%d, %d\n",reg1,reg2,-op2->u.value);
	}
	else if(result->kind==irTempVAR && op1->kind==irCONSTANT && op2->kind==irVARIABLE){
		reg1 = getT(result->u.value);
		if(reg1==-1)
			reg1 = add2regT(result->u.value,fp);

		reg3 = getS(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regS(op2->u.value,fp);

		fprintf(fp,"  li $v1, %d\n",op1->u.value);
		fprintf(fp,"  sub $t%d, $v1, $s%d\n",reg1,reg3);
	}
	else if(result->kind==irTempVAR && op1->kind==irCONSTANT && op2->kind==irTempVAR){
		reg1 = getT(result->u.value);
		if(reg1==-1)
			reg1 = add2regT(result->u.value,fp);

		reg3 = getT(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regT(op2->u.value,fp);

		if(reg1==reg3)
			reg1 = add2regT(result->u.value,fp);

		fprintf(fp,"  li $v1, %d\n",op1->u.value);
		fprintf(fp,"  sub $t%d, $v1, $t%d\n",reg1,reg3);
	}
	else if(result->kind==irTempVAR && op1->kind==irCONSTANT && op2->kind==irCONSTANT){
		reg1 = getT(result->u.value);
		if(reg1==-1)
			reg1 = add2regT(result->u.value,fp);

		fprintf(fp,"  li $v1, %d\n",op1->u.value);
		fprintf(fp,"  addi $t%d, $v1, %d\n",reg1,-op2->u.value);
	}
	else;
}
static void asmStar(InterCode code,FILE* fp){
	Operand result=code->u.binop.result;
	Operand op1 = code->u.binop.op1;
	Operand op2 = code->u.binop.op2;
	int reg1,reg2,reg3;

	add2mem(result,fp);
	add2mem(op1,fp);
	add2mem(op2,fp);
	if(result->kind==irVARIABLE && op1->kind==irVARIABLE && op2->kind==irVARIABLE){
		reg1 = getS(result->u.value);
		if(reg1==-1)
			reg1 = add2regS(result->u.value,fp);

		reg2 = getS(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regS(op1->u.value,fp);

		if(reg1==reg2)
			reg1 = add2regS(result->u.value,fp);

		reg3 = getS(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regS(op2->u.value,fp);

		if(reg1==reg3){
			reg1 = add2regS(result->u.value,fp);
			if(reg1 == reg2)
				reg2 = add2regS(op1->u.value,fp);
		}
		else if(reg2==reg3){
			reg2 = add2regS(op1->u.value,fp);
			if(reg1==reg2)
				reg1 = add2regS(result->u.value,fp);
		}
		else;

		fprintf(fp,"  mul $s%d, $s%d, $s%d\n",reg1,reg2,reg3);
	}
	else if(result->kind==irVARIABLE && op1->kind==irVARIABLE && op2->kind==irTempVAR){
		reg1 = getS(result->u.value);
		if(reg1==-1)
			reg1 = add2regS(result->u.value,fp);

		reg2 = getS(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regS(op1->u.value,fp);

		if(reg1==reg2)
			reg1 = add2regS(result->u.value,fp);

		reg3 = getT(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regT(op2->u.value,fp);

		fprintf(fp,"  mul $s%d, $s%d, $t%d\n",reg1,reg2,reg3);
	}
	else if(result->kind==irVARIABLE && op1->kind==irVARIABLE && op2->kind==irCONSTANT){
		reg1 = getS(result->u.value);
		if(reg1==-1)
			reg1 = add2regS(result->u.value,fp);

		reg2 = getS(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regS(op1->u.value,fp);

		if(reg1==reg2)
			reg1 = add2regS(result->u.value,fp);

		fprintf(fp,"  li $v1, %d\n",op2->u.value);
		fprintf(fp,"  mul $s%d, $s%d, $v1\n",reg1,reg2);
	}
	else if(result->kind==irVARIABLE && op1->kind==irTempVAR && op2->kind==irVARIABLE){
		reg1 = getS(result->u.value);
		if(reg1==-1)
			reg1 = add2regS(result->u.value,fp);

		reg2 = getT(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regT(op1->u.value,fp);

		reg3 = getS(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regS(op2->u.value,fp);

		if(reg1==reg3)
			reg1 = add2regS(result->u.value,fp);

		fprintf(fp,"  mul $s%d, $t%d, $s%d\n",reg1,reg2,reg3);
	}
	else if(result->kind==irVARIABLE && op1->kind==irTempVAR && op2->kind==irTempVAR){
		reg1 = getS(result->u.value);
		if(reg1==-1)
			reg1 = add2regS(result->u.value,fp);

		reg2 = getT(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regT(op1->u.value,fp);

		reg3 = getT(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regT(op2->u.value,fp);


		if(reg2==reg3)
			reg2 = add2regT(op1->u.value,fp);


		fprintf(fp,"  mul $s%d, $t%d, $t%d\n",reg1,reg2,reg3);
	}
	else if(result->kind==irVARIABLE && op1->kind==irTempVAR && op2->kind==irCONSTANT){
		reg1 = getS(result->u.value);
		if(reg1==-1)
			reg1 = add2regS(result->u.value,fp);

		reg2 = getT(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regT(op1->u.value,fp);

		fprintf(fp,"  li $v1, %d\n",op2->u.value);
		fprintf(fp,"  mul $s%d, $t%d, $v1\n",reg1,reg2);
	}
	else if(result->kind==irVARIABLE && op1->kind==irCONSTANT && op2->kind==irVARIABLE){
		reg1 = getS(result->u.value);
		if(reg1==-1)
			reg1 = add2regS(result->u.value,fp);

		reg3 = getS(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regS(op2->u.value,fp);

		if(reg1==reg3)
			reg1 = add2regS(result->u.value,fp);

		fprintf(fp,"  li $v0, %d\n",op1->u.value);
		fprintf(fp,"  mul $s%d, $v0, $s%d\n",reg1,reg3);
	}
	else if(result->kind==irVARIABLE && op1->kind==irCONSTANT && op2->kind==irTempVAR){
		reg1 = getS(result->u.value);
		if(reg1==-1)
			reg1 = add2regS(result->u.value,fp);


		reg3 = getT(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regT(op2->u.value,fp);

		fprintf(fp,"  li $v0, %d\n",op1->u.value);
		fprintf(fp,"  mul $s%d, $v0, $t%d\n",reg1,reg3);
	}
	else if(result->kind==irVARIABLE && op1->kind==irCONSTANT && op2->kind==irCONSTANT){
		reg1 = getS(result->u.value);
		if(reg1==-1)
			reg1 = add2regS(result->u.value,fp);

		fprintf(fp,"  li $v0, %d\n",op1->u.value);
		fprintf(fp,"  li $v1, %d\n",op2->u.value);
		fprintf(fp,"  mul $s%d, $v0, $v1\n",reg1);
	}
	else if(result->kind==irTempVAR && op1->kind==irVARIABLE && op2->kind==irVARIABLE){
		reg1 = getT(result->u.value);
		if(reg1==-1)
			reg1 = add2regT(result->u.value,fp);

		reg2 = getS(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regS(op1->u.value,fp);

		reg3 = getS(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regS(op2->u.value,fp);

		if(reg2==reg3){
			reg2 = add2regS(op1->u.value,fp);
		}
		else;

		fprintf(fp,"  mul $t%d, $s%d, $s%d\n",reg1,reg2,reg3);
	}
	else if(result->kind==irTempVAR && op1->kind==irVARIABLE && op2->kind==irTempVAR){
		reg1 = getT(result->u.value);
		if(reg1==-1)
			reg1 = add2regT(result->u.value,fp);

		reg2 = getS(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regS(op1->u.value,fp);

		reg3 = getT(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regT(op2->u.value,fp);

		if(reg1==reg3)
			reg1 = add2regT(result->u.value,fp);

		fprintf(fp,"  mul $t%d, $s%d, $t%d\n",reg1,reg2,reg3);
	}
	else if(result->kind==irTempVAR && op1->kind==irVARIABLE && op2->kind==irCONSTANT){
		reg1 = getT(result->u.value);
		if(reg1==-1)
			reg1 = add2regT(result->u.value,fp);

		reg2 = getS(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regS(op1->u.value,fp);

		fprintf(fp,"  li $v1, %d\n",op2->u.value);
		fprintf(fp,"  mul $t%d, $s%d, $v1\n",reg1,reg2);
	}
	else if(result->kind==irTempVAR && op1->kind==irTempVAR && op2->kind==irVARIABLE){
		reg1 = getT(result->u.value);
		if(reg1==-1)
			reg1 = add2regT(result->u.value,fp);

		reg2 = getT(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regT(op1->u.value,fp);

		if(reg1==reg2)
			reg1 = add2regT(result->u.value,fp);

		reg3 = getS(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regS(op2->u.value,fp);

		fprintf(fp,"  mul $t%d, $t%d, $s%d\n",reg1,reg2,reg3);
	}
	else if(result->kind==irTempVAR && op1->kind==irTempVAR && op2->kind==irTempVAR){

		reg1 = getT(result->u.value);
		if(reg1==-1)
			reg1 = add2regT(result->u.value,fp);


		reg2 = getT(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regT(op1->u.value,fp);


		if(reg1==reg2){

			reg1 = add2regT(result->u.value,fp);
		}
		reg3 = getT(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regT(op2->u.value,fp);

		if(reg1==reg3){
			reg1 = add2regT(result->u.value,fp);
			if(reg1 == reg2)
				reg2 = add2regT(op1->u.value,fp);
		}
		else if(reg2==reg3){
			reg2 = add2regT(op1->u.value,fp);
			if(reg1==reg2)
				reg1 = add2regT(result->u.value,fp);
		}
		else;

		fprintf(fp,"  mul $t%d, $t%d, $t%d\n",reg1,reg2,reg3);
	}
	else if(result->kind==irTempVAR && op1->kind==irTempVAR && op2->kind==irCONSTANT){
		reg1 = getT(result->u.value);
		if(reg1==-1)
			reg1 = add2regT(result->u.value,fp);

		reg2 = getT(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regT(op1->u.value,fp);

		if(reg1==reg2)
			reg1 = add2regT(result->u.value,fp);

		fprintf(fp,"  li $v1, %d\n",op2->u.value);
		fprintf(fp,"  mul $t%d, $t%d, $v1\n",reg1,reg2);
	}
	else if(result->kind==irTempVAR && op1->kind==irCONSTANT && op2->kind==irVARIABLE){
		reg1 = getT(result->u.value);
		if(reg1==-1)
			reg1 = add2regT(result->u.value,fp);

		reg3 = getS(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regS(op2->u.value,fp);

		fprintf(fp,"  li $v0, %d\n",op1->u.value);
		fprintf(fp,"  mul $t%d, $v0, $s%d\n",reg1,reg3);
	}
	else if(result->kind==irTempVAR && op1->kind==irCONSTANT && op2->kind==irTempVAR){
		reg1 = getT(result->u.value);
		if(reg1==-1)
			reg1 = add2regT(result->u.value,fp);

		reg3 = getT(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regT(op2->u.value,fp);

		if(reg1==reg3)
			reg1 = add2regT(result->u.value,fp);

		fprintf(fp,"  li $v0, %d\n",op1->u.value);
		fprintf(fp,"  mul $t%d, $v0, $t%d\n",reg1,reg3);
	}
	else if(result->kind==irTempVAR && op1->kind==irCONSTANT && op2->kind==irCONSTANT){
		reg1 = getT(result->u.value);
		if(reg1==-1)
			reg1 = add2regT(result->u.value,fp);

		fprintf(fp,"  li $v0, %d\n",op1->u.value);
		fprintf(fp,"  li $v1, %d\n",op2->u.value);
		fprintf(fp,"  mul $t%d, $v0, $v1\n",reg1);
	}
	else;
}
static void asmDiv(InterCode code,FILE* fp){
	Operand result=code->u.binop.result;
	Operand op1 = code->u.binop.op1;
	Operand op2 = code->u.binop.op2;
	int reg1,reg2,reg3;
	
	add2mem(result,fp);
	add2mem(op1,fp);
	add2mem(op2,fp);
	if(result->kind==irVARIABLE && op1->kind==irVARIABLE && op2->kind==irVARIABLE){
		reg1 = getS(result->u.value);
		if(reg1==-1)
			reg1 = add2regS(result->u.value,fp);

		reg2 = getS(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regS(op1->u.value,fp);

		if(reg1==reg2)
			reg1 = add2regS(result->u.value,fp);

		reg3 = getS(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regS(op2->u.value,fp);

		if(reg1==reg3){
			reg1 = add2regS(result->u.value,fp);
			if(reg1 == reg2)
				reg2 = add2regS(op1->u.value,fp);
		}
		else if(reg2==reg3){
			reg2 = add2regS(op1->u.value,fp);
			if(reg1==reg2)
				reg1 = add2regS(result->u.value,fp);
		}
		else;

		fprintf(fp,"  div $s%d, $s%d\n",reg2,reg3);
		fprintf(fp,"  mflo $s%d\n",reg1);
	}
	else if(result->kind==irVARIABLE && op1->kind==irVARIABLE && op2->kind==irTempVAR){
		reg1 = getS(result->u.value);
		if(reg1==-1)
			reg1 = add2regS(result->u.value,fp);

		reg2 = getS(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regS(op1->u.value,fp);

		if(reg1==reg2)
			reg1 = add2regS(result->u.value,fp);

		reg3 = getT(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regT(op2->u.value,fp);

		fprintf(fp,"  div $s%d, $t%d\n",reg2,reg3);
		fprintf(fp,"  mflo $s%d\n",reg1);
	}
	else if(result->kind==irVARIABLE && op1->kind==irVARIABLE && op2->kind==irCONSTANT){
		reg1 = getS(result->u.value);
		if(reg1==-1)
			reg1 = add2regS(result->u.value,fp);

		reg2 = getS(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regS(op1->u.value,fp);

		if(reg1==reg2)
			reg1 = add2regS(result->u.value,fp);

		fprintf(fp,"  li $v1, %d\n",op2->u.value);
		fprintf(fp,"  div $s%d, $v1\n",reg2);
		fprintf(fp,"  mflo $s%d\n",reg1);
	}
	else if(result->kind==irVARIABLE && op1->kind==irTempVAR && op2->kind==irVARIABLE){
		reg1 = getS(result->u.value);
		if(reg1==-1)
			reg1 = add2regS(result->u.value,fp);

		reg2 = getT(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regT(op1->u.value,fp);

		reg3 = getS(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regS(op2->u.value,fp);

		if(reg1==reg3)
			reg1 = add2regS(result->u.value,fp);

		fprintf(fp,"  div $t%d, $s%d\n",reg2,reg3);
		fprintf(fp,"  mflo $s%d\n",reg1);
	}
	else if(result->kind==irVARIABLE && op1->kind==irTempVAR && op2->kind==irTempVAR){
		reg1 = getS(result->u.value);
		if(reg1==-1)
			reg1 = add2regS(result->u.value,fp);

		reg2 = getT(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regT(op1->u.value,fp);

		reg3 = getT(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regT(op2->u.value,fp);


		if(reg2==reg3)
			reg2 = add2regT(op1->u.value,fp);


		fprintf(fp,"  div $t%d, $t%d\n",reg2,reg3);
		fprintf(fp,"  mflo $s%d\n",reg1);
	}
	else if(result->kind==irVARIABLE && op1->kind==irTempVAR && op2->kind==irCONSTANT){
		reg1 = getS(result->u.value);
		if(reg1==-1)
			reg1 = add2regS(result->u.value,fp);

		reg2 = getT(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regT(op1->u.value,fp);

		fprintf(fp,"  li $v1, %d\n",op2->u.value);
		fprintf(fp,"  div $t%d, $v1\n",reg2);
		fprintf(fp,"  mflo $s%d\n",reg1);
	}
	else if(result->kind==irVARIABLE && op1->kind==irCONSTANT && op2->kind==irVARIABLE){
		reg1 = getS(result->u.value);
		if(reg1==-1)
			reg1 = add2regS(result->u.value,fp);

		reg3 = getS(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regS(op2->u.value,fp);

		if(reg1==reg3)
			reg1 = add2regS(result->u.value,fp);

		fprintf(fp,"  li $v0, %d\n",op1->u.value);
		fprintf(fp,"  div $v0, $s%d\n",reg3);
		fprintf(fp,"  mflo $s%d\n",reg1);
	}
	else if(result->kind==irVARIABLE && op1->kind==irCONSTANT && op2->kind==irTempVAR){
		reg1 = getS(result->u.value);
		if(reg1==-1)
			reg1 = add2regS(result->u.value,fp);


		reg3 = getT(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regT(op2->u.value,fp);

		fprintf(fp,"  li $v0, %d\n",op1->u.value);
		fprintf(fp,"  div $v0, $t%d\n",reg3);
		fprintf(fp,"  mflo $s%d\n",reg1);
	}
	else if(result->kind==irVARIABLE && op1->kind==irCONSTANT && op2->kind==irCONSTANT){
		reg1 = getS(result->u.value);
		if(reg1==-1)
			reg1 = add2regS(result->u.value,fp);

		fprintf(fp,"  li $v0, %d\n",op1->u.value);
		fprintf(fp,"  li $v1, %d\n",op2->u.value);
		fprintf(fp,"  div $v0, $v1\n");
		fprintf(fp,"  mflo $s%d\n",reg1);
	}
	else if(result->kind==irTempVAR && op1->kind==irVARIABLE && op2->kind==irVARIABLE){
		reg1 = getT(result->u.value);
		if(reg1==-1)
			reg1 = add2regT(result->u.value,fp);

		reg2 = getS(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regS(op1->u.value,fp);

		reg3 = getS(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regS(op2->u.value,fp);

		if(reg2==reg3){
			reg2 = add2regS(op1->u.value,fp);
		}
		else;

		fprintf(fp,"  div $s%d, $s%d\n",reg2,reg3);
		fprintf(fp,"  mflo $t%d\n",reg1);
	}
	else if(result->kind==irTempVAR && op1->kind==irVARIABLE && op2->kind==irTempVAR){
		reg1 = getT(result->u.value);
		if(reg1==-1)
			reg1 = add2regT(result->u.value,fp);

		reg2 = getS(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regS(op1->u.value,fp);

		reg3 = getT(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regT(op2->u.value,fp);

		if(reg1==reg3)
			reg1 = add2regT(result->u.value,fp);

		fprintf(fp,"  div $s%d, $t%d\n",reg2,reg3);
		fprintf(fp,"  mflo $t%d\n",reg1);
	}
	else if(result->kind==irTempVAR && op1->kind==irVARIABLE && op2->kind==irCONSTANT){
		reg1 = getT(result->u.value);
		if(reg1==-1)
			reg1 = add2regT(result->u.value,fp);

		reg2 = getS(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regS(op1->u.value,fp);

		fprintf(fp,"  li $v1, %d\n",op2->u.value);
		fprintf(fp,"  div $s%d, $v1\n",reg2);
		fprintf(fp,"  mflo $t%d\n",reg1);
	}
	else if(result->kind==irTempVAR && op1->kind==irTempVAR && op2->kind==irVARIABLE){
		reg1 = getT(result->u.value);
		if(reg1==-1)
			reg1 = add2regT(result->u.value,fp);

		reg2 = getT(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regT(op1->u.value,fp);

		if(reg1==reg2)
			reg1 = add2regT(result->u.value,fp);

		reg3 = getS(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regS(op2->u.value,fp);

		fprintf(fp,"  div $t%d, $s%d\n",reg2,reg3);
		fprintf(fp,"  mflo $t%d\n",reg1);
	}
	else if(result->kind==irTempVAR && op1->kind==irTempVAR && op2->kind==irTempVAR){

		reg1 = getT(result->u.value);
		if(reg1==-1)
			reg1 = add2regT(result->u.value,fp);


		reg2 = getT(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regT(op1->u.value,fp);


		if(reg1==reg2){

			reg1 = add2regT(result->u.value,fp);
		}
		reg3 = getT(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regT(op2->u.value,fp);

		if(reg1==reg3){
			reg1 = add2regT(result->u.value,fp);
			if(reg1 == reg2)
				reg2 = add2regT(op1->u.value,fp);
		}
		else if(reg2==reg3){
			reg2 = add2regT(op1->u.value,fp);
			if(reg1==reg2)
				reg1 = add2regT(result->u.value,fp);
		}
		else;

		fprintf(fp,"  div $t%d, $t%d\n",reg2,reg3);
		fprintf(fp,"  mflo $t%d\n",reg1);
	}
	else if(result->kind==irTempVAR && op1->kind==irTempVAR && op2->kind==irCONSTANT){
		reg1 = getT(result->u.value);
		if(reg1==-1)
			reg1 = add2regT(result->u.value,fp);

		reg2 = getT(op1->u.value);
		if(reg2 == -1)
			reg2 = add2regT(op1->u.value,fp);

		if(reg1==reg2)
			reg1 = add2regT(result->u.value,fp);

		fprintf(fp,"  li $v1, %d\n",op2->u.value);
		fprintf(fp,"  div $t%d, $v1\n",reg2);
		fprintf(fp,"  mflo $t%d\n",reg1);
	}
	else if(result->kind==irTempVAR && op1->kind==irCONSTANT && op2->kind==irVARIABLE){
		reg1 = getT(result->u.value);
		if(reg1==-1)
			reg1 = add2regT(result->u.value,fp);

		reg3 = getS(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regS(op2->u.value,fp);

		fprintf(fp,"  li $v0, %d\n",op1->u.value);
		fprintf(fp,"  div $v0, $s%d\n",reg3);
		fprintf(fp,"  mflo $t%d\n",reg1);
	}
	else if(result->kind==irTempVAR && op1->kind==irCONSTANT && op2->kind==irTempVAR){
		reg1 = getT(result->u.value);
		if(reg1==-1)
			reg1 = add2regT(result->u.value,fp);

		reg3 = getT(op2->u.value);
		if(reg3 == -1)
			reg3 = add2regT(op2->u.value,fp);

		if(reg1==reg3)
			reg1 = add2regT(result->u.value,fp);

		fprintf(fp,"  li $v0, %d\n",op1->u.value);
		fprintf(fp,"  div $v0, $t%d\n",reg3);
		fprintf(fp,"  mflo $t%d\n",reg1);
	}
	else if(result->kind==irTempVAR && op1->kind==irCONSTANT && op2->kind==irCONSTANT){
		reg1 = getT(result->u.value);
		if(reg1==-1)
			reg1 = add2regT(result->u.value,fp);

		fprintf(fp,"  li $v0, %d\n",op1->u.value);
		fprintf(fp,"  li $v1, %d\n",op2->u.value);
		fprintf(fp,"  div $v0, $v1\n");
		fprintf(fp,"  mflo $t%d\n",reg1);
	}
	else;
}
static void asmGoto(InterCode code,FILE* fp){
	fprintf(fp,"  j label%d\n",code->u.label_no);
}
static void asmCond(InterCode code,FILE* fp){
	Operand op1 = code->u.condition.x;
	Operand op2 = code->u.condition.y;
	int reg1,reg2,reg3;

	add2mem(op1,fp);
	add2mem(op2,fp);
	if(!strcmp("==",code->u.condition.relop)){
		if(op1->kind==irVARIABLE && op2->kind==irVARIABLE){
			reg2 = getS(op1->u.value);
			if(reg2 == -1)
				reg2 = add2regS(op1->u.value,fp);

			reg3 = getS(op2->u.value);
			if(reg3 == -1)
				reg3 = add2regS(op2->u.value,fp);

			if(reg2 == reg3)
				reg2 = add2regS(op1->u.value,fp);

			fprintf(fp,"  beq $s%d, $s%d, label%d\n",reg2,reg3,code->u.condition.label_no);
		}
		else if(op1->kind==irVARIABLE && op2->kind==irTempVAR){
			reg2 = getS(op1->u.value);
			if(reg2 == -1)
				reg2 = add2regS(op1->u.value,fp);

			reg3 = getT(op2->u.value);
			if(reg3 == -1)
				reg3 = add2regT(op2->u.value,fp);

			fprintf(fp,"  beq $s%d, $t%d, label%d\n",reg2,reg3,code->u.condition.label_no);
		}
		else if(op1->kind==irVARIABLE && op2->kind==irCONSTANT){
			reg2 = getS(op1->u.value);
			if(reg2 == -1)
				reg2 = add2regS(op1->u.value,fp);

			fprintf(fp,"  li $v1, %d\n",op2->u.value);
			fprintf(fp,"  beq $s%d, $v1, label%d\n",reg2,code->u.condition.label_no);
		}
		else if(op1->kind==irTempVAR && op2->kind==irVARIABLE){
			reg2 = getT(op1->u.value);
			if(reg2 == -1)
				reg2 = add2regT(op1->u.value,fp);

			reg3 = getS(op2->u.value);
			if(reg3 == -1)
				reg3 = add2regS(op2->u.value,fp);

			fprintf(fp,"  beq $t%d, $s%d, label%d\n",reg2,reg3,code->u.condition.label_no);
		}
		else if(op1->kind==irTempVAR && op2->kind==irTempVAR){
			reg2 = getT(op1->u.value);
			if(reg2 == -1)
				reg2 = add2regT(op1->u.value,fp);

			reg3 = getT(op2->u.value);
			if(reg3 == -1)
				reg3 = add2regT(op2->u.value,fp);

			if(reg2 == reg3)
				reg2 = add2regT(op1->u.value,fp);

			fprintf(fp,"  beq $t%d, $t%d, label%d\n",reg2,reg3,code->u.condition.label_no);

		}
		else if(op1->kind==irTempVAR && op2->kind==irCONSTANT){
			reg2 = getT(op1->u.value);
			if(reg2 == -1)
				reg2 = add2regT(op1->u.value,fp);

			fprintf(fp,"  li $v1, %d\n",op2->u.value);
			fprintf(fp,"  beq $t%d, $v1, label%d\n",reg2,code->u.condition.label_no);

		}
		else if(op1->kind==irCONSTANT && op2->kind==irVARIABLE){
			reg3 = getS(op2->u.value);
			if(reg3 == -1)
				reg3 = add2regS(op2->u.value,fp);

			fprintf(fp,"  li $v0, %d\n",op1->u.value);
			fprintf(fp,"  beq $v0, $s%d, label%d\n",reg3,code->u.condition.label_no);

		}
		else if(op1->kind==irCONSTANT && op2->kind==irTempVAR){
			reg3 = getT(op2->u.value);
			if(reg3 == -1)
				reg3 = add2regT(op2->u.value,fp);

			fprintf(fp,"  li $v0, %d\n",op1->u.value);
			fprintf(fp,"  beq $v0, $t%d, label%d\n",reg3,code->u.condition.label_no);

		}
		else if(op1->kind==irCONSTANT && op2->kind==irCONSTANT){

			fprintf(fp,"  li $v0, %d\n",op1->u.value);
			fprintf(fp,"  li $v1, %d\n",op2->u.value);
			fprintf(fp,"  beq $v0, $v1, label%d\n",code->u.condition.label_no);
		}
		else;
	}
	else if(!strcmp("!=",code->u.condition.relop)){
		if(op1->kind==irVARIABLE && op2->kind==irVARIABLE){
			reg2 = getS(op1->u.value);
			if(reg2 == -1)
				reg2 = add2regS(op1->u.value,fp);

			reg3 = getS(op2->u.value);
			if(reg3 == -1)
				reg3 = add2regS(op2->u.value,fp);

			if(reg2 == reg3)
				reg2 = add2regS(op1->u.value,fp);

			fprintf(fp,"  bne $s%d, $s%d, label%d\n",reg2,reg3,code->u.condition.label_no);

		}
		else if(op1->kind==irVARIABLE && op2->kind==irTempVAR){
			reg2 = getS(op1->u.value);
			if(reg2 == -1)
				reg2 = add2regS(op1->u.value,fp);

			reg3 = getT(op2->u.value);
			if(reg3 == -1)
				reg3 = add2regT(op2->u.value,fp);

			fprintf(fp,"  bne $s%d, $t%d, label%d\n",reg2,reg3,code->u.condition.label_no);
		}
		else if(op1->kind==irVARIABLE && op2->kind==irCONSTANT){
			reg2 = getS(op1->u.value);
			if(reg2 == -1)
				reg2 = add2regS(op1->u.value,fp);


			fprintf(fp,"  li $v1, %d\n",op2->u.value);
			fprintf(fp,"  bne $s%d, $v1, label%d\n",reg2,code->u.condition.label_no);

		}
		else if(op1->kind==irTempVAR && op2->kind==irVARIABLE){
			reg2 = getT(op1->u.value);
			if(reg2 == -1)
				reg2 = add2regT(op1->u.value,fp);

			reg3 = getS(op2->u.value);
			if(reg3 == -1)
				reg3 = add2regS(op2->u.value,fp);


			fprintf(fp,"  bne $t%d, $s%d, label%d\n",reg2,reg3,code->u.condition.label_no);

		}
		else if(op1->kind==irTempVAR && op2->kind==irTempVAR){
			reg2 = getT(op1->u.value);
			if(reg2 == -1)
				reg2 = add2regT(op1->u.value,fp);

			reg3 = getT(op2->u.value);
			if(reg3 == -1)
				reg3 = add2regT(op2->u.value,fp);

			if(reg2 == reg3)
				reg2 = add2regT(op1->u.value,fp);

			fprintf(fp,"  bne $t%d, $t%d, label%d\n",reg2,reg3,code->u.condition.label_no);

		}
		else if(op1->kind==irTempVAR && op2->kind==irCONSTANT){
			reg2 = getT(op1->u.value);
			if(reg2 == -1)
				reg2 = add2regT(op1->u.value,fp);

			fprintf(fp,"  li $v1, %d\n",op2->u.value);
			fprintf(fp,"  bne $t%d, $v1, label%d\n",reg2,code->u.condition.label_no);

		}
		else if(op1->kind==irCONSTANT && op2->kind==irVARIABLE){
			reg3 = getS(op2->u.value);
			if(reg3 == -1)
				reg3 = add2regS(op2->u.value,fp);

			fprintf(fp,"  li $v0, %d\n",op1->u.value);
			fprintf(fp,"  bne $v0, $s%d, label%d\n",reg3,code->u.condition.label_no);

		}
		else if(op1->kind==irCONSTANT && op2->kind==irTempVAR){
			reg3 = getT(op2->u.value);
			if(reg3 == -1)
				reg3 = add2regT(op2->u.value,fp);

			fprintf(fp,"  li $v0, %d\n",op1->u.value);
			fprintf(fp,"  bne $v0, $t%d, label%d\n",reg3,code->u.condition.label_no);

		}
		else if(op1->kind==irCONSTANT && op2->kind==irCONSTANT){

			fprintf(fp,"  li $v0, %d\n",op1->u.value);
			fprintf(fp,"  li $v1, %d\n",op2->u.value);
			fprintf(fp,"  bne $v0, $v1, label%d\n",code->u.condition.label_no);
		}
		else;
	}
	else if(!strcmp(">",code->u.condition.relop)){
		if(op1->kind==irVARIABLE && op2->kind==irVARIABLE){
			reg2 = getS(op1->u.value);
			if(reg2 == -1)
				reg2 = add2regS(op1->u.value,fp);

			reg3 = getS(op2->u.value);
			if(reg3 == -1)
				reg3 = add2regS(op2->u.value,fp);

			if(reg2 == reg3)
				reg2 = add2regS(op1->u.value,fp);

			fprintf(fp,"  bgt $s%d, $s%d, label%d\n",reg2,reg3,code->u.condition.label_no);

		}
		else if(op1->kind==irVARIABLE && op2->kind==irTempVAR){
			reg2 = getS(op1->u.value);
			if(reg2 == -1)
				reg2 = add2regS(op1->u.value,fp);

			reg3 = getT(op2->u.value);
			if(reg3 == -1)
				reg3 = add2regT(op2->u.value,fp);

			fprintf(fp,"  bgt $s%d, $t%d, label%d\n",reg2,reg3,code->u.condition.label_no);
		}
		else if(op1->kind==irVARIABLE && op2->kind==irCONSTANT){
			reg2 = getS(op1->u.value);
			if(reg2 == -1)
				reg2 = add2regS(op1->u.value,fp);


			fprintf(fp,"  li $v1, %d\n",op2->u.value);
			fprintf(fp,"  bgt $s%d, $v1, label%d\n",reg2,code->u.condition.label_no);

		}
		else if(op1->kind==irTempVAR && op2->kind==irVARIABLE){
			reg2 = getT(op1->u.value);
			if(reg2 == -1)
				reg2 = add2regT(op1->u.value,fp);

			reg3 = getS(op2->u.value);
			if(reg3 == -1)
				reg3 = add2regS(op2->u.value,fp);


			fprintf(fp,"  bgt $t%d, $s%d, label%d\n",reg2,reg3,code->u.condition.label_no);

		}
		else if(op1->kind==irTempVAR && op2->kind==irTempVAR){
			reg2 = getT(op1->u.value);
			if(reg2 == -1)
				reg2 = add2regT(op1->u.value,fp);

			reg3 = getT(op2->u.value);
			if(reg3 == -1)
				reg3 = add2regT(op2->u.value,fp);

			if(reg2 == reg3)
				reg2 = add2regT(op1->u.value,fp);

			fprintf(fp,"  bgt $t%d, $t%d, label%d\n",reg2,reg3,code->u.condition.label_no);

		}
		else if(op1->kind==irTempVAR && op2->kind==irCONSTANT){
			reg2 = getT(op1->u.value);
			if(reg2 == -1)
				reg2 = add2regT(op1->u.value,fp);

			fprintf(fp,"  li $v1, %d\n",op2->u.value);
			fprintf(fp,"  bgt $t%d, $v1, label%d\n",reg2,code->u.condition.label_no);

		}
		else if(op1->kind==irCONSTANT && op2->kind==irVARIABLE){
			reg3 = getS(op2->u.value);
			if(reg3 == -1)
				reg3 = add2regS(op2->u.value,fp);

			fprintf(fp,"  li $v0, %d\n",op1->u.value);
			fprintf(fp,"  bgt $v0, $s%d, label%d\n",reg3,code->u.condition.label_no);

		}
		else if(op1->kind==irCONSTANT && op2->kind==irTempVAR){
			reg3 = getT(op2->u.value);
			if(reg3 == -1)
				reg3 = add2regT(op2->u.value,fp);

			fprintf(fp,"  li $v0, %d\n",op1->u.value);
			fprintf(fp,"  bgt $v0, $t%d, label%d\n",reg3,code->u.condition.label_no);

		}
		else if(op1->kind==irCONSTANT && op2->kind==irCONSTANT){

			fprintf(fp,"  li $v0, %d\n",op1->u.value);
			fprintf(fp,"  li $v1, %d\n",op2->u.value);
			fprintf(fp,"  bgt $v0, $v1, label%d\n",code->u.condition.label_no);
		}
		else;
	}
	else if(!strcmp("<",code->u.condition.relop)){
		if(op1->kind==irVARIABLE && op2->kind==irVARIABLE){
			reg2 = getS(op1->u.value);
			if(reg2 == -1)
				reg2 = add2regS(op1->u.value,fp);

			reg3 = getS(op2->u.value);
			if(reg3 == -1)
				reg3 = add2regS(op2->u.value,fp);

			if(reg2 == reg3)
				reg2 = add2regS(op1->u.value,fp);

			fprintf(fp,"  blt $s%d, $s%d, label%d\n",reg2,reg3,code->u.condition.label_no);

		}
		else if(op1->kind==irVARIABLE && op2->kind==irTempVAR){
			reg2 = getS(op1->u.value);
			if(reg2 == -1)
				reg2 = add2regS(op1->u.value,fp);

			reg3 = getT(op2->u.value);
			if(reg3 == -1)
				reg3 = add2regT(op2->u.value,fp);

			fprintf(fp,"  blt $s%d, $t%d, label%d\n",reg2,reg3,code->u.condition.label_no);
		}
		else if(op1->kind==irVARIABLE && op2->kind==irCONSTANT){
			reg2 = getS(op1->u.value);
			if(reg2 == -1)
				reg2 = add2regS(op1->u.value,fp);


			fprintf(fp,"  li $v1, %d\n",op2->u.value);
			fprintf(fp,"  blt $s%d, $v1, label%d\n",reg2,code->u.condition.label_no);

		}
		else if(op1->kind==irTempVAR && op2->kind==irVARIABLE){
			reg2 = getT(op1->u.value);
			if(reg2 == -1)
				reg2 = add2regT(op1->u.value,fp);

			reg3 = getS(op2->u.value);
			if(reg3 == -1)
				reg3 = add2regS(op2->u.value,fp);


			fprintf(fp,"  blt $t%d, $s%d, label%d\n",reg2,reg3,code->u.condition.label_no);

		}
		else if(op1->kind==irTempVAR && op2->kind==irTempVAR){
			reg2 = getT(op1->u.value);
			if(reg2 == -1)
				reg2 = add2regT(op1->u.value,fp);

			reg3 = getT(op2->u.value);
			if(reg3 == -1)
				reg3 = add2regT(op2->u.value,fp);

			if(reg2 == reg3)
				reg2 = add2regT(op1->u.value,fp);

			fprintf(fp,"  blt $t%d, $t%d, label%d\n",reg2,reg3,code->u.condition.label_no);

		}
		else if(op1->kind==irTempVAR && op2->kind==irCONSTANT){
			reg2 = getT(op1->u.value);
			if(reg2 == -1)
				reg2 = add2regT(op1->u.value,fp);

			fprintf(fp,"  li $v1, %d\n",op2->u.value);
			fprintf(fp,"  blt $t%d, $v1, label%d\n",reg2,code->u.condition.label_no);

		}
		else if(op1->kind==irCONSTANT && op2->kind==irVARIABLE){
			reg3 = getS(op2->u.value);
			if(reg3 == -1)
				reg3 = add2regS(op2->u.value,fp);

			fprintf(fp,"  li $v0, %d\n",op1->u.value);
			fprintf(fp,"  blt $v0, $s%d, label%d\n",reg3,code->u.condition.label_no);

		}
		else if(op1->kind==irCONSTANT && op2->kind==irTempVAR){
			reg3 = getT(op2->u.value);
			if(reg3 == -1)
				reg3 = add2regT(op2->u.value,fp);

			fprintf(fp,"  li $v0, %d\n",op1->u.value);
			fprintf(fp,"  blt $v0, $t%d, label%d\n",reg3,code->u.condition.label_no);

		}
		else if(op1->kind==irCONSTANT && op2->kind==irCONSTANT){

			fprintf(fp,"  li $v0, %d\n",op1->u.value);
			fprintf(fp,"  li $v1, %d\n",op2->u.value);
			fprintf(fp,"  blt $v0, $v1, label%d\n",code->u.condition.label_no);
		}
		else;
	}
	else if(!strcmp(">=",code->u.condition.relop)){
		if(op1->kind==irVARIABLE && op2->kind==irVARIABLE){
			reg2 = getS(op1->u.value);
			if(reg2 == -1)
				reg2 = add2regS(op1->u.value,fp);

			reg3 = getS(op2->u.value);
			if(reg3 == -1)
				reg3 = add2regS(op2->u.value,fp);

			if(reg2 == reg3)
				reg2 = add2regS(op1->u.value,fp);

			fprintf(fp,"  bge $s%d, $s%d, label%d\n",reg2,reg3,code->u.condition.label_no);

		}
		else if(op1->kind==irVARIABLE && op2->kind==irTempVAR){
			reg2 = getS(op1->u.value);
			if(reg2 == -1)
				reg2 = add2regS(op1->u.value,fp);

			reg3 = getT(op2->u.value);
			if(reg3 == -1)
				reg3 = add2regT(op2->u.value,fp);

			fprintf(fp,"  bge $s%d, $t%d, label%d\n",reg2,reg3,code->u.condition.label_no);
		}
		else if(op1->kind==irVARIABLE && op2->kind==irCONSTANT){
			reg2 = getS(op1->u.value);
			if(reg2 == -1)
				reg2 = add2regS(op1->u.value,fp);


			fprintf(fp,"  li $v1, %d\n",op2->u.value);
			fprintf(fp,"  bge $s%d, $v1, label%d\n",reg2,code->u.condition.label_no);

		}
		else if(op1->kind==irTempVAR && op2->kind==irVARIABLE){
			reg2 = getT(op1->u.value);
			if(reg2 == -1)
				reg2 = add2regT(op1->u.value,fp);

			reg3 = getS(op2->u.value);
			if(reg3 == -1)
				reg3 = add2regS(op2->u.value,fp);


			fprintf(fp,"  bge $t%d, $s%d, label%d\n",reg2,reg3,code->u.condition.label_no);

		}
		else if(op1->kind==irTempVAR && op2->kind==irTempVAR){
			reg2 = getT(op1->u.value);
			if(reg2 == -1)
				reg2 = add2regT(op1->u.value,fp);

			reg3 = getT(op2->u.value);
			if(reg3 == -1)
				reg3 = add2regT(op2->u.value,fp);

			if(reg2 == reg3)
				reg2 = add2regT(op1->u.value,fp);

			fprintf(fp,"  bge $t%d, $t%d, label%d\n",reg2,reg3,code->u.condition.label_no);

		}
		else if(op1->kind==irTempVAR && op2->kind==irCONSTANT){
			reg2 = getT(op1->u.value);
			if(reg2 == -1)
				reg2 = add2regT(op1->u.value,fp);

			fprintf(fp,"  li $v1, %d\n",op2->u.value);
			fprintf(fp,"  bge $t%d, $v1, label%d\n",reg2,code->u.condition.label_no);

		}
		else if(op1->kind==irCONSTANT && op2->kind==irVARIABLE){
			reg3 = getS(op2->u.value);
			if(reg3 == -1)
				reg3 = add2regS(op2->u.value,fp);

			fprintf(fp,"  li $v0, %d\n",op1->u.value);
			fprintf(fp,"  bge $v0, $s%d, label%d\n",reg3,code->u.condition.label_no);

		}
		else if(op1->kind==irCONSTANT && op2->kind==irTempVAR){
			reg3 = getT(op2->u.value);
			if(reg3 == -1)
				reg3 = add2regT(op2->u.value,fp);

			fprintf(fp,"  li $v0, %d\n",op1->u.value);
			fprintf(fp,"  bge $v0, $t%d, label%d\n",reg3,code->u.condition.label_no);

		}
		else if(op1->kind==irCONSTANT && op2->kind==irCONSTANT){

			fprintf(fp,"  li $v0, %d\n",op1->u.value);
			fprintf(fp,"  li $v1, %d\n",op2->u.value);
			fprintf(fp,"  bge $v0, $v1, label%d\n",code->u.condition.label_no);
		}
		else;
	}
	else if(!strcmp("<=",code->u.condition.relop)){
		if(op1->kind==irVARIABLE && op2->kind==irVARIABLE){
			reg2 = getS(op1->u.value);
			if(reg2 == -1)
				reg2 = add2regS(op1->u.value,fp);

			reg3 = getS(op2->u.value);
			if(reg3 == -1)
				reg3 = add2regS(op2->u.value,fp);

			if(reg2 == reg3)
				reg2 = add2regS(op1->u.value,fp);

			fprintf(fp,"  ble $s%d, $s%d, label%d\n",reg2,reg3,code->u.condition.label_no);

		}
		else if(op1->kind==irVARIABLE && op2->kind==irTempVAR){
			reg2 = getS(op1->u.value);
			if(reg2 == -1)
				reg2 = add2regS(op1->u.value,fp);

			reg3 = getT(op2->u.value);
			if(reg3 == -1)
				reg3 = add2regT(op2->u.value,fp);

			fprintf(fp,"  ble $s%d, $t%d, label%d\n",reg2,reg3,code->u.condition.label_no);
		}
		else if(op1->kind==irVARIABLE && op2->kind==irCONSTANT){
			reg2 = getS(op1->u.value);
			if(reg2 == -1)
				reg2 = add2regS(op1->u.value,fp);


			fprintf(fp,"  li $v1, %d\n",op2->u.value);
			fprintf(fp,"  ble $s%d, $v1, label%d\n",reg2,code->u.condition.label_no);

		}
		else if(op1->kind==irTempVAR && op2->kind==irVARIABLE){
			reg2 = getT(op1->u.value);
			if(reg2 == -1)
				reg2 = add2regT(op1->u.value,fp);

			reg3 = getS(op2->u.value);
			if(reg3 == -1)
				reg3 = add2regS(op2->u.value,fp);


			fprintf(fp,"  ble $t%d, $s%d, label%d\n",reg2,reg3,code->u.condition.label_no);

		}
		else if(op1->kind==irTempVAR && op2->kind==irTempVAR){
			reg2 = getT(op1->u.value);
			if(reg2 == -1)
				reg2 = add2regT(op1->u.value,fp);

			reg3 = getT(op2->u.value);
			if(reg3 == -1)
				reg3 = add2regT(op2->u.value,fp);

			if(reg2 == reg3)
				reg2 = add2regT(op1->u.value,fp);

			fprintf(fp,"  ble $t%d, $t%d, label%d\n",reg2,reg3,code->u.condition.label_no);

		}
		else if(op1->kind==irTempVAR && op2->kind==irCONSTANT){
			reg2 = getT(op1->u.value);
			if(reg2 == -1)
				reg2 = add2regT(op1->u.value,fp);

			fprintf(fp,"  li $v1, %d\n",op2->u.value);
			fprintf(fp,"  ble $t%d, $v1, label%d\n",reg2,code->u.condition.label_no);

		}
		else if(op1->kind==irCONSTANT && op2->kind==irVARIABLE){
			reg3 = getS(op2->u.value);
			if(reg3 == -1)
				reg3 = add2regS(op2->u.value,fp);

			fprintf(fp,"  li $v0, %d\n",op1->u.value);
			fprintf(fp,"  ble $v0, $s%d, label%d\n",reg3,code->u.condition.label_no);

		}
		else if(op1->kind==irCONSTANT && op2->kind==irTempVAR){
			reg3 = getT(op2->u.value);
			if(reg3 == -1)
				reg3 = add2regT(op2->u.value,fp);

			fprintf(fp,"  li $v0, %d\n",op1->u.value);
			fprintf(fp,"  ble $v0, $t%d, label%d\n",reg3,code->u.condition.label_no);

		}
		else if(op1->kind==irCONSTANT && op2->kind==irCONSTANT){

			fprintf(fp,"  li $v0, %d\n",op1->u.value);
			fprintf(fp,"  li $v1, %d\n",op2->u.value);
			fprintf(fp,"  ble $v0, $v1, label%d\n",code->u.condition.label_no);
		}
		else;
	}
	else;
}
static void asmReturn(InterCode code,FILE* fp){
	Operand result = code->u.ret;
	int reg1;

	if(result->kind == irCONSTANT){
		fprintf(fp,"  li $v0, %d\n",code->u.ret->u.value);
	}

	else{
		add2mem(result,fp);
		if(result->kind == irVARIABLE){
			reg1 = getS(result->u.value);
			if(reg1 == -1)
				reg1 = add2regS(result->u.value,fp);
			fprintf(fp,"  move $v0, $s%d\n",reg1);

		}
		else if(result->kind == irTempVAR){
			reg1 = getT(result->u.value);
			if(reg1 == -1)
				reg1 = add2regT(result->u.value,fp);
			fprintf(fp,"  move $v0, $t%d\n",reg1);
		}
		else;
	}
	//clearDec();
	leave(fp);
	fprintf(fp,"  jr $ra\n");
}
static void asmDec(InterCode code,FILE* fp){
	fprintf(fp,"  addi $sp, $sp, -%d\n",code->u.dec.size);
	frameSize += code->u.dec.size;
	addAddr(code->u.dec.alloc->u.value,asmALLOC);
}
static void asmArg(InterCode code,FILE* fp){
	Operand result = code->u.call.x;
	add2mem(result,fp);
	int reg;
	arg_no++;
	if(arg_no==0){
		saveDec(fp);
		pushRa(fp);
	}
		
	if(arg_no<=3){
		switch (result->kind)
		{
		case irVARIABLE:
			reg = getS(result->u.value);
			if(reg==-1)
				reg = add2regS(result->u.value,fp);
			fprintf(fp,"  move $a%d, $s%d\n",arg_no,reg);
			break;
		
		case irTempVAR:
			reg = getT(result->u.value);
			if(reg==-1)
				reg = add2regT(result->u.value,fp);
			fprintf(fp,"  move $a%d, $t%d\n",arg_no,reg);
			break;
			
		case irCONSTANT:
			fprintf(fp,"  li $v1, %d\n",result->u.value);
			fprintf(fp,"  move $a%d, $v1\n",arg_no);
			break;	

		default:
			break;
		}
	}
	else{
		fprintf(fp,"  addi $sp, $sp, -4\n");
		frameSize+=4;
		switch (result->kind)
		{
		case irVARIABLE:
			reg = getS(result->u.value);
			if(reg==-1)
				reg = add2regS(result->u.value,fp);
			
			fprintf(fp,"  sw $s%d, 0($sp)\n",reg);
			break;
		
		case irTempVAR:
			reg = getT(result->u.value);
			if(reg==-1)
				reg = add2regT(result->u.value,fp);
			fprintf(fp,"  sw $t%d, 0($sp)\n",reg);
			break;
			
		case irCONSTANT:
			fprintf(fp,"  li $v1, %d\n",result->u.value);
			fprintf(fp,"  sw $v1, 0($sp)\n");
			break;	

		default:
			break;
		}
	}
}
static void asmCall(InterCode code,FILE* fp){
	if(arg_no==-1){
		saveDec(fp);
		pushRa(fp);
	}
	clearDec();
	Operand result = code->u.call.x;
	int reg;
	fprintf(fp,"  jal %s\n",code->u.call.func);
	if(arg_no>=4){
		fprintf(fp,"  addi $sp, $sp, %d\n",4*(arg_no-3));
		frameSize-=(4*(arg_no-3));
	}
	popRa(fp);
	add2mem(result,fp);
	switch (result->kind)
	{
	case irVARIABLE:
		reg = getS(result->u.value);
		if(reg==-1)
			reg = add2regS(result->u.value,fp);
		fprintf(fp,"  move $s%d, $v0\n",reg);
		break;
	
	case irTempVAR:
		reg = getT(result->u.value);
		if(reg==-1)
			reg = add2regT(result->u.value,fp);
		fprintf(fp,"  move $t%d, $v0\n",reg);
		break;

	default:
		break;
	}
	arg_no=-1;
}
static void asmParam(InterCode code,FILE* fp){
	Operand result = code->u.param;
	int reg;
	add2mem(result,fp);
	if(param_no<=3){
		param_pos = 0;
		switch (result->kind)
		{
		case irVARIABLE:
			reg = getS(result->u.value);
			if(reg==-1)
				reg = add2regS(result->u.value,fp);
			fprintf(fp,"  move $s%d, $a%d\n",reg,param_no);
			break;

		case irTempVAR:
			reg = getT(result->u.value);
			if(reg==-1)
				reg = add2regT(result->u.value,fp);
			fprintf(fp,"  move $t%d, $a%d\n",reg,param_no);
			break;
		
		default:
			break;
		}
	}
	else{
		param_pos+=4;
		switch (result->kind)
		{
		case irVARIABLE:
			reg = getS(result->u.value);
			if(reg==-1)
				reg = add2regS(result->u.value,fp);
			fprintf(fp,"  lw $s%d, %d($fp)\n",reg,param_pos);
			break;

		case irTempVAR:
			reg = getT(result->u.value);
			if(reg==-1)
				reg = add2regT(result->u.value,fp);
			fprintf(fp,"  lw $t%d, %d($fp)\n",reg,param_pos);
			break;
		
		default:
			break;
		}
	}
	param_no--;
}
static void asmWrite(InterCode code,FILE* fp){
	Operand result = code->u.write;
	int reg1;

	add2mem(result,fp);
	switch (result->kind)
	{
		case irVARIABLE:
			reg1 = getS(result->u.value);
			if(reg1==-1)
				reg1 = add2regS(result->u.value,fp);
			saveDec(fp);
			pushRa(fp);
			fprintf(fp,"  move $a0, $s%d\n",reg1);
			clearDec();
			break;

		case irTempVAR:
			reg1 = getT(result->u.value);
			if(reg1==-1)
				reg1 = add2regT(result->u.value,fp);
			saveDec(fp);
			pushRa(fp);
			fprintf(fp,"  move $a0, $t%d\n",reg1);
			clearDec();
			break;

		case irCONSTANT:
			saveDec(fp);
			pushRa(fp);
			fprintf(fp,"  li $v1, %d\n",result->u.value);
			fprintf(fp,"  move $a0, $v1\n");
			clearDec();
			break;

		default:
			break;
	}
	
	fprintf(fp, "  jal write\n");
	popRa(fp);
}
static void asmRead(InterCode code,FILE* fp){
	Operand result = code->u.read;
	saveDec(fp);
	clearDec();
	pushRa(fp);
	fprintf(fp,"  jal read\n");
	popRa(fp);
	add2mem(result,fp);
	if(result->kind == irVARIABLE){
		int reg = getS(result->u.value);
		if(reg==-1)
			reg=add2regS(result->u.value,fp);
		fprintf(fp,"  move $s%d, $v0\n",reg);
	}
	else if(result->kind ==irTempVAR){
		int reg = getT(result->u.value);
		if(reg==-1)
			reg=add2regT(result->u.value,fp);
		fprintf(fp,"  move $t%d, $v0\n",reg);
	}
	else;
}

static void pushRa(FILE* fp){
	fprintf(fp,"  addi $sp, $sp, -4\n");
	fprintf(fp,"  sw $ra, 0($sp)\n");
	frameSize+=4;
}
static void popRa(FILE* fp){
	fprintf(fp,"  lw $ra, 0($sp)\n");
	fprintf(fp,"  addi $sp, $sp, 4\n");
	frameSize-=4;
}
static void leave(FILE* fp){
	fprintf(fp,"  move $sp $fp\n");
	fprintf(fp,"  lw $fp, 0($sp)\n");
	fprintf(fp,"  addi $sp, $sp, 4\n");
}

static void freeFrame(){

}

static int getT(int no){

	for(int i=0;i<10;i++)
		if((regT[i][0]==1)&&(regT[i][1]==no)){
			return i;
		}
	return -1;
}
static int getS(int no){
	for(int i=0;i<8;i++)
		if(regS[i][0]==1&&regS[i][1]==no)
			return i;
	return -1;
}

static int add2regT(int no,FILE* fp){
	/*for(int i=0;i<10;i++)
	  printf("%d ",regT[i][1]);
	  printf("\n");
	 */
	if(regT[ptr_T][0]==1){
		int relAddr = getAddr(regT[ptr_T][1],asmTEMP);
		if(relAddr >=0)
			fprintf(fp,"  sw $t%d, -%d($fp)\n",ptr_T,relAddr);
	}
	else
		regT[ptr_T][0]=1;
	regT[ptr_T][1] = no;
	int ret = ptr_T;
	fprintf(fp,"  lw $t%d, -%d($fp)\n",ptr_T,getAddr(no,asmTEMP));
	ptr_T = (ptr_T+1)%10;
	return ret;
}

static int add2regS(int no,FILE* fp){
	if(regS[ptr_S][0]==1){
		int relAddr = getAddr(regS[ptr_S][1],asmVARIABLE);
		if(relAddr >=0)
			fprintf(fp,"  sw $s%d, -%d($fp)\n",ptr_S,relAddr);
	}
	else{
		regS[ptr_S][0]=1;
	}

	regS[ptr_S][1] = no;
	int ret = ptr_S;
	fprintf(fp,"  lw $s%d, -%d($fp)\n",ptr_S,getAddr(no,asmVARIABLE));
	ptr_S = (ptr_S+1)%8;
	return ret;
}

static void saveDec(FILE* fp){
	for(int i=0;i<8;i++){
		if(regS[i][0]==1){
			int relAddr = getAddr(regS[i][1],asmVARIABLE);
			if(relAddr>=0)
				fprintf(fp,"  sw $s%d, -%d($fp)\n",i,relAddr);
		}
	}
	for(int i=0;i<10;i++){
		if(regT[i][0]==1){
			int relAddr = getAddr(regT[i][1],asmTEMP);
			if(relAddr>=0)
				fprintf(fp,"  sw $t%d, -%d($fp)\n",i,relAddr);
		}
	}
}

static void clearDec(){
	for(int i=0;i<8;i++)
		regS[i][0]=0;
	for(int i=0;i<10;i++)
		regT[i][0]=0;
	ptr_S = 0;
	ptr_T = 0;
}


static int getAddr(int no,int kind){
	int i=0;
	if(kind == asmVARIABLE)
		i = 0;
	else if(kind == asmTEMP)
		i = 1;
	else
		i = 2;
	for(asmSymtabNode cur = asmSymtab[i]->next;cur!=NULL;cur=cur->next)
		if(no == cur->no)
			return cur->relAddr;
	return -1;
}

static void addAddr(int no,int kind){
	int i=0;
	if(kind == asmVARIABLE)
		i = 0;
	else if(kind == asmTEMP)
		i = 1;
	else
		i = 2;
	asmSymtabNode node = (asmSymtabNode)malloc(sizeof(struct asmSymtabNode_));
	node->no = no;
	node->relAddr = frameSize;
	node->next = asmSymtab[i]->next;
	asmSymtab[i]->next = node;
}
static void add2mem(Operand op,FILE* fp){
	int relAddr;
	switch (op->kind)
	{
		case irVARIABLE:
			relAddr = getAddr(op->u.value,asmVARIABLE);
			if(relAddr==-1){
				fprintf(fp,"  addi $sp, $sp, -4\n");
				frameSize += 4;
				addAddr(op->u.value,asmVARIABLE);
			}
			break;

		case irTempVAR:
			relAddr = getAddr(op->u.value,asmTEMP);
			if(relAddr==-1){
				fprintf(fp,"  addi $sp, $sp, -4\n");
				frameSize += 4;
				addAddr(op->u.value,asmTEMP);
			}
			break;

		default:
			break;
	}
}
static void partition(InterCodes intercodes){
	for(InterCodes cur=intercodes;cur!=NULL;cur=cur->next){
		InterCode code = cur->code;
		switch (code->kind)
		{
		case irLABEL:
			code->blockbeg = 1;
			break;
		
		case irGOTO:
			code->blockbeg=1;
			break;

		case irCOND:
			code->blockbeg = 1;
			break;
		
		default:
			code->blockbeg = code->blockbeg | 0;
			break;
		}
	}
}
static InterCode findLabel(int no,InterCodes intercodes){
	for(InterCodes cur=intercodes;cur!=NULL;cur=cur->next){
		InterCode code = cur->code;
		if(code->kind==irLABEL&&code->u.label_no==no)
			return code;
	}
	return NULL;
}