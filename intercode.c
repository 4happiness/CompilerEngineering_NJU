#include "intercode.h"
#include "parseTree.h"
#include "symtab.h"
#include "parseProduction.h"
#include "common.h"

static int LABEL_NO;
static int TempVAR_NO;
static int AllocVAR_NO;
static int FLAG;

static int new_label();
static int new_tvar();
static int new_alloc();
static void printOp(Operand);
static void printIR(InterCodes);
static void deleteIR(InterCodes intercodes);
static Operand new_operand();
static void freeOperand(Operand op);
static InterCode new_intercode();
static void freeInterCode(InterCode intercode);
static InterCodes new_intercodes();
static Operand cpyOperand(Operand src);
static InterCodes concatenate(InterCodes dest,InterCodes src);
static InterCodes translate_Program(Node Program);
static InterCodes translate_CompSt(Node CompSt);
static InterCodes translate_ExtDef(Node ExtDef);
static InterCodes translate_ExtDefList(Node ExtDefList);
static InterCodes translate_FunDec(Node FunDec);
static InterCodes translate_VarList(Node VarList);
static InterCodes translate_ParamDec(Node ParamDec);
static InterCodes translate_Def(Node Def);
static InterCodes translate_DefList(Node DefList);
static InterCodes translate_Dec(Node Dec);
static InterCodes translate_DecList(Node DecList);
static InterCodes translate_Exp(Node Exp,Operand place);
static InterCodes translate_Stmt(Node Stmt);
static InterCodes translate_StmtList(Node StmtList);
static InterCodes translate_Cond(Node Exp,int label_true, int label_false);
static InterCodes translate_Args(Node Args,ArgsList* args_list);

static int new_label(){
    int ret = LABEL_NO;
    LABEL_NO++;
    return ret;
}

static Operand new_tempvar(){
    Operand temp = (Operand)malloc(sizeof(struct Operand_));
    temp->kind = irTempVAR;
    temp->which = irSELF;
    temp->u.value = TempVAR_NO;
    TempVAR_NO++;
    return temp;
}

static Operand new_allocvar(){
    Operand alloc = (Operand)malloc(sizeof(struct Operand_));
    alloc->kind = irAllocVAR;
    alloc->which = irSELF;
    alloc->u.value =AllocVAR_NO;
    AllocVAR_NO++;
    return alloc;
}
static Operand new_operand(){
    Operand ret = (Operand)malloc(sizeof(struct Operand_));
    return ret;
}
static InterCode new_intercode(){
    InterCode ret = (InterCode)malloc(sizeof(struct InterCode_));
    return ret;
}
static InterCodes new_intercodes(){
    InterCodes ret = (InterCodes)malloc(sizeof(struct InterCodes_));
    ret->next=NULL;
    ret->prev=NULL;
    return ret;
}

void generateIR(Node Program){
    LABEL_NO=1;
    TempVAR_NO=1;
    AllocVAR_NO=1;
    FLAG=1;
    InterCodes intercodes = translate_Program(Program);
    if(FLAG)
        printIR(intercodes);
    else
        printf("Cannot translate: Code contains variables of multi-dimensional array type or parameters of array type.\n");

    deleteIR(intercodes);
}

static void deleteIR(InterCodes intercodes){
    if(intercodes==NULL)
        return;
    if(intercodes->next!=NULL)
        deleteIR(intercodes->next);
    intercodes->next=NULL;
    freeInterCode(intercodes->code);
    free(intercodes);
}

static InterCodes concatenate(InterCodes dest,InterCodes src){
    if(dest==NULL)
        return src;
    InterCodes tail = dest;
    while(tail->next!=NULL)
        tail=tail->next;
    tail->next=src;
    if(src!=NULL)
        src->prev=tail;
    return dest;
}

static Operand cpyOperand(Operand src){
    if(src==NULL)
        return NULL;
    Operand dest = (Operand)malloc(sizeof(struct Operand_));
    dest->which = src->which;
    dest->kind = src->kind;
    switch (src->kind)
    {
    case irVARIABLE:
        strcpy(dest->u.id,src->u.id);
        break;

    case irCONSTANT:
    case irTempVAR:
    case irAllocVAR:
        dest->u.value = src->u.value;
        break;

    default:
        break;
    }
    return dest;
}
static void freeOperand(Operand op){
    if(op!=NULL)
        free(op);
}

static InterCodes translate_Program(Node Program){
    if(Program==NULL)
        return NULL;
    Node ExtDefList = Program->children;
    return translate_ExtDefList(ExtDefList);
}

static InterCodes translate_ExtDef(Node ExtDef){
    Node FunDec = ExtDef->children->siblings;
    if(!strcmp("FunDec",FunDec->node_name)){
        Node CompSt = FunDec->siblings;
        InterCodes code1 = translate_FunDec(FunDec);
        InterCodes code2 = translate_CompSt(CompSt);
        return concatenate(code1,code2);
    }
    else{
        return NULL;
    }
}

static InterCodes translate_ExtDefList(Node ExtDefList){
    if(ExtDefList==NULL)
        return NULL;
    if(ExtDefList->children==NULL)
        return NULL;
    Node ExtDef = ExtDefList->children;
    InterCodes code1 = translate_ExtDef(ExtDef);
    InterCodes code2 = NULL;
    if(ExtDef->siblings!=NULL)
        code2 = translate_ExtDefList(ExtDef->siblings);
    return concatenate(code1,code2);
}

