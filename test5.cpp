#include <iostream>

#include <java_ast.hh>
#include <javabuilder.hh>
#include <javafile.hh>
#include <javapool.hh>

//Build the main method
JavaMain *buildMain() {
	auto method = new JavaMain;
	return method;
}

//Build the class
JavaClass *buildAst() {
	auto clazz = new JavaClass("Test5");
	clazz->methods.push_back(buildMain());
	return clazz;
}

int main() {
	auto clazz = buildAst();

	//JavaBuilder
	JavaBuilder *builder = new JavaBuilder(clazz);
	builder->useOutput();
	
	//Save
	builder->write();

	return 0;
}
