#include "symtab.h"
#include "parseProduction.h"
#include "multiway_tree.h"
#include "common.h"

static Type parseTYPE(const Node);
static FieldList parseDef_toFieldList(const Node);
static Type parseStructSpecifier(const Node);
static FieldList parseDefList_toFieldList(const Node);
static int checkLvalue(Node);
static void printArgs(Node);
static void printExp(Node);
static void printFuncParams(Function);


static Type parseTYPE(const Node node){
#ifdef DEBUGING
    if(node == NULL){
        printf("parseTYPE(),NULL ptr\n");
        return NULL;
    }  
    if(strcmp(node->node_name,"TYPE")!=0){
        printf("parseTYPE(),node is not TYPE\n");
        return NULL;
    }       
#endif
    Type type = (Type)malloc(sizeof(struct Type_));
    type->kind = BASIC;
    if(!strcmp(node->node_value,"int"))
        type->u.basic=INT;
    else
        type->u.basic=FLOAT;
    return type;
}

static Type parseStructSpecifier(const Node node){
#ifdef DEBUGING
    if(node == NULL){
        printf("parseStructSpecifier(),NULL ptr\n");
        return NULL;
    }  
    if(strcmp(node->node_name,"StructSpecifier")!=0){
        printf("parseStructSpecifier(),node is not StructSpecifier\n");
        return NULL;
    }       
#endif
    if(node->children->siblings->siblings == NULL){
        Node Tag = node->children->siblings;
        Type type = getType(Tag->children->node_value,STRUCTNAME);
        if(type == NULL){
            printf("Error type 17 at Line %d: Undefined structure \"%s\".\n",Tag->row,Tag->children->node_value);
            return NULL;
        }
        else{
            Type ret = (Type)malloc(sizeof(struct Type_));
            typecpy(ret,type);
            return ret;
        }
    }
    else{
        Node OptTag = node->children->siblings;
        Node DefList = OptTag->siblings->siblings;
        Type type = (Type)malloc(sizeof(struct Type_));
        type->kind = STRUCTURE;
        type->u.structure = parseDefList_toFieldList(DefList);
        if(OptTag->children != NULL){
            Node ID = OptTag->children;
            if(UNOCCUPIED == checkDefinition(ID->node_value,STRUCTNAME)){
                Symbol symbol = (Symbol)malloc(sizeof(struct Symbol_));
                symbol->kind = STRUCTNAME;
                strcpy(symbol->name,ID->node_value);
                symbol->u.type = (Type)malloc(sizeof(struct Type_));
                typecpy(symbol->u.type,type);
                addSymbol(symbol);
            }
            else{
                printf("Error type 16 at Line %d: Duplicated name \"%s\".\n",ID->row,ID->node_value);
            }
        }
        return type;
    }
}

Type parseSpecifier(const Node node){
#ifdef DEBUGING
    if(node == NULL){
        printf("parseSpecifier(),NULL ptr\n");
        return NULL;
    }  
    if(strcmp(node->node_name,"Specifier")!=0){
        printf("parseSpecifier(),node is not Specifier\n");
        return NULL;
    }       
#endif
    if(!strcmp(node->children->node_name,"TYPE"))
        return parseTYPE(node->children);
    else
        return parseStructSpecifier(node->children);
}

