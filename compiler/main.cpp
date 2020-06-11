#include <iostream>

#include <javabuilder.hh>
#include <javafile.hh>
#include <javapool.hh>

#include <ast.hh>

extern void parse(const char *path, AstNode *t);
void translateAST(AstNode *tree);

int main(int argc, char *argv[]) {
    AstNode *tree = new AstNode;

	if (argc > 1) {
		parse(argv[1], tree);
	} else {
		parse("", tree);
	}
	
	translateAST(tree);
	
	return 0;
}
