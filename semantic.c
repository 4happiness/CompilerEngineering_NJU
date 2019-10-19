#include "common.h"
#include "semantic.h"
#include "multiway_tree.h"
#include "symtab.h"
#include "parseProduction.h"


void semanticAnalysis(Node root){
    if(root==NULL)
        return;
    char node_name[50];
    strcpy(node_name,root->node_name);
    if(!strcmp(node_name,"Program")){
        push2Stack();
        semanticAnalysis(root->children);
        popStack();
#ifdef DEBUGING
        if(root->siblings!=NULL)
            printf("Program error!\n");
#endif
    }
    else if(!strcmp(node_name,"ExtDef")){
        Node Specifier = root->children;
        if(!strcmp(Specifier->siblings->node_name,"ExtDecList")){
            Node ExtDecList = Specifier->siblings;
            Node VarDec = ExtDecList->children;
            Type type = parseSpecifier(Specifier);
            while(1){
                Symbol symbol = parseVarDec_toSymbol(VarDec,typecpy(type));
                if(UNOCCUPIED == checkDefinition(symbol->name,VARIABLE))
                    addSymbol(symbol);
                else{
                    printf("Error type 3 at Line %d: Redefined variable\"%s\"\n",VarDec->row,symbol->name);
                    freeSymbol(symbol);
                }     
                if(VarDec->siblings==NULL)
                    break;
                ExtDecList=VarDec->siblings->siblings;
                VarDec=ExtDecList->children;
            }
            freeType(type);
        }
        else if(!strcmp(Specifier->siblings->node_name,"FunDec")){
            Node FunDec = Specifier->siblings;
            Node CompSt = FunDec->siblings;
            Node ID = FunDec->children;
            Node VarList = ID->siblings->siblings;
            Type type = parseSpecifier(Specifier);
            Symbol symbol = (Symbol)malloc(sizeof(struct Symbol_));
            Symbol* paramSymbols = NULL;
            symbol->kind = FUNCTION;
            strcpy(symbol->name,ID->node_value);
            symbol->u.func = (Function)malloc(sizeof(struct Function_));
            symbol->u.func->retType = type;
            if(!strcmp(VarList->node_name,"VarList")){
                paramSymbols = parseVarList_toSymbols(VarList);
                int num = parseVarList_toNum(VarList);
                symbol->u.func->varNum = num;
                symbol->u.func->varTypes = (Type*)malloc(num*sizeof(Type));
                for(int i=0;i<num;i++)
                    symbol->u.func->varTypes[i] = typecpy(paramSymbols[i]->u.type);
            }
            else{
                symbol->u.func->varNum = 0;
                symbol->u.func->varTypes = NULL;
            }

            int num = symbol->u.func->varNum;

            if(UNOCCUPIED == checkDefinition(symbol->name,FUNCTION))
                addSymbol(symbol);
            else{
                printf("Error type 4 at Line %d: Redefined function\"%s\"\n",ID->row,ID->node_value);
                freeSymbol(symbol);
            }
            push2Stack();
            
            for(int i=0;i<num;i++)
                addSymbol(paramSymbols[i]);
            semanticAnalysis(CompSt->children);
            popStack();
        }
        else{
            Type type = parseSpecifier(Specifier);
            freeType(type);
            type = NULL;
        }
        semanticAnalysis(root->siblings);
    }
    else if(!strcmp(node_name,"Def")){
        printf("Def\n");
    }
    else if(!strcmp(node_name,"CompSt")){
        push2Stack();
        semanticAnalysis(root->children);
        popStack();
        semanticAnalysis(root->siblings);
    }
    else{
        semanticAnalysis(root->children);
        semanticAnalysis(root->siblings);
    }
}