static InterCodes translate_FunDec(Node FunDec){
    
    Node ID = FunDec->children;
    Node VarList = ID->siblings->siblings;
    InterCodes code1 = new_intercodes();
    code1->code = new_intercode();
    code1->code->kind = irFUNCTION;
    strcpy(code1->code->u.id,ID->node_value);

    InterCodes code2 = NULL;
    if(!strcmp("VarList",VarList->node_name))
        code2 = translate_VarList(VarList);

    return concatenate(code1,code2);
}

static InterCodes translate_CompSt(Node CompSt){
    Node DefList = CompSt->children->siblings;
    Node StmtList = DefList->siblings;
    InterCodes code1 = translate_DefList(DefList);
    InterCodes code2 = translate_StmtList(StmtList);
    return concatenate(code1,code2);
}

static InterCodes translate_VarList(Node VarList){
    if(VarList==NULL)
        return NULL;
    Node ParamDec = VarList->children;
    InterCodes code1 = translate_ParamDec(ParamDec);
    InterCodes code2 = NULL;
    if(ParamDec->siblings!=NULL){
        code2 = translate_VarList(ParamDec->siblings->siblings);
    }
    return concatenate(code1,code2);
}
static InterCodes translate_ParamDec(Node ParamDec){
    Node VarDec = ParamDec->children->siblings;
    Node ID = VarDec->children;
    while(ID->children!=NULL)
        ID = ID->children;
    InterCodes intercode = new_intercodes();
    intercode->code =  new_intercode();
    intercode->code->kind = irPARAM;
    intercode->code->u.param = new_operand();
    intercode->code->u.param->kind = irVARIABLE;
    intercode->code->u.param->which = irSELF;
    strcpy(intercode->code->u.param->u.id,ID->node_value);
    return intercode;
}

static InterCodes translate_Def(Node Def){
    if(Def==NULL)
        return NULL;
    Node DecList = Def->children->siblings;
    return translate_DecList(DecList);
}
static InterCodes translate_DefList(Node DefList){
    if(DefList==NULL)
        return NULL;
    if(DefList->children==NULL)
        return NULL;
    Node Def = DefList->children;
    InterCodes code1 = translate_Def(Def);
    InterCodes code2 = translate_DefList(Def->siblings);
    return concatenate(code1,code2);
}

static InterCodes translate_DecList(Node DecList){
    Node Dec = DecList->children;
    InterCodes code1 = translate_Dec(Dec);
    InterCodes code2 = NULL;
    if(Dec->siblings!=NULL)
        code2 = translate_DecList(Dec->siblings->siblings);
    return concatenate(code1,code2);
}

static InterCodes translate_Dec(Node Dec){
    Node VarDec = Dec->children;
    Node ID = VarDec->children;
    while(ID->children!=NULL)
        ID=ID->children;
    
    Type type = getType(ID->node_value,VARIABLE);
    int size = sizeofType(type);
    InterCodes code1 = NULL;
    InterCodes code2 = NULL;
    if(type->kind!=BASIC){
        code1 = new_intercodes();
        code1->code = new_intercode();
        code1->code->kind = irDEC;
        code1->code->u.dec.size = size;
        Operand alloc = new_allocvar();
        code1->code->u.dec.alloc = alloc;

        code2 = new_intercodes();
        code2->code = new_intercode();
        code2->code->kind = irASSIGN;
        Operand left = new_operand();
        Operand right = cpyOperand(alloc);
        left->which = irSELF;
        left->kind = irVARIABLE;
        strcpy(left->u.id,ID->node_value);
        right->which = irADDRESS;;
        code2->code->u.assign.left = left;
        code2->code->u.assign.right = right;
    }

    InterCodes code3 = NULL;
    InterCodes code4 = NULL;
    if(VarDec->siblings!=NULL){
        Node Exp = VarDec->siblings->siblings;
        Operand t1 = new_tempvar();
        t1->which = irVALUE;
        code3 = translate_Exp(Exp,t1);
        t1->which = irSELF;
        
        code4 = new_intercodes();
        code4->code = new_intercode();
        code4->code->kind = irASSIGN;
        code4->code->u.assign.right = t1;
        Operand left = new_operand();
        left->kind = irVARIABLE;
        left->which = irSELF;
        strcpy(left->u.id,ID->node_value);
        code4->code->u.assign.left = left;

    }

    code3 = concatenate(code3,code4);
    code2 = concatenate(code2,code3);

    return concatenate(code1,code2);
}

