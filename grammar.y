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

int yylex();
void yyerror(const char *s);

//Needed for variable control
enum class DataType {
    None,
    Int,
    Double
};

std::map<std::string, DataType> vars;
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

%left '+' '-' '*' '/'
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
      ID '=' math_expr      { current->storeIntVar($1); }
    | ID '=' INTEGER        { current->storeIntVar($1, $3); }
    ;
    
math_expr:
      math_expr '+' ld_expr   { current->addSingle(JavaCode::IAdd); }
    | math_expr '-' ld_expr   { current->addSingle(JavaCode::ISub); }
    | math_expr '*' ld_expr   { current->addSingle(JavaCode::IMul); }
    | math_expr '/' ld_expr   { current->addSingle(JavaCode::IDiv); }
    | ld_expr '+' ld_expr     { current->addSingle(JavaCode::IAdd); }
    | ld_expr '-' ld_expr     { current->addSingle(JavaCode::ISub); }
    | ld_expr '*' ld_expr     { current->addSingle(JavaCode::IMul); }
    | ld_expr '/' ld_expr     { current->addSingle(JavaCode::IDiv); }
    ;
    
ld_expr:
      INTEGER       { current->loadInt($1); }
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

