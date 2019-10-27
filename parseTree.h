#ifndef __parseTree_H__
#define __parseTree_H__

typedef struct Node_* Node;

struct Node_{
	int row;
	int node_type;
	char node_name[50];
	char node_value[50];
	struct Node_* siblings;
	struct Node_* children;	
};

void traverseTree_DLR(Node root, int count);
Node createNode(char*, char*, int, int);
void deleteTree(Node);

#endif
