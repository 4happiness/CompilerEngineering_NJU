#include "common.h"
#include "semantic.h"
#include "multiway_tree.h"
#include "attribute.h"
#define SYMTAB_NR 0x3fff

static SymtabNode* hashTable;
static SymtabNode symStack;

static void push2Stack();
static void popStack();
unsigned int hash_pjw(char*);
void freeType(Type);
void freeFieldList(FieldList);
void freeFunction(Function);
void freeSymbol(Symbol);
void freeSymtabNode(SymtabNode node);

void initSymtab(){
    hashTable = (SymtabNode*)malloc(SYMTAB_NR*sizeof(SymtabNode));
    symStack = (SymtabNode)malloc(sizeof(struct SymtabNode_));
    for(int i=0;i<SYMTAB_NR;i++){
        hashTable[i]=(SymtabNode)malloc(sizeof(struct SymtabNode_));
        hashTable[i]->symbol=NULL;
        hashTable[i]->link.next_inStack=NULL;
        hashTable[i]->link.bottom=NULL;
        hashTable[i]->link.next_inTable=NULL;
    }
    symStack->symbol=NULL;
    symStack->link.next_inStack=NULL;
    symStack->link.next_inTable=NULL;
    symStack->link.bottom=NULL;
}

void freeSymtab(){
    for(int i=0;i<SYMTAB_NR;i++){
        free(hashTable[i]);
    }
    free(symStack);
}

void semanticAnalysis(Node root){
    if(root==NULL)
        return;
    char node_name[50];
    strcpy(node_name,root->node_name);
    if(!strcmp(node_name,"Program")){
        push2Stack();
        semanticAnalysis(root->children);
        popStack();
#ifdef DEBUGING
        if(root->siblings!=NULL)
            printf("Program error!\n");
#endif
    }
    else if(!strcmp(node_name,"ExtDef")){
        Node Specifier = root->children;
        if(!strcmp(Specifier->siblings->node_name,"ExtDecList")){
            Node ExtDecList = Specifier->siblings;
            Node VarDec = ExtDecList->children;
            Node ID = VarDec->children;
            while(1){
                while(ID->children!=NULL)
                    ID = ID->children;

                /* Symbol part not done */
                
                SymtabNode newNode = (SymtabNode)malloc(sizeof(struct SymtabNode_));
                newNode->link.bottom = NULL;
                newNode->link.next_inStack = symStack->link.next_inStack;
                symStack->link.next_inStack = newNode;
                unsigned int hash = hash_pjw(ID->node_value);
                newNode->link.next_inTable = hashTable[hash]->link.next_inTable;
                hashTable[hash]->link.next_inTable=newNode;

                if(VarDec->siblings==NULL)
                    break;
                ExtDecList=VarDec->siblings->siblings;
                VarDec=ExtDecList->children;
            }
                
            

        }
        else if(!strcmp(Specifier->siblings->node_name,"FunDec")){
            Node FunDec = Specifier->siblings;
            Node CompSt = FunDec->siblings;

        }
    }
    else if(!strcmp(node_name,"Def")){
        printf("Def\n");
    }
    else{
        semanticAnalysis(root->children);
        semanticAnalysis(root->siblings);
    }
}




unsigned int hash_pjw(char* name){
    unsigned int val=0,i;
    for(;*name;++name){
        val = (val << 2) + *name;
        if(i = val & ~SYMTAB_NR)
            val = (val ^ (i >> 12)) & 0x3fff;
    }
    return val;
}

void freeType(Type type){
    if(type==NULL)
        return;

    switch (type->kind)
    {
    case BASIC:
        break;

    case ARRAY:
        freeType(type->u.array.elem);
        break;

    case STRUCTURE:
        freeFieldList(type->u.structure);
        break;

    default:
        break;
    }
    free(type);
}

void freeFieldList(FieldList fieldlist){
    if(fieldlist==NULL)
        return;
    
    freeType(fieldlist->type);
    freeFieldList(fieldlist->tail);
    free(fieldlist);
}

void freeFunction(Function function){
    if(function==NULL)
        return;
    
    freeType(function->retType);
    for(int i=0;i<function->varNum;i++)
        freeType(function->varTypes[i]);
    return;
}

void freeSymbol(Symbol symbol){
    if(symbol==NULL)
        return;
    
    switch (symbol->kind)
    {
    case VARIABLE:
        freeType(symbol->u.varible);
        break;
    
    case FUNCTION:
        freeFunction(symbol->u.func);
        break;

    default:
        break;
    }
    free(symbol);
}

void freeSymtabNode(SymtabNode node){
    freeSymbol(node->symbol);
    free(node);
}
static void push2Stack(){
    SymtabNode newNode = (SymtabNode)malloc(sizeof(struct SymtabNode_));
    newNode->symbol=NULL;
    newNode->link.next_inStack=NULL;
    newNode->link.next_inTable=NULL;
    newNode->link.bottom=symStack;
    symStack=newNode;
}

static void popStack(){
    SymtabNode current;
    while(symStack->link.next_inStack!=NULL){
        current = symStack->link.next_inStack;
        symStack->link.next_inStack=current->link.next_inStack;
        unsigned int hash = hash_pjw(current->symbol->name);
#ifdef DEBUGING
    if(hashTable[hash]->link.next_inTable!=current)
        printf("popStack(),address error\n");
#endif
        hashTable[hash]->link.next_inTable = current->link.next_inTable;
        freeSymtabNode(current);
    }
    current = symStack;
    symStack = current->link.bottom;
    freeSymtabNode(current);
}