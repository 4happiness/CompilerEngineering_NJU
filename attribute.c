#include "semantic.h"
#include "multiway_tree.h"
#include "common.h"

static Type TYPE2Type(Node);
static Type StructSpecifier2Type(Node);
static FieldList Def2FieldList(Node);
FieldList DefList2FieldList(Node);
Type Specifier2Type(Node);

static Type TYPE2Type(Node node){
#ifdef DEBUGING
    if(node == NULL){
        printf("TYPE2attr(),NULL ptr\n");
        return;
    }  
    if(strcmp(node->node_name,"TYPE")!=0){
        printf("TYPE2attr(),node is not TYPE\n");
        return;
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

}

FieldList DefList2FieldList(Node node){

}

Type Specifier2Type(Node node){
    Node child = node->children;
    if(strcmp(child->node_name,"TYPE")==0)
        return TYPE2Type(child);
    else
        return StructSpecifier2Type(child);
}