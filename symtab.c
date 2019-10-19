#include "common.h"
#include "symtab.h"
#define SYMTAB_NR 0x3fff

static SymtabNode* hashTable;
static SymtabNode symbolStack;

static void initHashTable();
static void initSymbolStack();


unsigned int hash_pjw(const char* name){
    unsigned int val=0,i;
    for(;*name;++name){
        val = (val << 2) + *name;
        if(i = val & ~SYMTAB_NR)
            val = (val ^ (i >> 12)) & 0x3fff;
    }
    return val;
}

void initSymtab(){
    initHashTable();
    initSymbolStack();
}

static void initHashTable(){
    hashTable = (SymtabNode*)malloc(SYMTAB_NR*sizeof(SymtabNode));
    for(int i=0;i<SYMTAB_NR;i++){
        hashTable[i]=(SymtabNode)malloc(sizeof(struct SymtabNode_));
        hashTable[i]->symbol=NULL;
        hashTable[i]->link.next_inStack=NULL;
        hashTable[i]->link.bottom=NULL;
        hashTable[i]->link.next_inTable=NULL;
    }
}

static void initSymbolStack(){
    symbolStack = (SymtabNode)malloc(sizeof(struct SymtabNode_));
    symbolStack->symbol=NULL;
    symbolStack->link.next_inStack=NULL;
    symbolStack->link.next_inTable=NULL;
    symbolStack->link.bottom=NULL;
}

void freeSymtab(){
    for(int i=0;i<SYMTAB_NR;i++){
        free(hashTable[i]);
        hashTable[i]=NULL;
    }
    free(hashTable);
    free(symbolStack);
}

void push2Stack(){
    SymtabNode node = (SymtabNode)malloc(sizeof(struct SymtabNode_));
    node->symbol = NULL;
    node->link.next_inStack = NULL;
    node->link.next_inTable = NULL;
    node->link.bottom = symbolStack;
    symbolStack = node;
}

void popStack(){
    SymtabNode current;
    while(symbolStack->link.next_inStack!=NULL){
        current = symbolStack->link.next_inStack;
        symbolStack->link.next_inStack=current->link.next_inStack;
        unsigned int hash_code = hash_pjw(current->symbol->name);
#ifdef DEBUGING
    if(hashTable[hash_code]->link.next_inTable!=current)
        printf("popStack(),address error\n");
#endif
        hashTable[hash_code]->link.next_inTable = current->link.next_inTable;
        freeSymtabNode(current);
    }
    current = symbolStack;
    symbolStack = current->link.bottom;
    freeSymtabNode(current);
}

void addSymbol(Symbol symbol){
    SymtabNode node = (SymtabNode)malloc(sizeof(struct SymtabNode_));
    node->symbol = symbol;
    node->link.next_inStack = symbolStack->link.next_inStack;
    symbolStack->link.next_inStack = node;
    unsigned int hash_code = hash_pjw(symbol->name);
    node->link.next_inTable = hashTable[hash_code]->link.next_inTable;
    hashTable[hash_code]->link.next_inTable = node;
    node->link.bottom = NULL;
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
        type->u.array.elem = NULL;
        break;

    case STRUCTURE:
        freeFieldList(type->u.structure);
        type->u.structure = NULL;
        break;

    default:
        break;
    }
    free(type);
    return;
}

void freeFieldList(FieldList fieldlist){
    if(fieldlist==NULL)
        return;
    
    freeFieldList(fieldlist->tail);
    fieldlist->tail = NULL;
    freeType(fieldlist->type);
    fieldlist->type = NULL;
    free(fieldlist);
    return;
}

void freeFunction(Function function){
    if(function==NULL)
        return;
    
    freeType(function->retType);
    function->retType = NULL;
    for(int i=0;i<function->varNum;i++){
        freeType(function->varTypes[i]);
        function->varTypes[i] = NULL;
    }
    free(function->varTypes);
    function->varTypes = NULL;
    free(function);
    return;
}

void freeSymbol(Symbol symbol){
    if(symbol==NULL)
        return;
    
    switch (symbol->kind)
    {
    case STRUCTNAME:
    case VARIABLE:
        freeType(symbol->u.type);
        symbol->u.type = NULL;
        break;
    
    case FUNCTION:
        freeFunction(symbol->u.func);
        symbol->u.func = NULL;
        break;
    
    default:
        break;
    }
    free(symbol);
}

void freeSymtabNode(SymtabNode node){
    freeSymbol(node->symbol);
    node->symbol = NULL;
    free(node);
}

int checkDefinition(const char* name,const int kind){
    SymtabNode node;
    Symbol symbol;
    node = symbolStack;
    while(node->link.next_inStack!=NULL){
        node = node->link.next_inStack;
        symbol = node->symbol;
        if(!strcmp(name,symbol->name)){
            if(kind == symbol->kind)
                return EXISTING;
            else
                return CONFLICT;
        }
    }
    return UNOCCUPIED;
}

/* 返回指向 Type 的指针，未开辟新空间 */
Type getType(const char* name,const int kind){
    SymtabNode node;
    Symbol symbol;
    unsigned int hash_code;
    hash_code = hash_pjw(name);
    node = hashTable[hash_code];
    while(node->link.next_inTable!=NULL){
        node = node->link.next_inTable;
        symbol = node->symbol;
        if(kind == symbol->kind){
            if(!strcmp(name,symbol->name)){
                switch (kind)
                {
                case VARIABLE:
                case STRUCTNAME:
                    return symbol->u.type;
                    break;

                default:
#ifdef DEBUGING
                    printf("getType(),error kind.\n");
#endif
                    return NULL;
                    break;
                }
            }
        }
    }
    return NULL;
}

Type typecpy(Type src){
    if (src == NULL)
        return NULL;
    else{
        Type dest = (Type)malloc(sizeof(struct Type_));
        dest->kind = src->kind;
        switch (src->kind)
        {
        case BASIC:
            dest->u.basic = src->u.basic;
            break;
        
        case ARRAY:
            dest->u.array.elem = typecpy(src->u.array.elem);
            dest->u.array.size = src->u.array.size;
            break;

        case STRUCTURE:
            dest->u.structure = fieldlistcpy(src->u.structure);
            break;
    
        default:
            break;
        }
        return dest;
    }
}

FieldList fieldlistcpy(FieldList src){
    if(src == NULL)
        return NULL;
    FieldList dest = (FieldList)malloc(sizeof(struct FieldList_));
    strcpy(dest->name,src->name);
    dest->type = typecpy(src->type);
    dest->tail = fieldlistcpy(src->tail);
    return dest;
}