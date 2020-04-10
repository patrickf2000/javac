#include "javabuilder.hh"

//Sets everything up
JavaBuilder::JavaBuilder(std::string name) {
	jfile = new JavaFile(name);
	jpool = new JavaPool(name);
	
	//Required attributes
	jpool->addAttribute("Code");
	jpool->addAttribute("LineNumberTable");
	
	//Reference to the main function
	jpool->addAttribute("main");
	jpool->addAttribute("([Ljava/lang/String;)V");
}

//Adds the common System.out.println libraries to the pool
// and enables their use
void JavaBuilder::useOutput() {
	jpool->useLibrary("java/lang/System");
	jpool->useLibrary("java/io/PrintStream");
	
	//Add a reference to the System.out static variable
	Ref ref;
	ref.base_lib = "java/lang/System";
	ref.name = "out";
	ref.type = "Ljava/io/PrintStream;";
	jpool->addStaticRef(ref);
	
	//Use println to print strings
	ref.base_lib = "java/io/PrintStream";
	ref.name = "println";
	ref.type = "(Ljava/lang/String;)V";
	jpool->useMethod(ref);
	
	//Use println to print ints
	ref.type = "(I)V";
	jpool->useMethod(ref);
}

//Adds a string to the constant pool and enables its use
void JavaBuilder::addString(std::string str) {
	jpool->addString(str);
}

//Writes everything to a class file
void JavaBuilder::write() {
	jfile->write(jpool);
}

//Creates the main function and returns a reference to it
JavaFunc *JavaBuilder::createMain() {
	auto func = new JavaFunc("main", "([Ljava/lang/String;)V");
	func->setPool(jpool);
	func->setAttributes(JFuncAttr::Public);
	func->setAttributes(JFuncAttr::Static);
	
	jfile->addFunc(func);
	return func;
}