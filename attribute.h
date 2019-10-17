
#include "multiway_tree.h"
#include "semantic.h"

#ifndef __ATTRIBUTE_H__
#define __ATTRIBUTE_H__



extern FieldList DefList2FieldList(Node);
extern Type Specifier2Type(Node);
extern char* IDinVarDec(Node);
extern Type TypeinVarDec(Node, Type);
#endif