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
	//pool->addString(str1);
	
	Ref ref;
	ref.base_lib = "java/lang/System";
	ref.name = "out";
	ref.type = "Ljava/io/PrintStream;";
	pool->addStaticRef(ref);
	
	ref.base_lib = "java/io/PrintStream";
	ref.name = "println";
	ref.type = "(Ljava/lang/String;)V";
	pool->useMethod(ref);
	
	ref.type = "(I)V";
	pool->useMethod(ref);
	
	pool->addAttribute("main");
	pool->addAttribute("([Ljava/lang/String;)V");
	pool->addAttribute("Code");
	
	/*writer.addReference("java/lang/System");
	writer.addReference("java/io/PrintStream");*/
	
	/*writer.addString("Hello World");
	writer.addString(str1);*/
	
	//Reference to the static out variable
	/*writer.addFieldRef("java/lang/System");
	writer.addNameType("out", "Ljava/io/PrintStream;");*/
	
	//Println function call
	/*writer.addMethodRef("java/io/PrintStream");
	writer.addNameType("println", "(Ljava/lang/String;)V");
	
	writer.addMethodRef("java/io/PrintStream");
	writer.addNameType("println", "(I)V");*/
	
	//Reference to our main function
	/*writer.addPoolAttr("main");
	writer.addPoolAttr("([Ljava/lang/String;)V");
	writer.addPoolAttr("Code");*/
	
	//Create the main func
	writer.labels = pool->pool;
	
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
	
	/*func->getStatic("out");
	func->loadConst(str1);
	func->callFunc("println", "(Ljava/lang/String;)V");*/
	
	func->getStatic("out");
	func->loadInt(10);
	func->callFunc("println", "(I)V");
	func->retVoid();
	
	writer.addFunc(func);
	
	writer.write(pool);

	return 0;
}
