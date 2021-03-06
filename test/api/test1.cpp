#include <iostream>

#include <javafile.hh>
#include <javapool.hh>

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
	
	ref.base_lib = "HelloWorld";
	ref.name = "printNum";
	ref.type = "()V";
	pool->useMethod(ref, true);
	
	//Add our main function
	pool->addAttribute("main");
	pool->addAttribute("([Ljava/lang/String;)V");
	
	//Add another function
	pool->addAttribute("sayHello");
	pool->addAttribute("()V");
	
	pool->addAttribute("printNum");
	
	//Attributes
	pool->addAttribute("Code");
	pool->addAttribute("LineNumberTable");
	
//===================================================================	
	//Add a function
	auto func2 = new JavaFunc("sayHello", "()V");
	func2->setPool(pool);
	func2->setAttributes(JFuncAttr::Public);
	func2->setAttributes(JFuncAttr::Static);
	
	func2->getStatic("out");
	func2->loadStrConst("Hello World");
	func2->callFunc("println", "(Ljava/lang/String;)V", FuncType::Virtual);
	func2->addSingle(JavaCode::RetVoid);
	
	writer.addFunc(func2);
	
	//Add another function
	auto func3 = new JavaFunc("printNum", "()V");
	func3->setPool(pool);
	func3->setAttributes(JFuncAttr::Public);
	func3->setAttributes(JFuncAttr::Static);
	
	func3->getStatic("out");
	func3->loadInt(100);
	func3->callFunc("println", "(I)V", FuncType::Virtual);
	func3->addSingle(JavaCode::RetVoid);
	
	writer.addFunc(func3);
	
	//Create the main func
	//We want to print a message and an integer
	auto func = new JavaFunc("main", "([Ljava/lang/String;)V");
	func->setPool(pool);
	func->setAttributes(JFuncAttr::Public);
	func->setAttributes(JFuncAttr::Static);
	
	func->getStatic("out");
	func->loadStrConst("Hello World");
	func->callFunc("println", "(Ljava/lang/String;)V", FuncType::Virtual);
	
	func->getStatic("out");
	func->loadInt(10);
	func->loadInt(32);
	func->addSingle(JavaCode::IAdd);
	func->callFunc("println", "(I)V", FuncType::Virtual);
	
	func->callFunc("sayHello", "()V", FuncType::Static);
	func->callFunc("printNum", "()V", FuncType::Static);
	func->addSingle(JavaCode::RetVoid);
	
	writer.addFunc(func);
	
	//Write it all out
	writer.write(pool);

	return 0;
}
