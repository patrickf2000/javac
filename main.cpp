#include <iostream>

#include "javafile.hh"

int main() {
	JavaFile writer("HelloWorld");
	
	writer.addReference("java/lang/System");
	writer.addReference("java/io/PrintStream");
	
	std::string str1 = "Java shell";
	
	writer.addString("Hello World");
	writer.addString(str1);
	
	//Reference to the static out variable
	writer.addFieldRef("java/lang/System");
	writer.addNameType("out", "Ljava/io/PrintStream;");
	
	//Println function call
	writer.addMethodRef("java/io/PrintStream");
	writer.addNameType("println", "(Ljava/lang/String;)V");
	
	writer.addMethodRef("java/io/PrintStream");
	writer.addNameType("println", "(I)V");
	
	//Reference to our main function
	writer.addPoolAttr("main");
	writer.addPoolAttr("([Ljava/lang/String;)V");
	writer.addPoolAttr("Code");
	
	//Create the main func
	auto func = new JavaFunc();
	func->setPool(writer.getPool());
	func->setRefs(writer.getRefs());
	func->setAttributes(JFuncAttr::Public);
	func->setAttributes(JFuncAttr::Static);
	
	func->name = "main";
	func->type = "([Ljava/lang/String;)V";
	
	func->getStatic("out");
	func->loadConst("Hello World");
	func->callFunc("println", "(Ljava/lang/String;)V");
	
	func->getStatic("out");
	func->loadConst(str1);
	func->callFunc("println", "(Ljava/lang/String;)V");
	
	func->getStatic("out");
	func->loadInt(10);
	func->callFunc("println", "(I)V");
	func->retVoid();
	
	writer.addFunc(func);
	
	writer.write();

	return 0;
}
