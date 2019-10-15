#ifndef __MULTIWAY_TREE_H__
#define __MULTIWAY_TREE_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Node_t{
	int row;
	int node_type;
	char node_name[20];
	char node_value[33];
	struct Node_t* siblings;
	struct Node_t* children;	
}Node;

void printTree_pre(Node* root, int count);
Node* createNode(char*, char*, int, int);
void deleteTree(Node*);

#endif
