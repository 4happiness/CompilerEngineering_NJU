#ifndef __ASSEMBLY_H__
#define __ASSEMBLY_H__

#include "common.h"
#include "intercode.h"

typedef struct asmSymtabNode_* asmSymtabNode;
typedef struct frame_* frame;
enum{asmVARIABLE,asmALLOC,asmTEMP};
enum{MEM,NONMEM};
struct asmSymtabNode_
{
    
    int no;
    int relAddr;
    asmSymtabNode next;
};
struct frame_
{
    int size;
    frame next;
};

extern char* path;
void generateASM(InterCodes intercodes);

#endif