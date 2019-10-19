#include "symtab.h"
#include "multiway_tree.h"
#include "common.h"

static Type parseTYPE(const Node);
static FieldList parseDef_toFieldList(const Node);
static Type parseStructSpecifier(const Node);
static FieldList parseDefList_toFieldList(const Node);
static char* parseVarDec_toID(const Node);
static Type parseVarDec_toType(const Node, const Type);

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
    if(strcmp(node->node_name,"Specifier")!=0){
        printf("parseStructSpecifier(),node is not StructSpecifier\n");
        return NULL;
    }       
#endif
    if(node->children->siblings->siblings == NULL){
        Node Tag = node->children->siblings;
        Type type = NULL;
        type = typecpy(getType(Tag->children->node_value,STRUCTNAME));
        if(type == NULL){
            printf("Error type 17 at Line %d: Undefined structure \"%s\".\n",Tag->row,Tag->children->node_value);
        }
        return type;
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
                symbol->u.type = typecpy(type);
                addSymbol(symbol);
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
    if(VarDec->siblings!=NULL)
        printf("Error type 15 at Line %d: Assignment in struct-definition.\n",VarDec->row);
    while(Dec->siblings!=NULL){
        DecList = Dec->siblings->siblings;
        Dec = DecList->children;
        VarDec = Dec->children;
        FieldList temp = (FieldList)malloc(sizeof(struct FieldList_));
        strcpy(temp->name,parseVarDec_toID(VarDec));
        temp->type = parseVarDec_toType(VarDec,typecpy(type));
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

static char* parseVarDec_toID(const Node node){
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

static Type parseVarDec_toType(const Node node, const Type type){
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