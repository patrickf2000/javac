%{
#include <stdio.h>
#include <string>

#include "parser.yy.hpp"
%}

%%
#.*							{}
func                        { return FUNC; }
println                     { return PRINTLN; }
end                         { return END; }
int                         { return INT; }
"+"                         { return '+'; }
"="                         { return '='; }
\"(\\.|[^"\\])*\" 			{yylval.stype = strdup(yytext); return STRING;}
[ \t]                       {}
\n                          {}
[-]?[0-9]+					{yylval.itype = std::stoi(yytext); return INTEGER;}
[-]?([0-9]*[.])?[0-9]+		{yylval.ftype = std::stof(yytext); return FLOAT;}
[A-Za-z]*					{yylval.stype = strdup(yytext); return ID;}
%%

int yywrap() { return 1; }
