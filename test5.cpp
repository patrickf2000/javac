#include <iostream>

#include <java_ast.hh>
#include <javabuilder.hh>
#include <javafile.hh>
#include <javapool.hh>

std::string str1 = "Hello World!";
std::string str2 = "Your number: ";

//Build the main method
JavaMain *buildMain() {
	auto method = new JavaMain;
	
	auto fc = new JVirtualFuncCall("out", "println", "(Ljava/lang/String;)V");
	fc->args.push_back(new JString(str1));
	method->code.push_back(fc);
	
	auto vd = new JVarDec("x", JDataType::Int);
	vd->val = new JInt(7);
	method->code.push_back(vd);
	
	auto fc2 = new JVirtualFuncCall("out", "println", "(I)V");
	fc2->args.push_back(new JVar("x", JDataType::Int));
	method->code.push_back(fc2);
	
	return method;
}

//Build the class
JavaClass *buildAst() {
	auto clazz = new JavaClass("Test5");
	clazz->methods.push_back(buildMain());
	
	clazz->strings.push_back(str1);
	clazz->strings.push_back(str2);
	
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
