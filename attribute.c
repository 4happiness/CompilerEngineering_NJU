#include "semantic.h"
#include "multiway_tree.h"
#include "common.h"

static Type TYPE2Type(Node);
static Type StructSpecifier2Type(Node);
static FieldList Def2FieldList(Node);
FieldList DefList2FieldList(Node);
Type Specifier2Type(Node);
char* IDinVarDec(Node);
Type TypeinVarDec(Node, Type);

static Type TYPE2Type(Node node){
#ifdef DEBUGING
    if(node == NULL){
        printf("TYPE2attr(),NULL ptr\n");
        return NULL;
    }  
    if(strcmp(node->node_name,"TYPE")!=0){
        printf("TYPE2attr(),node is not TYPE\n");
        return NULL;
    }       
#endif
    Type ret = (Type)malloc(sizeof(struct Type_));
    ret->kind = BASIC;
    if(!strcmp(node->node_value,"int"))
        ret->u.basic=INT;
    else
        ret->u.basic=FLOAT;
    return ret;
}

static Type StructSpecifier2Type(Node node){
    Type ret = (Type)malloc(sizeof(struct Type_));
    ret->kind = STRUCTURE;
    Node STRUCT = node->children;
    if(STRUCT->siblings->siblings == NULL)
        ret->u.structure=NULL;
    else{
        Node DefList = STRUCT->siblings->siblings->siblings;
        ret->u.structure = DefList2FieldList(DefList);
    }
    return ret;
}

static FieldList Def2FieldList(Node node){
#ifdef DEBUGING
    if(node == NULL){
        printf("Def2FieldList(),NULL ptr\n");
        return NULL;
    }  
    if(strcmp(node->node_name,"Def")!=0){
        printf("Def2FieldList(),node is not Def\n");
        return NULL;
    }       
#endif    
    Node Specifier = node->children;
    Node DecList = Specifier->siblings;
    Node Dec = DecList->children;
    Node VarDec = Dec->children;
    FieldList ret = (FieldList)malloc(sizeof(struct FieldList_));
    strcpy(ret->name,IDinVarDec(VarDec));
    ret->tail=NULL;
    ret->type = TypeinVarDec(VarDec,Specifier2Type(Specifier));
    if(VarDec->siblings!=NULL)
        printf("Error type 15 at Line %d: Assignment in struct-definition.\n",VarDec->row);
    while(Dec->siblings!=NULL){
        DecList = Dec->siblings->siblings;
        Dec = DecList->children;
        VarDec = Dec->children;
        FieldList temp = (FieldList)malloc(sizeof(struct FieldList_));
        strcpy(temp->name,IDinVarDec(VarDec));
        temp->type = TypeinVarDec(VarDec,Specifier2Type(Specifier));
        if(VarDec->siblings!=NULL)
            printf("Error type 15 at Line %d: Assignment in struct-definition.\n",VarDec->row);
        temp->tail = ret;
        ret = temp;
    }
    return ret;
}

Symbol VarDec2Symbol(Node node,Type type){
#ifdef DEBUGING
    if(node == NULL){
        printf("VarDec2Symbol(),NULL ptr\n");
        return NULL;
    }  
    if(strcmp(node->node_name,"VarDec")!=0){
        printf("VarDec2Symbol(),node is not VarDec\n");
        return NULL;
    }       
#endif
    if(strcmp(node->children->node_name,"ID")==0){
        Node ID = node->children;
        Symbol ret = (Symbol)malloc(sizeof(struct Symbol_));
        ret->kind=VARIABLE;
        strcpy(ret->name,ID->node_value);
        ret->u.varible=type;
        return ret;
    }
    else{
        Node VarDec = node->children;
        Node INT = VarDec->siblings->siblings;
        Type newType = (Type)malloc(sizeof(struct Type_));
        newType->kind = ARRAY;
        newType->u.array.elem = type;
        newType->u.array.size = atoi(INT->node_value);
        return VarDec2Symbol(VarDec,newType);
    }
}

char* IDinVarDec(Node node){
#ifdef DEBUGING
    if(node == NULL){
        printf("IDinVarDec(),NULL ptr\n");
        return NULL;
    }  
    if(strcmp(node->node_name,"VarDec")!=0){
        printf("IDinVarDec(),node is not VarDec\n");
        return NULL;
    }       
#endif
    Node ID = node->children;
    while(ID->children!=NULL)
        ID = ID->children;
    return ID->node_value;
}

Type TypeinVarDec(Node node, Type type){
#ifdef DEBUGING
    if(node == NULL){
        printf("TypeinVarDec(),NULL ptr\n");
        return NULL;
    }  
    if(strcmp(node->node_name,"VarDec")!=0){
        printf("TypeinVarDec(),node is not VarDec\n");
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
        return TypeinVarDec(VarDec,newType);
    }
}

FieldList DefList2FieldList(Node node){
#ifdef DEBUGING
    if(node == NULL){
        printf("DefList2FieldList(),NULL ptr\n");
        return NULL;
    }  
    if(strcmp(node->node_name,"DefList")!=0){
        printf("DefList2FieldList(),node is not DefList\n");
        return NULL;
    }       
#endif
    if(node->children==NULL)
        return NULL;
    else {
        Node Def = node->children;
        Node DefList = Def->siblings;
        FieldList ret_Def = Def2FieldList(Def);
        FieldList ret_DefList = DefList2FieldList(DefList);
        if(ret_DefList==NULL){
            return ret_Def;
        }
        else{
            FieldList tail = ret_DefList;
            while(tail->tail!=NULL)
                tail = tail->tail;
            tail->tail = ret_Def;
            return ret_DefList;
        }
    }
}



Type Specifier2Type(Node node){
    Node child = node->children;
    if(strcmp(child->node_name,"TYPE")==0)
        return TYPE2Type(child);
    else
        return StructSpecifier2Type(child);
}