#include <iostream>

#include <java_ast.hh>
#include <javabuilder.hh>
#include <javafile.hh>
#include <javapool.hh>

std::string str1 = "Hello World!";

//Build the main method
JavaMain *buildMain() {
	auto method = new JavaMain;
	
	auto fc = new JVirtualFuncCall("out", "println", "(Ljava/lang/String;)V");
	fc->args.push_back(new JString(str1));
	method->code.push_back(fc);
	
	return method;
}

//Build the class
JavaClass *buildAst() {
	auto clazz = new JavaClass("Test5");
	clazz->strings.push_back(str1);
	clazz->methods.push_back(buildMain());
	return clazz;
}

int main() {
	auto clazz = buildAst();

	//JavaBuilder
	JavaBuilder *builder = new JavaBuilder(clazz);
	builder->useOutput();
	builder->assemble(clazz);
	
	//Save
	builder->write();

	return 0;
}
