#ifndef __SEMANTIC_H__
#define __SEMANTIC_H__

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
        int basic;
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
    char name[33];
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
    enum {VARIABLE,FUNCTION} kind;
    char name[33];
    union
    {
        Type varible;
        Function func;
    }u;
};

struct SymtabNode_{
    Symbol symbol;
    struct
    {
        SymtabNode next_inStack;
        SymtabNode next_inTable;
    }link;
};

extern void initSymtab();
#endif