static InterCodes translate_Exp(Node Exp,Operand place){
    if(!strcmp("INT",Exp->children->node_name)){
        if(place!=NULL){
            place->kind = irCONSTANT;
            place->which = irSELF;
            place->u.value = atoi(Exp->children->node_value);
        }
        return NULL;
    }
    else if(!strcmp("ID",Exp->children->node_name)){
        Node ID = Exp->children;
        if(ID->siblings==NULL){
            if(place==NULL)
                return NULL;
            Type type = getType(ID->node_value,VARIABLE);
            if(type->kind==BASIC){
                place->kind = irVARIABLE;
                place->which = irSELF;
                strcpy(place->u.id,ID->node_value);
                return NULL;
            }
            else{
                InterCodes intercodes = new_intercodes();
                intercodes->code = new_intercode();
                intercodes->code->kind = irASSIGN;
                Operand t1 = new_tempvar();
                t1->kind = irVARIABLE;
                strcpy(t1->u.id,ID->node_value);
                if(place->which == irSELF)
                    t1->which = irSELF;
                else
                    t1->which = irVALUE;

                intercodes->code->u.assign.right = t1;
                intercodes->code->u.assign.left = cpyOperand(place);
                intercodes->code->u.assign.left->which = irSELF;

                return intercodes;

            }
        }
        else if(!strcmp("LP",ID->siblings->node_name)){
            Node Args = ID->siblings->siblings;
            if(!strcmp("Args",Args->node_name)){
                ArgsList args_list = NULL;
                InterCodes code1 = translate_Args(Args,&args_list);
                

                if(!strcmp("write",ID->node_value)){
                    InterCodes code2 = new_intercodes();
                    code2->code = new_intercode();
                    code2->code->kind = irWRITE;
                    code2->code->u.write = args_list->x;
                    return concatenate(code1,code2);
                }
                else{
                    InterCodes code2 = NULL;
                    for(ArgsList current = args_list;current!=NULL;current=current->next){
                        InterCodes temp = new_intercodes();
                        temp->code = new_intercode();
                        temp->code->kind = irARG;
                        temp->code->u.arg = current->x;
                        code2 = concatenate(code2,temp);
                        
                    }
                    InterCodes temp = new_intercodes();
                    temp->code =  new_intercode();
                    temp->code->kind = irCALL;
                    if(place!=NULL){
                        temp->code->u.call.x = cpyOperand(place);
                    }
                    else{
                        temp->code->u.call.x = new_tempvar();
                    }
                    temp->code->u.call.x->which = irSELF;
                    strcpy(temp->code->u.call.func,ID->node_value);

                    code2 = concatenate(code2,temp);
                    return concatenate(code1,code2);
                }
            }
            else{
                if(!strcmp("read",ID->node_value)){
                    InterCodes code1 = new_intercodes();
                    code1->code = new_intercode();
                    code1->code->kind = irREAD;
                    if(place!=NULL)
                        code1->code->u.read = cpyOperand(place);
                    else
                        code1->code->u.read = new_tempvar();
                    code1->code->u.read->which = irSELF;
                    return code1;
                }
                else{
                    InterCodes code1 = new_intercodes();
                    code1->code =  new_intercode();
                    code1->code->kind = irCALL;
                    if(place!=NULL)
                        code1->code->u.call.x = cpyOperand(place);
                    else
                        code1->code->u.call.x = new_tempvar();
                    code1->code->u.call.x->which = irSELF;
                    strcpy(code1->code->u.call.func,ID->node_value);
                    return code1;
                }
            }
        }
        else{
            return NULL;
        }
    }
    else if(!strcmp("Exp",Exp->children->node_name)){
        Node Exp_1 = Exp->children;
        if(!strcmp("ASSIGNOP",Exp_1->siblings->node_name)){
            Node Exp_2 = Exp_1->siblings->siblings;
            Operand t1 = new_tempvar();
            Operand t2 = new_tempvar();
            t2->which = irVALUE;
            InterCodes code1 = translate_Exp(Exp_2,t2);
            t2->which = irSELF;

            InterCodes code2 = translate_Exp(Exp_1,t1);

            InterCodes code3 = new_intercodes();
            code3->code = new_intercode();
            code3->code->kind = irASSIGN;
            code3->code->u.assign.right = t2;
            code3->code->u.assign.left = cpyOperand(t1);

            if(t1->kind != irVARIABLE)
                code3->code->u.assign.left->which = irVALUE;

            InterCodes code4 = NULL;
            if(place!=NULL){
                if(t1->kind == irVARIABLE){
                    place->kind  = irVARIABLE;
                    place->which = irSELF;
                    strcpy(place->u.id,t1->u.id);
                    freeOperand(t1);
                }
                else{
                    code4 = new_intercodes();
                    code4->code = new_intercode();
                    code4->code->kind = irASSIGN;
                    code4->code->u.assign.right = t1;
                    code4->code->u.assign.left = cpyOperand(place);
                    if(place->which==irVALUE){
                        code4->code->u.assign.right->which = irVALUE;
                    }
                    else
                    {
                        code4->code->u.assign.right->which = irSELF;
                    }
                    code4->code->u.assign.left->which= irSELF;
                }    
            }
            else
            {
                freeOperand(t1);
            }
            code3 = concatenate(code3,code4);
            code2 = concatenate(code2,code3);
            return concatenate(code1,code2);
            
        }
        else if(!strcmp("AND",Exp_1->siblings->node_name)){
            int label1 = new_label();
            int label2 = new_label();
            InterCodes code0 = NULL;
            if(place!=NULL){
                code0 = new_intercodes();
                code0->code = new_intercode();
                code0->code->kind = irASSIGN;
                code0->code->u.assign.left = cpyOperand(place);
                code0->code->u.assign.left->which = irSELF;
                code0->code->u.assign.right =new_operand();
                code0->code->u.assign.right->which = irSELF;
                code0->code->u.assign.right->kind = irCONSTANT;
                code0->code->u.assign.right->u.value = 0;  
            }
            InterCodes code1 = translate_Cond(Exp,label1,label2);

            InterCodes code2 = new_intercodes();
            code2->code = new_intercode();
            code2->code->kind = irLABEL;
            code2->code->u.label_no = label1;

            InterCodes temp = NULL;
            if(place!=NULL){
                temp = new_intercodes();
                temp->code = new_intercode();
                temp->code->kind = irASSIGN;
                temp->code->u.assign.left = cpyOperand(place);
                temp->code->u.assign.left->which = irSELF;
                temp->code->u.assign.right = new_operand();
                temp->code->u.assign.right->kind = irCONSTANT;
                temp->code->u.assign.right->which = irSELF;
                temp->code->u.assign.right->u.value = 1;  
            }   

            code2 = concatenate(code2,temp);

            temp = new_intercodes();
            temp->code = new_intercode();
            temp->code->kind = irLABEL;
            temp->code->u.label_no = label2;

            code2 = concatenate(code2,temp);
            code1 = concatenate(code1,code2);
            return concatenate(code0,code1); 
        }
        else if(!strcmp("OR",Exp_1->siblings->node_name)){
            int label1 = new_label();
            int label2 = new_label();
            InterCodes code0 = NULL;
            if(place!=NULL){
                code0 = new_intercodes();
                code0->code = new_intercode();
                code0->code->kind = irASSIGN;
                code0->code->u.assign.left = cpyOperand(place);
                code0->code->u.assign.left->which = irSELF;
                code0->code->u.assign.right =new_operand();
                code0->code->u.assign.right->which = irSELF;
                code0->code->u.assign.right->kind = irCONSTANT;
                code0->code->u.assign.right->u.value = 0;  
            }
            InterCodes code1 = translate_Cond(Exp,label1,label2);

            InterCodes code2 = new_intercodes();
            code2->code = new_intercode();
            code2->code->kind = irLABEL;
            code2->code->u.label_no = label1;

            InterCodes temp = NULL;
            if(place!=NULL){
                temp = new_intercodes();
                temp->code = new_intercode();
                temp->code->kind = irASSIGN;
                temp->code->u.assign.left = cpyOperand(place);
                temp->code->u.assign.left->which = irSELF;
                temp->code->u.assign.right = new_operand();
                temp->code->u.assign.right->kind = irCONSTANT;
                temp->code->u.assign.right->which = irSELF;
                temp->code->u.assign.right->u.value = 1;  
            }   

            code2 = concatenate(code2,temp);

            temp = new_intercodes();
            temp->code = new_intercode();
            temp->code->kind = irLABEL;
            temp->code->u.label_no = label2;

            code2 = concatenate(code2,temp);
            code1 = concatenate(code1,code2);
            return concatenate(code0,code1); 
        }
        else if(!strcmp("RELOP",Exp_1->siblings->node_name)){
            int label1 = new_label();
            int label2 = new_label();
            InterCodes code0 = NULL;
            if(place!=NULL){
                code0 = new_intercodes();
                code0->code = new_intercode();
                code0->code->kind = irASSIGN;
                code0->code->u.assign.left = cpyOperand(place);
                code0->code->u.assign.left->which = irSELF;
                code0->code->u.assign.right =new_operand();
                code0->code->u.assign.right->which = irSELF;
                code0->code->u.assign.right->kind = irCONSTANT;
                code0->code->u.assign.right->u.value = 0;  
            }
            InterCodes code1 = translate_Cond(Exp,label1,label2);

            InterCodes code2 = new_intercodes();
            code2->code = new_intercode();
            code2->code->kind = irLABEL;
            code2->code->u.label_no = label1;

            InterCodes temp = NULL;
            if(place!=NULL){
                temp = new_intercodes();
                temp->code = new_intercode();
                temp->code->kind = irASSIGN;
                temp->code->u.assign.left = cpyOperand(place);
                temp->code->u.assign.left->which = irSELF;
                temp->code->u.assign.right = new_operand();
                temp->code->u.assign.right->kind = irCONSTANT;
                temp->code->u.assign.right->which = irSELF;
                temp->code->u.assign.right->u.value = 1;  
            }   

            code2 = concatenate(code2,temp);

            temp = new_intercodes();
            temp->code = new_intercode();
            temp->code->kind = irLABEL;
            temp->code->u.label_no = label2;

            code2 = concatenate(code2,temp);
            code1 = concatenate(code1,code2);
            return concatenate(code0,code1); 
        }
        else if(!strcmp("PLUS",Exp_1->siblings->node_name)){
            Node Exp_2 = Exp_1->siblings->siblings;

            Operand t1 = new_tempvar();
            Operand t2 = new_tempvar();

            t1->which = irVALUE;
            t2->which = irVALUE;
            InterCodes code1 = translate_Exp(Exp_1,t1);
            InterCodes code2 = translate_Exp(Exp_2,t2);
            t1->which = irSELF;
            t2->which = irSELF;

            InterCodes code3 = NULL;
            if(place==NULL){
                freeOperand(t1);
                freeOperand(t2);
            }
            if(place!=NULL){
                code3 = new_intercodes();
                code3->code =  new_intercode();
                code3->code->kind = irPLUS;
                code3->code->u.binop.op1=t1;
                code3->code->u.binop.op2=t2;
                code3->code->u.binop.result = cpyOperand(place);
                code3->code->u.binop.result->which = irSELF;
            }
            code2 = concatenate(code2,code3);

            return concatenate(code1,code2);

        }
        else if(!strcmp("MINUS",Exp_1->siblings->node_name)){
            Node Exp_2 = Exp_1->siblings->siblings;

            Operand t1 = new_tempvar();
            Operand t2 = new_tempvar();

            t1->which = irVALUE;
            t2->which = irVALUE;
            InterCodes code1 = translate_Exp(Exp_1,t1);
            InterCodes code2 = translate_Exp(Exp_2,t2);
            t1->which = irSELF;
            t2->which = irSELF;

            InterCodes code3 = NULL;
            if(place==NULL){
                freeOperand(t1);
                freeOperand(t2);
            }
            if(place!=NULL){
                code3 = new_intercodes();
                code3->code =  new_intercode();
                code3->code->kind = irMINUS;
                code3->code->u.binop.op1=t1;
                code3->code->u.binop.op2=t2;
                code3->code->u.binop.result = cpyOperand(place);
                code3->code->u.binop.result->which = irSELF;
            }
            code2 = concatenate(code2,code3);

            return concatenate(code1,code2);

        }
        else if(!strcmp("STAR",Exp_1->siblings->node_name)){
            Node Exp_2 = Exp_1->siblings->siblings;

            Operand t1 = new_tempvar();
            Operand t2 = new_tempvar();

            t1->which = irVALUE;
            t2->which = irVALUE;
            InterCodes code1 = translate_Exp(Exp_1,t1);
            InterCodes code2 = translate_Exp(Exp_2,t2);
            t1->which = irSELF;
            t2->which = irSELF;

            InterCodes code3 = NULL;
            if(place==NULL){
                freeOperand(t1);
                freeOperand(t2);
            }
            if(place!=NULL){
                code3 = new_intercodes();
                code3->code =  new_intercode();
                code3->code->kind = irSTAR;
                code3->code->u.binop.op1=t1;
                code3->code->u.binop.op2=t2;
                code3->code->u.binop.result = cpyOperand(place);
                code3->code->u.binop.result->which = irSELF;
            }
            code2 = concatenate(code2,code3);

            return concatenate(code1,code2);

        }
        else if(!strcmp("DIV",Exp_1->siblings->node_name)){
            Node Exp_2 = Exp_1->siblings->siblings;

            Operand t1 = new_tempvar();
            Operand t2 = new_tempvar();

            t1->which = irVALUE;
            t2->which = irVALUE;
            InterCodes code1 = translate_Exp(Exp_1,t1);
            InterCodes code2 = translate_Exp(Exp_2,t2);
            t1->which = irSELF;
            t2->which = irSELF;

            InterCodes code3 = NULL;
            if(place==NULL){
                freeOperand(t1);
                freeOperand(t2);
            }
            if(place!=NULL){
                code3 = new_intercodes();
                code3->code =  new_intercode();
                code3->code->kind = irDIV;
                code3->code->u.binop.op1=t1;
                code3->code->u.binop.op2=t2;
                code3->code->u.binop.result = cpyOperand(place);
                code3->code->u.binop.result->which = irSELF;
            }
            code2 = concatenate(code2,code3);

            return concatenate(code1,code2);
        }
        else if(!strcmp("DOT",Exp_1->siblings->node_name)){
            if(place==NULL)
                return NULL;
            Node ID = Exp_1->siblings->siblings;
            Operand t1 = new_tempvar();
            InterCodes code1 = translate_Exp(Exp_1,t1);
            Type type = TypeOfExp(Exp_1);
            int offset = offsetofType(type,ID->node_value);
            
            
            InterCodes code2 = new_intercodes();
            code2->code =  new_intercode();
            code2->code->kind = irPLUS;
            code2->code->u.binop.op1 = t1;
            code2->code->u.binop.op2 = new_operand();
            code2->code->u.binop.op2->kind = irCONSTANT;
            code2->code->u.binop.op2->which = irSELF;
            code2->code->u.binop.op2->u.value = offset;
        
            if(place->which == irSELF){
                code2->code->u.binop.result = cpyOperand(place);
            }
            else{
                Operand t2 = new_tempvar();
                code2->code->u.binop.result = cpyOperand(t2);

                InterCodes code3 = new_intercodes();
                code3->code =  new_intercode();
                code3->code->kind = irASSIGN;
                t2->which = irVALUE;
                code3->code->u.assign.right = t2;
                code3->code->u.assign.left = cpyOperand(place);
                code3->code->u.assign.left->which = irSELF;

                code2 = concatenate(code2,code3);
            }
            return concatenate(code1,code2);

        }
        else if(!strcmp("LB",Exp_1->siblings->node_name)){
            Node Exp_2 = Exp_1->siblings->siblings;
            Type type = TypeOfExp(Exp_1);
            if(type->u.array.elem->kind==BASIC){
                if(place==NULL)
                    return NULL;

                Operand t0 = new_tempvar();
                Operand t1 = new_tempvar();
                Operand t2 = new_tempvar();
                Operand t3 = new_tempvar();

                InterCodes code0 = translate_Exp(Exp_1,t0);

                t1->which = irVALUE;
                InterCodes code1 = translate_Exp(Exp_2,t1);
                t1->which = irSELF;


                InterCodes code2 = new_intercodes();
                code2->code = new_intercode();
                code2->code->kind = irSTAR;
                code2->code->u.binop.op1 = new_operand();
                code2->code->u.binop.op1->kind = irCONSTANT;
                code2->code->u.binop.op1->which = irSELF;

                code2->code->u.binop.op1->u.value = sizeofType(type->u.array.elem);

                code2->code->u.binop.op2 = t1;
                code2->code->u.binop.result = cpyOperand(t2);

                InterCodes code3 = new_intercodes();
                code3->code = new_intercode();
                code3->code->kind = irPLUS;
                code3->code->u.binop.op1 = t0;
                code3->code->u.binop.op2 = t2;
                code3->code->u.binop.result = cpyOperand(t3);
                InterCodes code4 = new_intercodes();
                code4->code = new_intercode();
                code4->code->kind = irASSIGN;
                code4->code->u.assign.left = cpyOperand(place);
                code4->code->u.assign.left->which = irSELF;
                code4->code->u.assign.right = t3;
                t3->which = place->which;

                code3 = concatenate(code3,code4);
                code2 = concatenate(code2,code3);
                code1 = concatenate(code1,code2);

                return concatenate(code0,code1);
            }
            else
            {
                FLAG = 0;
                return NULL;
            }
            
        }
        else;
    }
    else if(!strcmp("LP",Exp->children->node_name)){
        /*
        Operand t1 = new_tempvar();
        InterCodes code1 = translate_Exp(Exp->children->siblings,t1);
        InterCodes code2 = NULL;
        if(place!=NULL){
            code2 = new_intercodes();
            code2->code = new_intercode();
            code2->code->kind = irASSIGN;
            code2->code->u.assign.left = cpyOperand(place);
            code2->code->u.assign.right = t1;
            if(TypeOfExp(Exp->children->siblings)->kind == BASIC)
                t1->which = irSELF;
            else
                t1->which = place->which;
            code2->code->u.assign.left->which = irSELF;
        }
        return concatenate(code1,code2);
        */
        return translate_Exp(Exp->children->siblings,place);
    }
    else if(!strcmp("NOT",Exp->children->node_name)){
        int label1 = new_label();
        int label2 = new_label();
        InterCodes code0 = NULL;
        if(place!=NULL){
            code0 = new_intercodes();
            code0->code = new_intercode();
            code0->code->kind = irASSIGN;
            code0->code->u.assign.left = cpyOperand(place);
            code0->code->u.assign.left->which = irSELF;
            code0->code->u.assign.right =new_operand();
            code0->code->u.assign.right->which = irSELF;
            code0->code->u.assign.right->kind = irCONSTANT;
            code0->code->u.assign.right->u.value = 0;  
        }
        InterCodes code1 = translate_Cond(Exp,label1,label2);

        InterCodes code2 = new_intercodes();
        code2->code = new_intercode();
        code2->code->kind = irLABEL;
        code2->code->u.label_no = label1;

        InterCodes temp = NULL;
        if(place!=NULL){
            temp = new_intercodes();
            temp->code = new_intercode();
            temp->code->kind = irASSIGN;
            temp->code->u.assign.left = cpyOperand(place);
            temp->code->u.assign.left->which = irSELF;
            temp->code->u.assign.right = new_operand();
            temp->code->u.assign.right->kind = irCONSTANT;
            temp->code->u.assign.right->which = irSELF;
            temp->code->u.assign.right->u.value = 1;  
        }   

        code2 = concatenate(code2,temp);

        temp = new_intercodes();
        temp->code = new_intercode();
        temp->code->kind = irLABEL;
        temp->code->u.label_no = label2;

        code2 = concatenate(code2,temp);
        code1 = concatenate(code1,code2);
        return concatenate(code0,code1); 
    }
    else if(!strcmp("MINUS",Exp->children->node_name)){
        Operand t1 = new_tempvar();

        t1->which = irVALUE;
        InterCodes code1 = translate_Exp(Exp->children->siblings,t1);
        t1->which = irSELF;

        InterCodes code2 = NULL;
        if(place==NULL){
            freeOperand(t1);
        }
        else{
            code2 = new_intercodes();
            code2->code = new_intercode();
            code2->code->kind=irMINUS;
            code2->code->u.binop.result = cpyOperand(place);
            code2->code->u.binop.result->which = irSELF;
            code2->code->u.binop.op1 =new_operand();
            code2->code->u.binop.op1->kind = irCONSTANT;
            code2->code->u.binop.op1->which = irSELF;
            code2->code->u.binop.op1->u.value = 0;
            code2->code->u.binop.op2 = t1;
        }

        return concatenate(code1,code2);

    }
    else
    {
        return NULL;
    }
    
}
static InterCodes translate_Stmt(Node Stmt){
    if(!strcmp("Exp",Stmt->children->node_name))
        return translate_Exp(Stmt->children,NULL);
    else if(!strcmp("CompSt",Stmt->children->node_name))
        return translate_CompSt(Stmt->children);
    else if(!strcmp("RETURN",Stmt->children->node_name)){
        Operand t1 = new_tempvar();

        t1->which = irVALUE;
        InterCodes code1 = translate_Exp(Stmt->children->siblings,t1);
        t1->which = irSELF;

        InterCodes code2 = new_intercodes();
        code2->code = new_intercode();
        code2->code->kind = irRETURN;
        code2->code->u.ret = t1;

        return concatenate(code1,code2);
    }
    else if(!strcmp("IF",Stmt->children->node_name)){
        Node Exp = Stmt->children->siblings->siblings;
        Node Stmt1 = Exp->siblings->siblings;
        int label1 = new_label();
        int label2 = new_label();
        InterCodes code1 = translate_Cond(Exp,label1,label2);
        InterCodes code2 = translate_Stmt(Stmt1);
        if(Stmt1->siblings==NULL){
            InterCodes temp = new_intercodes();
            temp->code = new_intercode();
            temp->code->kind = irLABEL;
            temp->code->u.label_no = label2;
            code2 = concatenate(code2,temp);

            temp = new_intercodes();
            temp->code =new_intercode();
            temp->code->kind = irLABEL;
            temp->code->u.label_no = label1;
            temp = concatenate(temp,code2);

            return concatenate(code1,temp);
        }
        else{
            int label3 = new_label();
            InterCodes code3 = translate_Stmt(Stmt1->siblings->siblings);
            InterCodes temp1,temp2;
        
            temp2 = new_intercodes();
            temp2->code = new_intercode();
            temp2->code->kind = irLABEL;
            temp2->code->u.label_no = label3;
            code3 = concatenate(code3,temp2);

            temp1 = new_intercodes();
            temp1->code = new_intercode();
            temp1->code->kind = irGOTO;
            temp1->code->u.label_no = label3;

            temp2 = new_intercodes();
            temp2->code = new_intercode();
            temp2->code->kind = irLABEL;
            temp2->code->u.label_no = label2;

            temp2 = concatenate(temp2,code3);

            temp1 = concatenate(temp1,temp2);

            code2 = concatenate(code2,temp1);

            temp1 = new_intercodes();
            temp1->code = new_intercode();
            temp1->code->kind = irLABEL;
            temp1->code->u.label_no = label1;

            temp1 = concatenate(temp1,code2);

            return concatenate(code1,temp1);
        }

    }
    else if(!strcmp("WHILE",Stmt->children->node_name)){
        int label1 = new_label();
        int label2 = new_label();
        int label3 = new_label();
        InterCodes code1 = translate_Cond(Stmt->children->siblings->siblings,label2,label3);
        InterCodes code2 = translate_Stmt(Stmt->children->siblings->siblings->siblings->siblings);
        InterCodes temp1,temp2;
        temp1 = new_intercodes();
        temp2 = new_intercodes();
        temp1->code = new_intercode();
        temp2->code = new_intercode();
        temp2->code->kind = irLABEL;
        temp2->code->u.label_no = label3;
        temp1->code->kind = irGOTO;
        temp1->code->u.label_no = label1;
        temp1 = concatenate(temp1,temp2);

        code2 = concatenate(code2,temp1);

        temp1 = new_intercodes();
        temp2 = new_intercodes();
        temp1->code = new_intercode();
        temp2->code = new_intercode();
        temp2->code->kind = irLABEL;
        temp2->code->u.label_no = label2;
        temp1->code->kind = irLABEL;
        temp1->code->u.label_no = label1;

        temp2 = concatenate(temp2,code2);

        code1 = concatenate(code1,temp2);

        return concatenate(temp1,code1);
        
    }
    else;
}
static InterCodes translate_StmtList(Node StmtList){
    if(StmtList==NULL)
        return NULL;
    if(StmtList->children==NULL)
        return NULL;
    InterCodes code1 = translate_Stmt(StmtList->children);
    InterCodes code2 = translate_StmtList(StmtList->children->siblings);
    return concatenate(code1,code2);
}
static InterCodes translate_Cond(Node Exp,int label_true, int label_false){
    if(!strcmp("NOT",Exp->children->node_name)){
        Node Exp_1 = Exp->children->siblings;
        return translate_Cond(Exp_1,label_false,label_true);
    }
    else if(!strcmp("Exp",Exp->children->node_name)){
        Node Exp_1 = Exp->children;
        if(!strcmp("AND",Exp_1->siblings->node_name)){
            Node Exp_2 = Exp_1->siblings->siblings;
            int label1 = new_label();
            InterCodes code1 = translate_Cond(Exp_1,label1,label_false);
            InterCodes code2 = translate_Cond(Exp_2,label_true,label_false);
            InterCodes temp = new_intercodes();
            temp->code = new_intercode();
            temp->code->kind = irLABEL;
            temp->code->u.label_no = label1;
            temp = concatenate(temp,code2);
            return concatenate(code1,temp);
        }
        else if(!strcmp("OR",Exp_1->siblings->node_name)){
            Node Exp_2 = Exp_1->siblings->siblings;
            int label1 = new_label();
            InterCodes code1 = translate_Cond(Exp_1,label_true,label1);
            InterCodes code2 = translate_Cond(Exp_2,label_true,label_false);
            InterCodes temp = new_intercodes();
            temp->code = new_intercode();
            temp->code->kind = irLABEL;
            temp->code->u.label_no = label1;
            temp = concatenate(temp,code2);
            return concatenate(code1,temp);
        }
        else if(!strcmp("RELOP",Exp_1->siblings->node_name)){
            Operand t1 = new_tempvar();

            Operand t2 = new_tempvar();

            Node Exp_2 = Exp_1->siblings->siblings;
            t1->which = irVALUE;
            t2->which = irVALUE;
            InterCodes code1 = translate_Exp(Exp_1,t1);
            InterCodes code2 = translate_Exp(Exp_2,t2);
            t1->which = irSELF;
            t2->which = irSELF;

            InterCodes code3 = new_intercodes();
            code3->code = new_intercode();
            code3->code->kind = irCOND;
            code3->code->u.condition.x = t1;
            code3->code->u.condition.y = t2;
            code3->code->u.condition.label_no = label_true;
            strcpy(code3->code->u.condition.relop,Exp_1->siblings->node_value);

            InterCodes temp = new_intercodes();
            temp->code = new_intercode();
            temp->code->kind =irGOTO;
            temp->code->u.label_no = label_false;

            code3 = concatenate(code3,temp);
            code2 = concatenate(code2,code3);
            return concatenate(code1,code2);
        }
        else
        {
            Operand t1 = new_tempvar();

            t1->which = irVALUE;
            InterCodes code1 = translate_Exp(Exp,t1);
            t1->which = irSELF;

            InterCodes code2 = new_intercodes();
            code2->code = new_intercode();
            code2->code->kind = irCOND;
            code2->code->u.condition.x = t1;
            code2->code->u.condition.label_no = label_true;
            strcpy(code2->code->u.condition.relop,"!=");
            code2->code->u.condition.y = new_operand();
            code2->code->u.condition.y->kind = irCONSTANT;
            code2->code->u.condition.y->which = irSELF;
            code2->code->u.condition.y->u.value = 0;

            InterCodes temp = new_intercodes();
            temp->code = new_intercode();
            temp->code->kind =irGOTO;
            temp->code->u.label_no = label_false;

            code2 = concatenate(code2,temp);
            return concatenate(code1,code2);
        }
        
    }
    else{
        Operand t1 = new_tempvar();

        t1->which = irVALUE;
        InterCodes code1 = translate_Exp(Exp,t1);
        t1->which = irSELF;

        InterCodes code2 = new_intercodes();
        code2->code = new_intercode();
        code2->code->kind = irCOND;
        code2->code->u.condition.x = t1;
        code2->code->u.condition.label_no = label_true;
        strcpy(code2->code->u.condition.relop,"!=");
        code2->code->u.condition.y = new_operand();
        code2->code->u.condition.y->kind = irCONSTANT;
        code2->code->u.condition.y->which = irSELF;
        code2->code->u.condition.y->u.value = 0;

        InterCodes temp = new_intercodes();
        temp->code = new_intercode();
        temp->code->kind =irGOTO;
        temp->code->u.label_no = label_false;

        code2 = concatenate(code2,temp);
        return concatenate(code1,code2);
    }
}

