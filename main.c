#include <stdio.h>
#include <stdlib.h>
#include "multiway_tree.h"
#include "semantic.h"

extern int yylineno;
extern int errorNum;
extern Node root;
extern int yyparse(void);
extern void yyrestart(FILE*);

int main(int argc, char** argv){
	if(argc <= 1)
		return 1;
	FILE* f = fopen(argv[1], "r");
	if(!f){
		perror(argv[1]);
		return 1;
	}
	yylineno=1;
	errorNum=0;
	yyrestart(f);
	//yyparse();
	if(!errorNum){
		initSymtab();
		//traverseTree_DLR(root,0);
		//deleteTree(root);
	}
	return 0;
}