static FieldList parseDef_toFieldList(const Node node){
#ifdef DEBUGING
    if(node == NULL){
        printf("parseDef_toFieldList(),NULL ptr\n");
        return NULL;
    }  
    if(strcmp(node->node_name,"Def")!=0){
        printf("parseDef_toFieldList(),node is not Def\n");
        return NULL;
    }       
#endif
    Node Specifier = node->children;
    Node DecList = Specifier->siblings;
    Node Dec = DecList->children;
    Node VarDec = Dec->children;
    FieldList ret = (FieldList)malloc(sizeof(struct FieldList_));
    
    strcpy(ret->name,parseVarDec_toID(VarDec));
    ret->tail=NULL;
    Type type = parseSpecifier(Specifier);
    ret->type = parseVarDec_toType(VarDec,type);

    Symbol symbol = (Symbol)malloc(sizeof(struct Symbol_));
    symbol->kind = VARIABLE;
    strcpy(symbol->name,ret->name);

    symbol->u.type = (Type)malloc(sizeof(struct Type_));
    typecpy(symbol->u.type,ret->type);
    if(UNOCCUPIED == checkDefinition(symbol->name,VARIABLE))
        addSymbol(symbol);
    else{
        printf("Error type 15 at Line %d: Redefined field \"%s\".\n",VarDec->row,symbol->name);
        freeSymbol(symbol);
    }
    if(VarDec->siblings!=NULL)
        printf("Error type 15 at Line %d: Assignment in struct-definition.\n",VarDec->row);
    
    while(Dec->siblings!=NULL){
        DecList = Dec->siblings->siblings;
        Dec = DecList->children;
        VarDec = Dec->children;
        FieldList temp = (FieldList)malloc(sizeof(struct FieldList_));
        strcpy(temp->name,parseVarDec_toID(VarDec));
        Type type_2 = (Type)malloc(sizeof(struct Type_));
        typecpy(type_2,type);
        temp->type = parseVarDec_toType(VarDec,type_2);

        symbol = (Symbol)malloc(sizeof(struct Symbol_));
        symbol->kind = VARIABLE;
        strcpy(symbol->name,temp->name);
        symbol ->u.type = (Type)malloc(sizeof(struct Type_));
        typecpy(symbol->u.type,temp->type);
        if(UNOCCUPIED == checkDefinition(symbol->name,VARIABLE))
            addSymbol(symbol);
        else{
            printf("Error type 15 at Line %d: Redefined field \"%s\".\n",VarDec->row,symbol->name);
            freeSymbol(symbol);
        }
        if(VarDec->siblings!=NULL)
            printf("Error type 15 at Line %d: Assignment in struct-definition.\n",VarDec->row);
        
        temp->tail = ret;
        ret = temp;
    }
    return ret;
}

static FieldList parseDefList_toFieldList(const Node node){
#ifdef DEBUGING
    if(node == NULL){
        printf("parseDefList_toFieldList(),NULL ptr\n");
        return NULL;
    }  
    if(strcmp(node->node_name,"DefList")!=0){
        printf("parseDefList_toFieldList(),node is not DefList\n");
        return NULL;
    }       
#endif
    if(node->children==NULL)
        return NULL;
    else {
        Node Def = node->children;
        Node DefList = Def->siblings;
        FieldList ret_Def = parseDef_toFieldList(Def);
        FieldList ret_DefList = parseDefList_toFieldList(DefList);
        if(ret_DefList==NULL)
            return ret_Def;
        else{
            FieldList tail = ret_DefList;
            while(tail->tail!=NULL)
                tail = tail->tail;
            tail->tail = ret_Def;
            return ret_DefList;
        }
    }
}

char* parseVarDec_toID(const Node node){
#ifdef DEBUGING
    if(node == NULL){
        printf("parseVarDec_toID(),NULL ptr\n");
        return NULL;
    }  
    if(strcmp(node->node_name,"VarDec")!=0){
        printf("parseVarDec_toID(),node is not VarDec\n");
        return NULL;
    }       
#endif
    Node ID = node->children;
    while(ID->children!=NULL)
        ID = ID->children;
    return ID->node_value;
}

Type parseVarDec_toType(const Node node, const Type type){
#ifdef DEBUGING
    if(node == NULL){
        printf("parseVarDec_toType(),NULL ptr\n");
        return NULL;
    }  
    if(strcmp(node->node_name,"VarDec")!=0){
        printf("parseVarDec_toType(),node is not VarDec\n");
        return NULL;
    }       
#endif
    if(strcmp(node->children->node_name,"ID")==0){
        return type;
    }
    else{
        Node VarDec = node->children;
        Node INT = VarDec->siblings->siblings;
        Type newType = (Type)malloc(sizeof(struct Type_));
        newType->kind = ARRAY;
        newType->u.array.elem = type;
        newType->u.array.size = atoi(INT->node_value);
        return parseVarDec_toType(VarDec,newType);
    }
}

