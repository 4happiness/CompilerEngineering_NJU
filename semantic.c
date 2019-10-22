#include "common.h"
#include "semantic.h"
#include "multiway_tree.h"
#include "symtab.h"
#include "parseProduction.h"

static Symbol current_func;
void semanticAnalysis(Node root){
    if(root==NULL)
        return;
    char node_name[50];
    strcpy(node_name,root->node_name);
    if(!strcmp(node_name,"Program")){
        initSymtab();
        semanticAnalysis(root->children);
        freeSymtab();
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
                Type type_2 = (Type)malloc(sizeof(struct Type_));
                typecpy(type_2,type);
                Symbol symbol = parseVarDec_toSymbol(VarDec,type_2);
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
            if(!strcmp(FunDec->siblings->node_name,"SEMI")){
                printf("Error type B at Line %d: Incomplete definition of function \"%s\".\n",FunDec->row,FunDec->children->node_value);
            }
            else{
                Node CompSt = FunDec->siblings;
                Node ID = FunDec->children;
                Node VarList = ID->siblings->siblings;
                Type type = parseSpecifier(Specifier);
                Symbol symbol = (Symbol)malloc(sizeof(struct Symbol_));
                symbol->kind = FUNCTION;
                strcpy(symbol->name,ID->node_value);
                symbol->u.func = (Function)malloc(sizeof(struct Function_));
                symbol->u.func->retType = type;

                if(!strcmp(VarList->node_name,"VarList")){
                    int num = parseVarList_toNum(VarList);
                    symbol->u.func->varNum = num;
                    symbol->u.func->varTypes = (Type*)malloc(num*sizeof(Type));
                    for(int i=0;i<num;i++)
                        symbol->u.func->varTypes[i] = (Type)malloc(sizeof(struct Type_));

                    Node ParamDec;
                    Node Specifier_2;
                    Type type_2;
                    Node VarDec;
                    for(int i=0;i<num;i++){
                        ParamDec = VarList->children;
                        if(ParamDec->siblings!=NULL)
                            VarList = ParamDec->siblings->siblings;
                        Specifier_2 = ParamDec->children;
                        VarDec = Specifier_2->siblings;
                        type_2 = parseSpecifier(Specifier_2);
                        symbol->u.func->varTypes[i] = parseVarDec_toType(VarDec,type_2);
                        char* name = parseVarDec_toID(VarDec);
                        if(UNOCCUPIED == checkDefinition(name,VARIABLE)){
                            Symbol temp_symbol = (Symbol)malloc(sizeof(struct Symbol_));
                            temp_symbol->kind = VARIABLE;
                            strcpy(temp_symbol->name,name);
                            temp_symbol->u.type = (Type)malloc(sizeof(struct Type_));
                            typecpy(temp_symbol->u.type,symbol->u.func->varTypes[i]);
                            addSymbol(temp_symbol);
                        }
                        else{
                            printf("Error type 3 at Line %d: Redefined variable \"%s\".\n",VarDec->row,name);
                        }
                    }          
                }
                else{
                    symbol->u.func->varNum = 0;
                    symbol->u.func->varTypes = NULL;
                }

                if(UNOCCUPIED == checkDefinition(symbol->name,FUNCTION)){
                    current_func = symbol;
                    addSymbol(symbol);
                }
                
                else{
                    printf("Error type 4 at Line %d: Redefined function \"%s\"\n",ID->row,ID->node_value);
                    freeSymbol(symbol);
                }
            
                semanticAnalysis(CompSt->children);
                }
        }
        else{
            Type type = parseSpecifier(Specifier);
            freeType(type);
            type = NULL;
        }
        semanticAnalysis(root->siblings);
    }
    else if(!strcmp(node_name,"Def")){
        Node Specifier = root->children;
        Node DecList = Specifier->siblings;
        Node Dec;
        Node VarDec;
        Node Exp;
        Type baseType = parseSpecifier(Specifier);
        Symbol symbol;
        if(baseType != NULL){
            while(1){
                Dec = DecList->children;
                VarDec = Dec->children;
                Type baseType_2 = (Type)malloc(sizeof(struct Type_));
                typecpy(baseType_2,baseType);
                symbol = parseVarDec_toSymbol(VarDec,baseType_2);
                if(VarDec->siblings!=NULL){
                    Exp = VarDec->siblings->siblings;
                    if(FAILURE == typecmp(symbol->u.type,TypeOfExp(Exp))){
                        printf("Error type 5 at Line %d: Type mismatched for assignment.\n",Exp->row);
                    }
                }
                if(UNOCCUPIED == checkDefinition(symbol->name,VARIABLE)){
                    addSymbol(symbol);
                }
                else
                {
                    printf("Error type 3 at Line %d: Redefined variable \"%s\".\n",VarDec->row,symbol->name);
                    freeSymbol(symbol);
                }
                if(Dec->siblings == NULL)
                    break;
                DecList = Dec->siblings->siblings;
                
            }
            freeType(baseType);
        }
        
        semanticAnalysis(root->siblings);
    }
    else if(!strcmp(node_name,"Stmt")){
        if(!strcmp(root->children->node_name,"Exp")){
            TypeOfExp(root->children);
        }
        else if(!strcmp(root->children->node_name,"CompSt")){
            semanticAnalysis(root->children);
        }
        else if(!strcmp(root->children->node_name,"RETURN")){
            Type retType = TypeOfExp(root->children->siblings);
            //printf("%d,%d\n",retType->kind,current_func->u.func->retType->kind);
            if(FAILURE == typecmp(retType,current_func->u.func->retType))
                printf("Error type 8 at Line %d: Type mismatched for return.\n",root->row);    
        }
        else if(!strcmp(root->children->node_name,"IF")){
            
            Node Exp = root->children->siblings->siblings;
            Node Stmt_1 = Exp->siblings->siblings;
            Type type = TypeOfExp(Exp);
            if(type!=NULL){
                if(type->kind!=BASIC || type->u.basic!=INT)
                    printf("Error type 7 at Line %d: condition error.\n",Exp->row);
            }
            
            semanticAnalysis(Stmt_1);
            if(Stmt_1->siblings!=NULL)
                semanticAnalysis(Stmt_1->siblings->siblings);
        }
        else{
            Node Exp = root->children->siblings->siblings;
            Node Stmt = Exp->siblings->siblings;
            Type type = TypeOfExp(Exp);
            if(type!=NULL){
                if(type->kind!=BASIC || type->u.basic!=INT)
                    printf("Error type 7 at Line %d: condition error.\n",Exp->row);
            }
            semanticAnalysis(Stmt);
        }
        semanticAnalysis(root->siblings);
    }
    else{
        semanticAnalysis(root->children);
        semanticAnalysis(root->siblings);
    }
}
