#include <iostream>

#include "javafile.hh"
#include "javapool.hh"

int main() {
	JavaFile writer("HelloWorld");
	JavaPool *pool = new JavaPool("HelloWorld");
	
	std::string str1 = "Java shell";
	
	pool->useLibrary("java/lang/System");
	pool->useLibrary("java/io/PrintStream");
	
	pool->addString("Hello World");
	
	//Add a reference to the System.out static variable
	Ref ref;
	ref.base_lib = "java/lang/System";
	ref.name = "out";
	ref.type = "Ljava/io/PrintStream;";
	pool->addStaticRef(ref);
	
	//Use println to print strings
	ref.base_lib = "java/io/PrintStream";
	ref.name = "println";
	ref.type = "(Ljava/lang/String;)V";
	pool->useMethod(ref);
	
	//Use println to print ints
	ref.type = "(I)V";
	pool->useMethod(ref);
	
	//Add our main function
	pool->addAttribute("main");
	pool->addAttribute("([Ljava/lang/String;)V");
	pool->addAttribute("Code");
	
	//Create the main func
	//We want to print a message and an integer
	auto func = new JavaFunc();
	func->setPool(pool->pool);
	func->setRefs(pool->funcRefs);
	func->setAttributes(JFuncAttr::Public);
	func->setAttributes(JFuncAttr::Static);
	
	func->name = "main";
	func->type = "([Ljava/lang/String;)V";
	
	func->getStatic("out");
	func->loadConst("Hello World");
	func->callFunc("println", "(Ljava/lang/String;)V");
	
	func->getStatic("out");
	func->loadInt(10);
	func->callFunc("println", "(I)V");
	func->retVoid();
	
	writer.addFunc(func);
	
	//Write it all out
	writer.write(pool);

	return 0;
}