Symbol parseVarDec_toSymbol(const Node node,const Type type){
#ifdef DEBUGING
    if(node == NULL){
        printf("parseVarDec_toSymbol(),NULL ptr\n");
        return NULL;
    }  
    if(strcmp(node->node_name,"VarDec")!=0){
        printf("parseVarDec_toSymbol(),node is not VarDec\n");
        return NULL;
    }       
#endif
    if(strcmp(node->children->node_name,"ID")==0){
        Node ID = node->children;
        Symbol ret = (Symbol)malloc(sizeof(struct Symbol_));
        ret->kind=VARIABLE;
        strcpy(ret->name,ID->node_value);
        ret->u.type=type;
        return ret;
    }
    else{
        Node VarDec = node->children;
        Node INT = VarDec->siblings->siblings;
        Type newType = (Type)malloc(sizeof(struct Type_));
        newType->kind = ARRAY;
        newType->u.array.elem = type;
        newType->u.array.size = atoi(INT->node_value);
        return parseVarDec_toSymbol(VarDec,newType);
    }
}

int parseVarList_toNum(Node node){
#ifdef DEBUGING
    if(node == NULL){
        printf("parseVarList_toNum(),NULL ptr\n");
        return 0;
    }  
    if(strcmp(node->node_name,"VarList")!=0){
        printf("parseVarList_toNum(),node is not VarList\n");
        return 0;
    }       
#endif
    Node ParamDec = node->children;
    if(ParamDec->siblings == NULL)
        return 1;
    else{
        Node VarList = ParamDec->siblings->siblings;
        return 1 + parseVarList_toNum(VarList);
    }
}
int parseArgs_toNum(Node node){
#ifdef DEBUGING
    if(node == NULL){
        printf("parsArgs_toNum(),NULL ptr\n");
        return 0;
    }  
    if(strcmp(node->node_name,"Args")!=0){
        printf("parseArgs_toNum(),node is not Args\n");
        return 0;
    }       
#endif
    Node Exp = node->children;
    if(Exp->siblings == NULL)
        return 1;
    else{
        Node Args = Exp->siblings->siblings;
        return 1 + parseArgs_toNum(Args);
    }
}

Symbol parseParamDec_toSymbol(Node node){
#ifdef DEBUGING
    if(node == NULL){
        printf("parseParamDec_toSymbol(),NULL ptr\n");
        return NULL;
    }  
    if(strcmp(node->node_name,"ParamDec")!=0){
        printf("parseParamDec_toSymbol(),node is not ParamDec\n");
        return NULL;
    }       
#endif
    Node Specifier = node->children;
    Node VarDec = Specifier->siblings;
    return parseVarDec_toSymbol(VarDec,parseSpecifier(Specifier));
}

Symbol* parseVarList_toSymbols(Node node){
#ifdef DEBUGING
    if(node == NULL){
        printf("parseVarList_toSymbols(),NULL ptr\n");
        return NULL;
    }  
    if(strcmp(node->node_name,"VarList")!=0){
        printf("parseVarList_toSymbols(),node is not VarList\n");
        return NULL;
    }       
#endif
    int num = parseVarList_toNum(node);
    Node VarList = node;
    Node ParamDec = NULL;
    Symbol* symbols = (Symbol*)malloc(num*sizeof(Symbol));
    for(int i=0;i<num;i++){
        ParamDec = VarList->children;
        if(ParamDec->siblings!=NULL)
            VarList = ParamDec->siblings->siblings;
        symbols[i] = parseParamDec_toSymbol(ParamDec);
    }
    return symbols;
}

