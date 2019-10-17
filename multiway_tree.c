#include "common.h"
#include "multiway_tree.h"
void traverseTree_DLR(Node root, int count){
	if(root==NULL)
		return;
	switch(root->node_type){
		case 0:
			for(int i=0;i<count;i++)
				printf(" ");
			printf("%s (%d)\n",root->node_name,root->row);
			break;
		case 1:
			break;
		case 2:	
			for(int i=0;i<count;i++)
				printf(" ");
			if(strcmp(root->node_name,"ID")==0 || strcmp(root->node_name,"TYPE")==0)

				printf("%s: %s\n",root->node_name,root->node_value);
			else if(strcmp(root->node_name,"INT")==0)
				printf("%s: %d\n",root->node_name,atoi(root->node_value));
			else if(strcmp(root->node_name,"FLOAT")==0)
				printf("%s: %f\n",root->node_name,atof(root->node_value));
			else
				printf("%s\n",root->node_name);
			break;
		default:
			break;
	}	



	traverseTree_DLR(root->children, count+2);
	traverseTree_DLR(root->siblings, count);
	return;
}

Node createNode(char* name, char* value, int type, int row){
	Node newNode = (Node)malloc(sizeof(struct Node_));
	strcpy(newNode->node_name, name);
	strcpy(newNode->node_value, value);
	newNode->node_type = type;
	newNode->row = row;
	newNode->children = NULL;
	newNode->siblings = NULL;
	return newNode;
}

void deleteTree(Node root){
	if(root==NULL)
		return;
	deleteTree(root->children);
	deleteTree(root->siblings);
	free(root);
	return;
}
