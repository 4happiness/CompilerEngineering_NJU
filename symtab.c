#include "common.h"
#include "symtab.h"
#define SYMTAB_NR 0x3fff

static SymtabNode* symtab;


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
    symtab = (SymtabNode*)malloc(SYMTAB_NR*sizeof(SymtabNode));
    for(int i=0;i<SYMTAB_NR;i++){
        symtab[i]=(SymtabNode)malloc(sizeof(struct SymtabNode_));
        symtab[i]->symbol=NULL;
        symtab[i]->next=NULL;
    }
}

void freeSymtab(){
    for(int i=0;i<SYMTAB_NR;i++)
        freeSymtabNode(symtab[i]);
    free(symtab);
}


void addSymbol(Symbol symbol){
    SymtabNode node = (SymtabNode)malloc(sizeof(struct SymtabNode_));
    node->symbol = symbol;
    unsigned int hash_code = hash_pjw(symbol->name);
    node->next = symtab[hash_code]->next;
    symtab[hash_code]->next = node;
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
    
    freeType(fieldlist->type);
    fieldlist->type = NULL;

    freeFieldList(fieldlist->tail);
    fieldlist->tail = NULL;
    
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
    if(node->next!=NULL)
        freeSymtabNode(node->next);

    freeSymbol(node->symbol);
    node->symbol = NULL;

    free(node);
}

int checkDefinition(const char* name,const int kind){
    SymtabNode node;
    Symbol symbol;
    unsigned int hash_code = hash_pjw(name);
    node = symtab[hash_code];
    while(node->next!=NULL){
        node = node->next;
        symbol = node->symbol;
        if(!strcmp(name,symbol->name)){
            if(kind == symbol->kind)
                return EXISTING;
            else{
                switch (kind)
                {
                case VARIABLE:
                    if(symbol->kind == STRUCTNAME)
                        return CONFLICT;
                    break;

                case STRUCTNAME:
                    if(symbol->kind == VARIABLE)
                        return CONFLICT;
                    break;

                default:
                    break;
                }
            }
        }
    }
    return UNOCCUPIED;
}

/* 返回指向 Type 的指针，未开辟新空间 */
Type getType(const char* name,const int kind){
    SymtabNode node;
    Symbol symbol;
    unsigned int hash_code = hash_pjw(name);
    node = symtab[hash_code];
    while(node->next!=NULL){
        node = node->next;
        symbol = node->symbol;
        if(kind == symbol->kind && !strcmp(name,symbol->name)){
            switch (kind)
            {
            case VARIABLE:
            case STRUCTNAME:
                return symbol->u.type;
                break;

            case FUNCTION:
                return symbol->u.func->retType;
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
    return NULL;
}

int typecpy(Type dest, Type src){
    if (src == NULL){
#ifdef DEBUING
    printf("error typecpy().\n");
#endif
        return FAILURE;
    }
        
    else{
        dest->kind = src->kind;
        switch (src->kind)
        {
        case BASIC:
            dest->u.basic = src->u.basic;
            break;
        
        case ARRAY:
            dest->u.array.elem = (Type)malloc(sizeof(struct Type_));
            if(FAILURE == typecpy(dest->u.array.elem,src->u.array.elem)){
                freeType(dest->u.array.elem);
                return FAILURE;
            }
            dest->u.array.size = src->u.array.size;
            break;

        case STRUCTURE:
            if(src->u.structure == NULL)
                dest->u.structure = NULL;
            else{
                dest->u.structure = (FieldList)malloc(sizeof(struct FieldList_));
                fieldlistcpy(dest->u.structure, src->u.structure);
            }
            break;
    
        default:
            break;
        }
        return SUCCESS;
    }
}

int typecmp(Type dest, Type src){
    if(dest==NULL || src==NULL)
        return FAILURE;
    if(dest->kind != src->kind)
        return FAILURE;
    switch (dest->kind)
    {
    case BASIC:
        if (dest->u.basic == src->u.basic)
            return SUCCESS;
        else
            return FAILURE;
        break;
    
    case ARRAY:
        return typecmp(dest->u.array.elem,src->u.array.elem);
        break;
    
    case STRUCTURE:
        return fieldlistcmp(dest->u.structure,src->u.structure);
        break;

    default:
        break;
    }
}

int fieldlistcpy(FieldList dest, FieldList src){
#ifdef DEBUGING
    if(src == NULL)
        printf("error fieldlistcpy().\n");
#endif
    if(src->tail!=NULL){
        dest->tail = (FieldList)malloc(sizeof(struct FieldList_));
        fieldlistcpy(dest->tail,src->tail);
    }
    else{
        dest->tail=NULL;
    }
    strcpy(dest->name,src->name);
    dest->type = (Type)malloc(sizeof(struct Type_));
    typecpy(dest->type,src->type);
    return SUCCESS;
}

int fieldlistcmp(FieldList dest, FieldList src){
    if(dest == NULL && src!=NULL)
        return FAILURE;
    else if(dest != NULL && src==NULL)
        return FAILURE;
    else{
        int flag_1 = typecmp(dest->type, src->type);
        if(flag_1 == FAILURE)
            return FAILURE;
        int flag_2 = SUCCESS;
        if(dest->tail!=NULL || src->tail!=NULL)
            flag_2 = fieldlistcmp(dest->tail,src->tail);
        return flag_1|flag_2;
    }
    
}

Symbol getSymbol(const char* name, const int kind){
    SymtabNode node;
    Symbol symbol;
    unsigned int hash_code = hash_pjw(name);
    node = symtab[hash_code];
    while(node->next!=NULL){
        node = node->next;
        symbol = node->symbol;
        if(kind == symbol->kind && !strcmp(name,symbol->name))
            return symbol;
    }
    return NULL;
}
