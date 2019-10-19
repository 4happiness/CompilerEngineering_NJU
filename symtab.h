#ifndef __SYMTAB_H__
#define __SYMTAB_H__
enum {UNOCCUPIED,EXISTING,CONFLICT};

typedef struct Type_* Type;
typedef struct FieldList_* FieldList;
typedef struct Function_* Function;
typedef struct Symbol_* Symbol;
typedef struct SymtabNode_* SymtabNode;

struct Type_ {
    enum {BASIC,ARRAY,STRUCTURE} kind;
    union
    {
        // 基本类型
        enum {INT,FLOAT} basic;
        // 数组类型信息包括元素类型与数组大小构成
        struct 
        {
            Type elem;
            int size;
        }array;
        // 结构体类型信息是一个链表
        FieldList structure;
    }u;
};

struct FieldList_{
    char name[50];
    Type type;
    FieldList tail;
};

struct Function_
{
    Type retType;
    int varNum;
    Type* varTypes;
};

struct Symbol_{
    enum {VARIABLE,FUNCTION,STRUCTNAME} kind;
    char name[50];
    union
    {
        Type type;
        Function func;
    }u;
};

struct SymtabNode_{
    Symbol symbol;
    struct
    {
        SymtabNode next_inStack;
        SymtabNode bottom;
        SymtabNode next_inTable;
    }link;
};

unsigned int hash_pjw(const char*);
void initSymtab();
void freeSymtab();
void push2Stack();
void popStack();
void addSymbol(Symbol);
void freeType(Type);
void freeFieldList(FieldList);
void freeSymbol(Symbol);
void freeFunction(Function);
void freeSymtabNode(SymtabNode);
int checkDefinition(const char*,const int);
Type getType(const char*,const int);
Type typecpy(Type);
FieldList fieldlistcpy(FieldList);


#endif