#include "symtab.h"
#include "multiway_tree.h"

#ifndef __PARSE_PRODUCTION_H__
#define __PARSE_PRODUCTION_H__

int parseVarList_toNum(Node);
Type parseSpecifier(const Node);
Symbol parseVarDec_toSymbol(const Node,const Type);
int parseVarList_toNum(Node);
Symbol parseParamDec_toSymbol(Node);
Symbol* parseVarList_toSymbols(Node);
char* parseVarDec_toID(const Node);
Type parseVarDec_toType(const Node, const Type);
Type TypeOfExp(Node);
void printExp(Node);
#endif