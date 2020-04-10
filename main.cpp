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
	
	//Add reference to our custom function
	ref.base_lib = "HelloWorld";
	ref.name = "sayHello";
	ref.type = "()V";
	pool->useMethod(ref, true);
	
	//Add our main function
	pool->addAttribute("main");
	pool->addAttribute("([Ljava/lang/String;)V");
	
	//Add another function
	pool->addAttribute("sayHello");
	pool->addAttribute("()V");
	
	pool->addAttribute("Code");
	pool->addAttribute("LineNumberTable");
	
	//Add a function
	auto func2 = new JavaFunc("sayHello", "()V");
	func2->setPool(pool);
	func2->setAttributes(JFuncAttr::Public);
	func2->setAttributes(JFuncAttr::Static);
	
	func2->getStatic("out");
	func2->loadStrConst("Hello World");
	func2->callFunc("println", "(Ljava/lang/String;)V");
	func2->retVoid();
	
	writer.addFunc(func2);
	
	//Create the main func
	//We want to print a message and an integer
	auto func = new JavaFunc("main", "([Ljava/lang/String;)V");
	func->setPool(pool);
	func->setAttributes(JFuncAttr::Public);
	func->setAttributes(JFuncAttr::Static);
	
	func->getStatic("out");
	func->loadStrConst("Hello World");
	func->callFunc("println", "(Ljava/lang/String;)V");
	
	func->getStatic("out");
	func->loadInt(10);
	func->callFunc("println", "(I)V");
	
	func->callStaticFunc("sayHello", "()V");
	func->retVoid();
	
	writer.addFunc(func);
	
	//Write it all out
	writer.write(pool);

	return 0;
}