static InterCodes translate_Args(Node Args,ArgsList* args_list){
    Node Exp = Args->children;
    Operand t1 =new_tempvar();

    Type type = TypeOfExp(Exp);
    if(type->kind == BASIC){
        t1->which = irVALUE;
    }
        
    InterCodes code1 = translate_Exp(Exp,t1);
    t1->which = irSELF;

    ArgsList temp = (ArgsList)malloc(sizeof(struct ArgsList_));
    
    temp->x = t1;
    temp->next = args_list[0];
    args_list[0] = temp;
    


    InterCodes code2 = NULL;

    if(Exp->siblings!=NULL){
        code2 = translate_Args(Exp->siblings->siblings,args_list);
    }

    return concatenate(code1,code2);

}

static void printOp(Operand op){
    if(op==NULL)
        return;

    switch (op->which)
    {
    case irADDRESS:
        printf("&");
        break;
    case irVALUE:
        printf("*");
        break;
    default:
        break;
    }
    switch (op->kind)
    {
    case irVARIABLE:
        printf("%s",op->u.id);
        break;

    case irCONSTANT:
        printf("#%d",op->u.value);
        break;

    case irTempVAR:
        printf("t%d",op->u.value);
        break;
    
    case irAllocVAR:
        printf("alloc%d",op->u.value);
        break;

    default:
        break;
    }
}

