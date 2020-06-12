%{
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

#include <ast.hh>

//Represents an external file
extern FILE *yyin;

std::string getString(char *in);

int yylex();
void yyerror(const char *s);

AstNode *tree;
AstFuncDec *currentFunc;

std::vector<AstNode *> children;
std::vector<std::string> strList;
std::string args = "";
std::string ret = "V";

bool inCond = false;
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
%token IF ELIF ELSE
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
    | func_call
    | int_dec
    | double_dec
    | var_assign
    | cond_if
    | cond_else
    | end
	;
	
func_dec:
    FUNC ID                         { currentFunc = new AstFuncDec($2); }
    | FUNC ID '(' dec_args ')'      { currentFunc = new AstFuncDec($2);
                                      currentFunc->sig = "(" + args + ")" + ret;
                                      args = "";
                                      ret = "V";
                                      currentFunc->args = strList;
                                      strList.clear();
                                    }
    ;
    
dec_args:
    dec_arg
    | dec_arg ',' dec_args
    ;
    
dec_arg:
    ID ':' INT          { strList.push_back($1); args += "I"; }
    | ID ':' DOUBLE     { strList.push_back($1); args += "D"; }
    ;
    
println:
    PRINTLN STRING      { auto val = getString($2);
                          auto str = new AstString(val);
                          auto println = new AstPrintln;
                          println->children.push_back(str);
                          currentFunc->children.push_back(println); 
                        }
                        
    | PRINTLN INTEGER   { auto i = new AstInt($2);
                          auto println = new AstPrintln;
                          println->children.push_back(i);
                          currentFunc->children.push_back(println);
                        }
                        
    | PRINTLN FLOATL    { auto i = new AstFloat($2);
                          auto println = new AstPrintln;
                          println->children.push_back(i);
                          currentFunc->children.push_back(println);
                        }
                        
    | PRINTLN ID        { auto id = new AstId($2);
                          auto println = new AstPrintln;
                          println->children.push_back(id);
                          currentFunc->children.push_back(println);
                        }
    ;
    
func_call:
    ID '(' ')'              { auto fc = new AstFuncCall($1);
                              currentFunc->children.push_back(fc);
                            }
    | ID '(' args ')'       { auto fc = new AstFuncCall($1);
                              fc->children = children;
                              children.clear();
                              currentFunc->children.push_back(fc);
                            }
    ;
    
args:
    ld_expr                   { }
    | ld_expr ',' args        { }
    ;
    
int_dec:
    INT ID '=' INTEGER          { auto vd = new AstVarDec($2, DataType::Int);
                                  auto i = new AstInt($4);
                                  vd->children.push_back(i);
                                  currentFunc->children.push_back(vd);
                                }
    | INT ID '=' math_expr      { auto vd = new AstVarDec($2, DataType::Int);
                                  currentFunc->children.push_back(vd);
                                  
                                  auto va = new AstVarAssign($2);
                                  va->children = children;
                                  children.clear();
                                  currentFunc->children.push_back(va);
                                }
    | INT ID '=' ID             { auto vd = new AstVarDec($2, DataType::Int);
                                  currentFunc->children.push_back(vd);
                                  
                                  auto va = new AstVarAssign($2);
                                  va->children.push_back(new AstId($4));
                                  currentFunc->children.push_back(va);
                                }
    ;
    
double_dec:
    DOUBLE ID '=' FLOATL        { auto vd = new AstVarDec($2, DataType::Double);
                                  auto flt = new AstFloat($4);
                                  vd->children.push_back(flt);
                                  currentFunc->children.push_back(vd);
                                }
    ;
    
var_assign:
      ID '=' math_expr      { auto va = new AstVarAssign($1);
                              va->children = children;
                              children.clear();
                              currentFunc->children.push_back(va);
                            }
    | ID '=' INTEGER        { auto va = new AstVarAssign($1);
                              auto i = new AstInt($3);
                              va->children.push_back(i);
                              currentFunc->children.push_back(va);
                            }
    | ID '=' FLOATL         { auto va = new AstVarAssign($1);
                              auto i = new AstFloat($3);
                              va->children.push_back(i);
                              currentFunc->children.push_back(va);
                            }
    ;
    
math_expr:
      math_expr '+' ld_expr   { children.push_back(new AstNode(AstType::Add)); }
    | math_expr '-' ld_expr   { children.push_back(new AstNode(AstType::Sub)); }
    | math_expr '*' ld_expr   { children.push_back(new AstNode(AstType::Mul)); }
    | math_expr '/' ld_expr   { children.push_back(new AstNode(AstType::Div)); }
    | ld_expr '+' ld_expr     { children.push_back(new AstNode(AstType::Add)); }
    | ld_expr '-' ld_expr     { children.push_back(new AstNode(AstType::Sub)); }
    | ld_expr '*' ld_expr     { children.push_back(new AstNode(AstType::Mul)); }
    | ld_expr '/' ld_expr     { children.push_back(new AstNode(AstType::Div)); }
    ;
    
cond_if:
    IF ld_expr operator ld_expr     { auto cmp = new AstNode(AstType::If);
                                      cmp->children = children;
                                      children.clear();
                                      currentFunc->children.push_back(cmp);
                                    }
    ;
    
cond_else:
    ELSE            { currentFunc->children.push_back(new AstNode(AstType::Else)); }
    ;
    
ld_expr:
      INTEGER       { children.push_back(new AstInt($1)); }
    | FLOATL        { children.push_back(new AstFloat($1)); }
    | ID            { children.push_back(new AstId($1)); }
    ;
    
operator:
      '>'           { children.push_back(new AstNode(AstType::Greater)); }
    | '<'           { children.push_back(new AstNode(AstType::Less)); }
    ;
    
end:
    END             { currentFunc->children.push_back(new AstEnd);
                      if (inCond) {
                          inCond = false;
                      } else {
                          tree->children.push_back(currentFunc);
                      }
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

