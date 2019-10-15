%{
#include "syntax.tab.h"
extern int error_flag;
char currentStr[200];
/*
int yycolumn = 1;
#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;\
			yylloc.first_column = yycolumn;\
			yylloc.last_column = yycolumn + yyleng - 1;\
			yycolumn += yyleng;
*/
%}

digit [0-9]
letter_ [_a-zA-Z]
ID {letter_}({letter_}|{digit})*
INT 0|[1-9]{digit}*
exponent [Ee][+-]?{digit}+
basis ({digit}+\.)|(\.{digit}+)
FLOAT ({digit}+\.{digit}+{exponent}?)|({basis}{exponent})
illegal_FLOAT ({basis}[Ee]?)|({digit}+\.{digit}+[Ee]\.?)|(\.[Ee]?[+-]?{digit}+)|({basis}[Ee]\.?)
WS (\t|" ")+

%option yylineno
%%
\n|(\r\n) {/*yycolumn = 1;*/}
int { yylval.type_node = createNode("TYPE","int",2,yylineno);strncpy(currentStr,"int",200);return TYPE; }
float {yylval.type_node = createNode("TYPE","float",2,yylineno);strncpy(currentStr,"float",200);return TYPE;}
struct { yylval.type_node = createNode("STRUCT","",2,yylineno);strncpy(currentStr,"struct",200);return STRUCT; }
return { yylval.type_node = createNode("RETURN","",2,yylineno);strncpy(currentStr,"return",200);return RETURN; }
if { yylval.type_node = createNode("IF","",2,yylineno);strncpy(currentStr,"if",200);return IF; }
else { yylval.type_node = createNode("ELSE","",2,yylineno);strncpy(currentStr,"else",200);return ELSE; }
while { yylval.type_node = createNode("WHILE","",2,yylineno);strncpy(currentStr,"while",200);return WHILE; }
{ID} { yylval.type_node = createNode("ID",yytext,2,yylineno);strncpy(currentStr,yytext,200);return ID; }
{WS}
{FLOAT} { yylval.type_node = createNode("FLOAT",yytext,2,yylineno);strncpy(currentStr,yytext,200);return FLOAT; }
{illegal_FLOAT} { printf("Error type A at Line %d: Illegal floating point number \"%s\"\n", yylineno, yytext);
		error_flag=1;
		yylval.type_node = createNode("Illegal",yytext,3,yylineno);
		strncpy(currentStr,yytext,200);
		return FLOAT;
		}
{INT} { yylval.type_node = createNode("INT",yytext,2,yylineno);strncpy(currentStr,yytext,200);return INT; }
"(" { yylval.type_node = createNode("LP","",2,yylineno);strncpy(currentStr,"(",200);return LP; }
")" { yylval.type_node = createNode("RP","",2,yylineno);strncpy(currentStr,")",200);return RP; }
"[" { yylval.type_node = createNode("LB","",2,yylineno);strncpy(currentStr,"[",200);return LB; }
"]" { yylval.type_node = createNode("RB","",2,yylineno);strncpy(currentStr,yytext,200);return RB; }
"{" { yylval.type_node = createNode("LC","",2,yylineno);strncpy(currentStr,yytext,200);return LC; }
"}" { yylval.type_node = createNode("RC","",2,yylineno);strncpy(currentStr,yytext,200);return RC; }
";" { yylval.type_node = createNode("SEMI","",2,yylineno);strncpy(currentStr,yytext,200);return SEMI; }
"," { yylval.type_node = createNode("COMMA","",2,yylineno);strncpy(currentStr,yytext,200);return COMMA; }
"=" { yylval.type_node = createNode("ASSIGNOP","",2,yylineno);strncpy(currentStr,yytext,200);return ASSIGNOP; }
">"|"<"|">="|"<="|"=="|"!=" { yylval.type_node = createNode("RELOP","",2,yylineno);strncpy(currentStr,yytext,200);return RELOP; }
"+" { yylval.type_node = createNode("PLUS","",2,yylineno);strncpy(currentStr,yytext,200);return PLUS; }
"-" { yylval.type_node = createNode("MINUS","",2,yylineno);strncpy(currentStr,yytext,200);return MINUS; }
"*" { yylval.type_node = createNode("STAR","",2,yylineno);strncpy(currentStr,yytext,200);return STAR; }
"/" { yylval.type_node = createNode("DIV","",2,yylineno);strncpy(currentStr,yytext,200);return DIV; }
"&&" { yylval.type_node = createNode("AND","",2,yylineno);strncpy(currentStr,yytext,200);return AND; }
"||" { yylval.type_node = createNode("OR","",2,yylineno);strncpy(currentStr,yytext,200);return OR; }
"." { yylval.type_node = createNode("DOT","",2,yylineno);strncpy(currentStr,yytext,200);return DOT; }
"!" { yylval.type_node = createNode("NOT","",2,yylineno);strncpy(currentStr,yytext,200);return NOT; }
. { printf("Error type A at Line %d: Mysterious characters \"%s\"\n", yylineno, yytext); error_flag=1; }
%%