static void printIR(InterCodes intercodes){
    for(InterCodes current = intercodes;current!=NULL;current=current->next){
        switch (current->code->kind)
        {
        case irLABEL:
            printf("LABEL label%d :\n",current->code->u.label_no);
            break;
        
        case irFUNCTION:
            printf("FUNCTION %s :\n",current->code->u.id);
            break;
            
        case irASSIGN:
            printOp(current->code->u.assign.left);
            printf(" := ");
            printOp(current->code->u.assign.right);
            printf("\n");
            break;

        case irPLUS:
            printOp(current->code->u.binop.result);
            printf(" := ");
            printOp(current->code->u.binop.op1);
            printf(" + ");
            printOp(current->code->u.binop.op2);
            printf("\n");
            break;
        
        case irMINUS:
            printOp(current->code->u.binop.result);
            printf(" := ");
            printOp(current->code->u.binop.op1);
            printf(" - ");
            printOp(current->code->u.binop.op2);
            printf("\n");
            break;
        
        case irSTAR:
            printOp(current->code->u.binop.result);
            printf(" := ");
            printOp(current->code->u.binop.op1);
            printf(" * ");
            printOp(current->code->u.binop.op2);
            printf("\n");
            break;
        
        case irDIV:
            printOp(current->code->u.binop.result);
            printf(" := ");
            printOp(current->code->u.binop.op1);
            printf(" / ");
            printOp(current->code->u.binop.op2);
            printf("\n");
            break;

        case irGOTO:
            printf("GOTO label%d\n",current->code->u.label_no);
            break;

        case irCOND:
            printf("IF ");
            printOp(current->code->u.condition.x);
            printf(" %s ",current->code->u.condition.relop);
            printOp(current->code->u.condition.y);
            printf(" GOTO label%d\n",current->code->u.condition.label_no);
            break;

        case irRETURN:
            printf("RETURN ");
            printOp(current->code->u.ret);
            printf("\n");
            break;

        case irDEC:
            printf("DEC ");
            printOp(current->code->u.dec.alloc);
            printf(" %d\n",current->code->u.dec.size);
            break;

        case irARG:
            printf("ARG ");
            printOp(current->code->u.arg);
            printf("\n");
            break;

        case irCALL:
            printOp(current->code->u.call.x);
            printf(" := CALL %s\n",current->code->u.call.func);
            break;
        
        case irPARAM:
            printf("PARAM ");
            printOp(current->code->u.param);
            printf("\n");
            break;

        case irWRITE:
            printf("WRITE ");
            printOp(current->code->u.write);
            printf("\n");
            break;

        case irREAD:
            printf("READ ");
            printOp(current->code->u.read);
            printf("\n");
            break;

        default:
            break;
        }
    }
}

