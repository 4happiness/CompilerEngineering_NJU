#include "parseTree.h"

#ifndef __INTERCODE_H__
#define __INTERCODE_H__

typedef struct Operand_* Operand;
typedef struct InterCode_* InterCode;
typedef struct InterCodes_* InterCodes;
typedef struct ArgsList_* ArgsList;
struct Operand_
{
    enum {irSELF,irADDRESS,irVALUE} which;
    enum {irVARIABLE, irCONSTANT, irTempVAR, irAllocVAR} kind;
    union 
    {
        char id[50];
        int value;
    }u;
};

struct InterCode_
{
    enum {irLABEL, irFUNCTION, irASSIGN, irPLUS, irMINUS, irSTAR,
        irDIV, irGOTO, irCOND, irRETURN, irDEC,
        irARG, irCALL, irPARAM, irREAD, irWRITE
    } kind;

    union 
    {

        int label_no;

        char id[50];
        
        struct 
        {
            Operand right,left;
        }assign;

        struct
        {
            Operand result,op1,op2;
        }binop;

        struct
        {
            Operand x,y;
            char relop[4];
            int label_no;
        }condition;

        Operand ret;

        struct
        {
            Operand alloc;
            int size;
        }dec;

        Operand arg;

        struct
        {
            Operand x;
            char func[50];
        }call;
        
        Operand param;

        Operand read;

        Operand write;

    }u;
};

struct InterCodes_
{
    InterCode code;
    InterCodes prev, next;
};

struct ArgsList_{
    Operand x;
    ArgsList next;
};

void generateIR(Node);
#endif