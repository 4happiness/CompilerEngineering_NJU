#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "semantic.h"
#include "multiway_tree.h"
#define SYMTAB_NR 0x3fff

static SymtabNode* hashTable;
static SymtabNode symStack;

void initSymtab(){
    hashTable = (SymtabNode*)malloc(SYMTAB_NR*sizeof(SymtabNode));
    symStack = (SymtabNode)malloc(sizeof(struct SymtabNode_));
    for(int i=0;i<SYMTAB_NR;i++){
        hashTable[i]=(SymtabNode)malloc(sizeof(struct SymtabNode_));
        hashTable[i]->symbol=NULL;
        hashTable[i]->link.next_inStack=NULL;
        hashTable[i]->link.next_inTable=NULL;
    }
    symStack->symbol=NULL;
    symStack->link.next_inStack=NULL;
    symStack->link.next_inTable=NULL;
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