Type TypeOfExp(Node node){
#ifdef DEBUGING
    if(node == NULL){
        printf("TypeOfExp(),NULL ptr\n");
        return NULL;
    }  
    if(strcmp(node->node_name,"Exp")!=0){
        printf("TypeOfExp(),node is not Exp\n");
        return NULL;
    }       
#endif
    if(!strcmp(node->children->node_name,"Exp")){
        if(!strcmp(node->children->siblings->node_name,"ASSIGNOP")){
            Type type_1 = TypeOfExp(node->children);
            Type type_2 = TypeOfExp(node->children->siblings->siblings);

            if(FAILURE == checkLvalue(node->children)){
                printf("Error type 6 at Line %d: The left-hand side of an assignment must be a variable.\n",node->children->siblings->row);
                return type_2;
            }
            
            if(type_1 == NULL)
                return type_2;
            if(type_2 == NULL)
                return type_1;

            if(SUCCESS == typecmp(type_1,type_2))
                return type_1;
            else{
                printf("Error type 5 at Line %d: Type mismatched for assignment.\n",node->children->siblings->row);
                return NULL;
            }
        }
        else if(!strcmp(node->children->siblings->node_name,"AND")){
            Type type_1 = TypeOfExp(node->children);
            Type type_2 = TypeOfExp(node->children->siblings->siblings);
            if(type_1==NULL || type_2==NULL){
                printf("Error type 7 at Line %d: Type mismatched fot operands.\n",node->children->siblings->row);
                return NULL;
            }
            if(type_1->kind!=BASIC || type_2->kind!=BASIC){
                printf("Error type 7 at Line %d: Type mismatched fot operands.\n",node->children->siblings->row);
                return NULL;
            }
            if(type_1->u.basic!=INT || type_2->u.basic!=INT){
                printf("Error type 7 at Line %d: Type mismatched fot operands.\n",node->children->siblings->row);
                return NULL;
            }
            return type_1;
        }
        else if(!strcmp(node->children->siblings->node_name,"OR")){
            Type type_1 = TypeOfExp(node->children);
            Type type_2 = TypeOfExp(node->children->siblings->siblings);
            if(type_1==NULL || type_2==NULL){
                printf("Error type 7 at Line %d: Type mismatched fot operands.\n",node->children->siblings->row);
                return NULL;
            }
            if(type_1->kind!=BASIC || type_2->kind!=BASIC){
                printf("Error type 7 at Line %d: Type mismatched fot operands.\n",node->children->siblings->row);
                return NULL;
            }
            if(type_1->u.basic!=INT || type_2->u.basic!=INT){
                printf("Error type 7 at Line %d: Type mismatched fot operands.\n",node->children->siblings->row);
                return NULL;
            }
            return type_1;
        }
        else if(!strcmp(node->children->siblings->node_name,"RELOP")){
            Type type_1 = TypeOfExp(node->children);
            Type type_2 = TypeOfExp(node->children->siblings->siblings);
            if(type_1==NULL || type_2==NULL){
                printf("Error type 7 at Line %d: Type mismatched fot operands.\n",node->children->siblings->row);
                return NULL;
            }
            if(type_1->kind!=BASIC || type_2->kind!=BASIC){
                printf("Error type 7 at Line %d: Type mismatched fot operands.\n",node->children->siblings->row);
                return NULL;
            }
            if(type_1->u.basic!=type_2->u.basic){
                printf("Error type 7 at Line %d: Type mismatched fot operands.\n",node->children->siblings->row);
                return NULL;
            }
            Type type = (Type)malloc(sizeof(struct Type_));
            type->kind = BASIC;
            type->u.basic = INT;
            return type;
        }
        else if(!strcmp(node->children->siblings->node_name,"PLUS")){
            Type type_1 = TypeOfExp(node->children);
            Type type_2 = TypeOfExp(node->children->siblings->siblings);
            if(type_1 == NULL){
                printf("Error type 7 at Line %d: Type mismatched fot operands.\n",node->children->siblings->row);
                return NULL;
            }
            if(type_1->kind!=BASIC){
                printf("Error type 7 at Line %d: Type mismatched fot operands.\n",node->children->siblings->row);
                return NULL;
            }
            if(FAILURE == typecmp(type_1,type_2)){
                printf("Error type 7 at Line %d: Type mismatched fot operands.\n",node->children->siblings->row);
                return NULL;
            }
            return type_1;
        }
        else if(!strcmp(node->children->siblings->node_name,"MINUS")){
            Type type_1 = TypeOfExp(node->children);
            Type type_2 = TypeOfExp(node->children->siblings->siblings);
            if(type_1 == NULL){
                printf("Error type 7 at Line %d: Type mismatched fot operands.\n",node->children->siblings->row);
                return NULL;
            }
            if(type_1->kind!=BASIC){
                printf("Error type 7 at Line %d: Type mismatched fot operands.\n",node->children->siblings->row);
                return NULL;
            }
            if(FAILURE == typecmp(type_1,type_2)){
                printf("Error type 7 at Line %d: Type mismatched fot operands.\n",node->children->siblings->row);
                return NULL;
            }
            return type_1;
        }
        else if(!strcmp(node->children->siblings->node_name,"STAR")){
            Type type_1 = TypeOfExp(node->children);
            Type type_2 = TypeOfExp(node->children->siblings->siblings);
            if(type_1 == NULL){
                printf("Error type 7 at Line %d: Type mismatched fot operands.\n",node->children->siblings->row);
                return NULL;
            }
            if(type_1->kind!=BASIC){
                printf("Error type 7 at Line %d: Type mismatched fot operands.\n",node->children->siblings->row);
                return NULL;
            }
            if(FAILURE == typecmp(type_1,type_2)){
                printf("Error type 7 at Line %d: Type mismatched fot operands.\n",node->children->siblings->row);
                return NULL;
            }
            return type_1;
        }
        else if(!strcmp(node->children->siblings->node_name,"DIV")){
            Type type_1 = TypeOfExp(node->children);
            Type type_2 = TypeOfExp(node->children->siblings->siblings);
            if(type_1 == NULL){
                printf("Error type 7 at Line %d: Type mismatched fot operands.\n",node->children->siblings->row);
                return NULL;
            }
            if(type_1->kind!=BASIC){
                printf("Error type 7 at Line %d: Type mismatched fot operands.\n",node->children->siblings->row);
                return NULL;
            }
            if(FAILURE == typecmp(type_1,type_2)){
                printf("Error type 7 at Line %d: Type mismatched fot operands.\n",node->children->siblings->row);
                return NULL;
            }
            return type_1;
        }
        else if(!strcmp(node->children->siblings->node_name,"LB")){
            Node Exp_1 = node->children;
            Node Exp_2 = Exp_1->siblings->siblings;
            Type type_1 = TypeOfExp(Exp_1);
            Type type_2 = TypeOfExp(Exp_2);
            if(type_1->kind != ARRAY){
                printf("Error type 10 at Line %d: \"",Exp_1->row);
                printExp(Exp_1);
                printf("\" is not an array.\n");
                if(type_2->kind != BASIC || type_2->u.basic!=INT){
                    printf("Error type 12 at Line %d: \"",Exp_2->row);
                    printExp(Exp_2);
                    printf("\" is not an integer.\n");
                }
                return NULL;
            }
            if(type_2->kind == BASIC && type_2->u.basic==INT){
                return type_1->u.array.elem;
            }
            else{
                printf("Error type 12 at Line %d: \"",Exp_2->row);
                printExp(Exp_2);
                printf("\" is not an integer.\n");
                return NULL;
            }
        }
        else if(!strcmp(node->children->siblings->node_name,"DOT")){
            Type type = TypeOfExp(node->children);
            Node ID = node->children->siblings->siblings;
            if(type->kind!=STRUCTURE){
                printf("Error type 13 at Line %d: Illegal use of \".\".\n",ID->row);
                return NULL;
            }
            FieldList fieldlist = type->u.structure;
            while(fieldlist!=NULL){
                if(!strcmp(fieldlist->name,ID->node_value))
                    break;
                fieldlist = fieldlist->tail;
            }
            if(fieldlist==NULL){
                printf("Error type 14 at Line %d: Non-existent field \"%s\".\n",ID->row,ID->node_value);
                return NULL;
            }
            else
                return fieldlist->type;
        }
        else{
            return NULL;
        }
        return NULL;
    }
    else if(!strcmp(node->children->node_name,"LP")){
        return TypeOfExp(node->children->siblings);
    }
    else if(!strcmp(node->children->node_name,"MINUS")){
        Type type = TypeOfExp(node->children->siblings);
        if(type == NULL){
            printf("Error type 7 at Line %d: Type mismatched fot operands.\n",node->children->siblings->row);
            return NULL;
        }
        if(type->kind!=BASIC){
            printf("Error type 7 at Line %d: Type mismatched fot operands.\n",node->children->siblings->row);
            return NULL;
        }
        return type;
    }
    else if(!strcmp(node->children->node_name,"NOT")){
        Type type = TypeOfExp(node->children->siblings);
        if(type == NULL){
            printf("Error type 7 at Line %d: Type mismatched fot operands.\n",node->children->siblings->row);
            return NULL;
        }
        if(type->kind!=BASIC){
            printf("Error type 7 at Line %d: Type mismatched fot operands.\n",node->children->siblings->row);
            return NULL;
        }
        if(type->u.basic!=INT){
            printf("Error type 7 at Line %d: Type mismatched fot operands.\n",node->children->siblings->row);
            return NULL;
        }
        return type;
    }
    else if(!strcmp(node->children->node_name,"ID")){
        Node ID = node->children;
        if(ID->siblings==NULL){
            Type type = getType(ID->node_value,VARIABLE);
            if(type==NULL)
                printf("Error type 1 at Line %d: Undefined variable \"%s\".\n",ID->row,ID->node_value);
            return type;
        }  
        else{
            Node Args = ID->siblings->siblings;
            Symbol symbol = getSymbol(ID->node_value,FUNCTION);
            if(symbol == NULL){
                if(UNOCCUPIED != checkDefinition(ID->node_value,VARIABLE))
                    printf("Error type 11 at Line %d: \"%s\" is not a function.\n",ID->row,ID->node_value);
                else
                    printf("Error type 2 at Line %d: Undefined function \"%s\".\n",ID->row,ID->node_value);
            }
            else{
                int Args_num = 0;
                if(!strcmp(Args->node_name,"Args"))
                    Args_num = parseArgs_toNum(Args);
                if(Args_num!=symbol->u.func->varNum){
                    printf("Error type 9 at Line %d: Function \"%s(",ID->row,ID->node_value);
                    printFuncParams(symbol->u.func);
                    printf(")\" is not applicable for arguments \"(");
                    if(Args_num>0)
                        printArgs(Args);
                    printf(")\".\n");
                }
                else{
                    int flag = SUCCESS;
                    Type* params = symbol->u.func->varTypes;
                    Node Exp =  Args->children;
                    for(int i=0;i<Args_num;i++){
                        if(FAILURE == typecmp(params[i],TypeOfExp(Exp))){
                            flag = FAILURE;
                            break;
                        }
                        if(Exp->siblings!=NULL)
                            Exp = Exp->siblings->siblings;
                    }
                    if(flag==FAILURE){
                        printf("Error type 9 at Line %d: Function \"%s(",ID->row,ID->node_value);
                        printFuncParams(symbol->u.func);
                        printf(")\" is not applicable for arguments \"(");
                        if(Args_num>0)
                            printArgs(Args);
                        printf(")\".\n");
                    }
                }     
            }

            if(symbol==NULL)
                return NULL;
            else
                return symbol->u.func->retType;
        }
    }
    else if(!strcmp(node->children->node_name,"INT")){
        Type type = (Type)malloc(sizeof(struct Type_));
        type->kind = BASIC;
        type->u.basic = INT;
        return type;
    }
    else{
        Type type = (Type)malloc(sizeof(struct Type_));
        type->kind = BASIC;
        type->u.basic = FLOAT;
        return type;
    }
    return NULL;
}