static void freeInterCode(InterCode intercode){
    if(intercode==NULL)
        return;
    switch (intercode->kind)
    {
    case irASSIGN:
        freeOperand(intercode->u.assign.left);
        intercode->u.assign.left=NULL;
        freeOperand(intercode->u.assign.right);
        intercode->u.assign.right=NULL;
        break;
    case irFUNCTION:
        break;
    case irPLUS:
    case irMINUS:
    case irSTAR:
    case irDIV:
        freeOperand(intercode->u.binop.result);
        freeOperand(intercode->u.binop.op1);
        freeOperand(intercode->u.binop.op2);
        break;
    
    case irCOND:
        freeOperand(intercode->u.condition.x);
        freeOperand(intercode->u.condition.y);
        break;
    
    case irRETURN:
        freeOperand(intercode->u.ret);
        break;

    case irDEC:
        freeOperand(intercode->u.dec.alloc);
        break;

    case irARG:
        freeOperand(intercode->u.arg);
        break;

    case irCALL:
        freeOperand(intercode->u.call.x);
        break;
    
    case irPARAM:
        freeOperand(intercode->u.param);
        break;
        
    case irREAD:
        freeOperand(intercode->u.read);
        break;
    
    case irWRITE:
        freeOperand(intercode->u.write);
        break;
    
    default:
        break;
    }
    free(intercode);
}