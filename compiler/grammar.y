%{
#include <stdio.h>
#include <string.h>
#include <string>
#include <map>

#include <ast.hh>

//Represents an external file
extern FILE *yyin;

std::string getString(char *in);

int yylex();
void yyerror(const char *s);

AstNode *tree;
AstFuncDec *currentFunc;
%}

%union {
	char* stype;
	int itype;
	float ftype;
}

%token FUNC
%token PRINTLN
%token END
%token INT FLOAT DOUBLE
%token <stype> ID
%token <itype> INTEGER 
%token <ftype> FLOATL
%token <stype> STRING

%token '+' '-' '*' '/'
%token '='

%%
all_statements: statement all_statements | /* empty */;

statement:
      func_dec
    | println
    | int_dec
    | double_dec
    | var_assign
    | end
	;
	
func_dec:
    FUNC ID             { currentFunc = new AstFuncDec($2); }
    ;
    
println:
    PRINTLN STRING      { }
                        
    | PRINTLN INTEGER   { }
                        
    | PRINTLN FLOATL    { }
                        
    | PRINTLN ID        { }
    ;
    
int_dec:
    INT ID '=' INTEGER          { }
    ;
    
double_dec:
    DOUBLE ID '=' FLOATL        { }
    ;
    
var_assign:
      ID '=' math_expr      { }
    | ID '=' INTEGER        { }
    | ID '=' FLOATL         { }
    ;
    
math_expr:
      math_expr '+' ld_expr   { }
    | math_expr '-' ld_expr   { }
    | math_expr '*' ld_expr   { }
    | math_expr '/' ld_expr   { }
    | ld_expr '+' ld_expr     { }
    | ld_expr '-' ld_expr     { }
    | ld_expr '*' ld_expr     { }
    | ld_expr '/' ld_expr     { }
    ;
    
ld_expr:
      INTEGER       { }
    | FLOATL        { }
    | ID            { }
    ;
    
end:
    END             { currentFunc->children.push_back(new AstEnd);
                      tree->children.push_back(currentFunc);
                    }
    ;

%%

//Remove quotes
std::string getString(char *in) {
    std::string ret = "";
    
    for (int i = 1; i<strlen(in)-1; i++) {
        ret += in[i];
    }
    
    return ret;
}

//Our parsing function
void parse(const char *path, AstNode *t) {
    tree = t;
	yyin = fopen(path, "r");
	yyparse();
}

//Handle syntax errors
void yyerror(const char *s) {
	printf("Syntax error: %s\n", s);
}

int yywrap() { return 1; }

