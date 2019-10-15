#include <stdio.h>
#include <stdlib.h>
#include "multiway_tree.h"
extern int yyparse(void);
//extern char errorMsg[];
extern void yyrestart(FILE*);

Node* root;
int error_flag;

int main(int argc, char** argv){
	error_flag=0;
	//errorMsg[0]='\0';
	if(argc <= 1)
		return 1;
	FILE* f = fopen(argv[1], "r");
	if(!f){
		perror(argv[1]);
		return 1;
	}
	yyrestart(f);
	int ret = yyparse();
	if((!error_flag)&&(!ret)){
		printTree_pre(root,0);
		deleteTree(root);
	}
	return 0;
}
