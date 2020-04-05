#include "common.h"
#include "parseTree.h"
#include "semantic.h"
#include "symtab.h"
#include "intercode.h"

extern int yylineno;
extern int errorNum;
extern Node root;
extern int yyparse(void);
extern void yyrestart(FILE*);
char* path;

int main(int argc, char** argv){
	if(argc <= 1)
		return 1;
	FILE* f = fopen(argv[1], "r");

	path = (char*)malloc(sizeof(char)*(strlen(argv[2])+1));
	
	strcpy(path,argv[2]);

	if(!f){
		perror(argv[1]);
		return 1;
	}
	yylineno=1;
	errorNum=0;
	root=NULL;
	yyrestart(f);
	yyparse();
	if(!errorNum){
		//traverseTree_DLR(root,0);
		semanticAnalysis(root);
		generateIR(root);
		freeSymtab();
		deleteTree(root);
	}

	free(path);

	return 0;
}
