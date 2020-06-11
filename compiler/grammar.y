%{
#include <stdio.h>
#include <string.h>
#include <string>
#include <map>

#include <javabuilder.hh>
#include <javafile.hh>
#include <javapool.hh>

//Represents an external file
extern FILE *yyin;

//The Java Builder class
JavaBuilder *builder;
JavaFunc *current;

std::string getString(char *in);
void updateCodes();

int yylex();
void yyerror(const char *s);

//Needed for variable control
enum class DataType {
    None,
    Int,
    Double
};

std::map<std::string, DataType> vars;
DataType currentType = DataType::None;

//Type-specific opcodes
JavaCode Add = JavaCode::IAdd;
JavaCode Sub = JavaCode::ISub;
JavaCode Mul = JavaCode::IMul;
JavaCode Div = JavaCode::IDiv;
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
    FUNC ID             { current = builder->createMain(); }
    ;
    
println:
    PRINTLN STRING      { std::string s = getString($2);
                          builder->addString(s);
                          builder->updatePool(current);
                          current->getStatic("out");
                          current->loadStrConst(s);
                          current->callFunc("println", "(Ljava/lang/String;)V", FuncType::Virtual);
                        }
                        
    | PRINTLN INTEGER   { current->getStatic("out");
                          current->loadInt($2);
                          current->callFunc("println", "(I)V", FuncType::Virtual);
                        }
                        
    | PRINTLN FLOATL    { builder->addDouble($2);
                          builder->updatePool(current);
                          current->getStatic("out");
                          current->loadDoubleConst($2);
                          current->callFunc("println", "(D)V", FuncType::Virtual);
                        }
                        
    | PRINTLN ID        { current->getStatic("out");
                          DataType dt = vars[$2];
                          
                          if (dt == DataType::Int) {
                              current->loadIntVar($2);
                              current->callFunc("println", "(I)V", FuncType::Virtual);
                          } else if (dt == DataType::Double) {
                              current->loadDoubleVar($2);
                              current->callFunc("println", "(D)V", FuncType::Virtual);
                          }
                        }
    ;
    
int_dec:
    INT ID '=' INTEGER          { vars[$2] = DataType::Int;
                                  current->createIntVar($2, $4);
                                }
    ;
    
double_dec:
    DOUBLE ID '=' FLOATL        { vars[$2] = DataType::Double;
                                  builder->addDouble($4);
                                  builder->updatePool(current);
                                  current->createDoubleVar($2, $4);
                                }
    ;
    
var_assign:
      ID '=' math_expr      { currentType = vars[$1];
                              updateCodes();
                              
                              if (currentType == DataType::Int) {
                                  current->storeIntVar($1);
                              } else if (currentType == DataType::Double) {
                                  current->storeDoubleVar($1);
                              } else {
                                  puts("UNKNOWN VAR");
                              }
                            }
    | ID '=' INTEGER        { current->storeIntVar($1, $3); }
    | ID '=' FLOATL         { builder->addDouble($3);
                              builder->updatePool(current);
                              current->storeDoubleVar($1, $3); }
    ;
    
math_expr:
      math_expr '+' ld_expr   { current->addSingle(Add); }
    | math_expr '-' ld_expr   { current->addSingle(Sub); }
    | math_expr '*' ld_expr   { current->addSingle(Mul); }
    | math_expr '/' ld_expr   { current->addSingle(Div); }
    | ld_expr '+' ld_expr     { current->addSingle(Add); }
    | ld_expr '-' ld_expr     { current->addSingle(Sub); }
    | ld_expr '*' ld_expr     { current->addSingle(Mul); }
    | ld_expr '/' ld_expr     { current->addSingle(Div); }
    ;
    
ld_expr:
      INTEGER       { current->loadInt($1); }
    | FLOATL        { builder->addDouble($1);
                      builder->updatePool(current);
                      current->loadDoubleConst($1);
                    }
    | ID            { current->loadIntVar($1); }
    ;
    
end:
    END             { current->addSingle(JavaCode::RetVoid); }
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

//Update type-specific opcodes
void updateCodes() {
    if (currentType == DataType::Int) {
        Add = JavaCode::IAdd;
        Sub = JavaCode::ISub;
        Mul = JavaCode::IMul;
        Div = JavaCode::IDiv;
    } else if (currentType == DataType::Double) {
        Add = JavaCode::DAdd;
        Sub = JavaCode::DSub;
        Mul = JavaCode::DMul;
        Div = JavaCode::DDiv;
    } else {
    
    }
}

//Our parsing function
void parse(const char *path, JavaBuilder *b) {
    builder = b;
	yyin = fopen(path, "r");
	yyparse();
}

//Handle syntax errors
void yyerror(const char *s) {
	printf("Syntax error: %s\n", s);
}

int yywrap() { return 1; }