static int checkLvalue(Node node){
#ifdef DEBUGING
    if(node == NULL){
        printf("checkLvalue(),NULL ptr\n");
        return FAILURE;
    }  
    if(strcmp(node->node_name,"Exp")!=0){
        printf("checkLvalue(),node is not Exp\n");
        return FAILURE;
    }       
#endif
    if(!strcmp(node->children->node_name,"ID")){
        if(node->children->siblings == NULL)
            return SUCCESS;
        else
            return FAILURE;
    }
    else if(!strcmp(node->children->node_name,"Exp")){
        Node Exp = node->children;
        if(!strcmp(Exp->siblings->node_name,"LB"))
            return SUCCESS;
        else if(!strcmp(Exp->siblings->node_name,"DOT"))
            return SUCCESS;
        else
            return FAILURE;
    }
        return FAILURE;
}

static void printExp(Node node){
#ifdef DEBUGING
    if(node == NULL){
        printf("printExp(),NULL ptr\n");
    }  
    if(strcmp(node->node_name,"Exp")!=0){
        printf("printExp(),node is not Exp\n");
    }       
#endif
    if(!strcmp(node->children->node_name,"Exp")){
        if(!strcmp(node->children->siblings->node_name,"LB")){
            Node Exp_1 = node->children;
            Node Exp_2 = Exp_1->siblings->siblings;
            printExp(Exp_1);
            printf("[");
            printExp(Exp_2);
            printf("]");

        }
        else if(!strcmp(node->children->siblings->node_name,"DOT")){
            Node Exp = node->children;
            Node ID = Exp->siblings->siblings;
            printExp(Exp);
            printf(".");
            printf("%s",ID->node_value);
        }
        else{
            Node Exp_1 = node->children;
            Node Exp_2 = node->siblings->siblings;
            printExp(Exp_1);
            printf("%s",Exp_1->siblings->node_value);
            printExp(Exp_2);
        }
    }
    else if(!strcmp(node->children->node_name,"LP")){
        Node Exp = node->children->siblings;
        printf("(");
        printExp(Exp);
        printf(")");
    }
    else if(!strcmp(node->children->node_name,"MINUS")){
        Node Exp = node->children->siblings;
        printf("-");
        printExp(Exp);
    }
    else if(!strcmp(node->children->node_name,"NOT")){
        Node Exp = node->children->siblings;
        printf("!");
        printExp(Exp);
    }
    else if(!strcmp(node->children->node_name,"ID")){
        Node ID = node->children;
        printf("%s",ID->node_value);
        if(ID->siblings!=NULL){
            printf("(");
            Node Args = ID->siblings->siblings;
            printArgs(Args);
            printf(")");
        }
    }
    else{
        printf("%s",node->children->node_value);
    }
}

static void printArgs(Node node){
    Node Exp = node->children;
    Type type = TypeOfExp(Exp);
    if(type==NULL)
        printf(" ");
    else{
        switch (type->kind)
        {
        case BASIC:
            if(type->u.basic==INT)
                printf("int");
            else
                printf("float");
            break;

        case ARRAY:
            printf("array");
            break;
        
        case STRUCTURE:
            printf("structure");
            break;

        default:
            break;
        }
    }
    if(Exp->siblings!=NULL){
        printf(",");
        printArgs(Exp->siblings->siblings);
    }
}
static void printFuncParams(Function func){
    int param_num = func->varNum;
    Type* params = func->varTypes;
    for(int i=0;i<param_num;i++){
        switch (params[i]->kind)
        {
        case BASIC:
            if(params[i]->u.basic==INT)
                printf("int");
            else
                printf("float");
            break;

        case ARRAY:
            printf("array");
            break;
        
        case STRUCTURE:
            printf("structure");
            break;

        default:
            break;
        }
    }
}