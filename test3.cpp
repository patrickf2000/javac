#include <iostream>

#include <javabuilder.hh>
#include <javafile.hh>
#include <javapool.hh>

int main() {
	JavaBuilder *builder = new JavaBuilder("Test3");
	builder->useOutput();
	
	builder->useLibrary("java/util/Scanner");
	
	//Add field reference to system.in
	Ref ref;
	ref.base_lib = "java/lang/System";
	ref.name = "in";
	ref.type = "Ljava/io/InputStream;";
	builder->jpool->addStaticRef(ref);
	
	//Add method reference to constructor
	ref.base_lib = "java/util/Scanner";
	ref.name = "<init>";
	ref.type = "(Ljava/io/InputStream;)V";
	builder->jpool->useMethod(ref);
	
	//Add method reference to nextLine
	ref.base_lib = "java/util/Scanner";
	ref.name = "nextLine";
	ref.type = "()Ljava/lang/String;";
	builder->jpool->useMethod(ref);
	
	std::string str1 = "Hello World!";
	builder->addString(str1);
	
//===================================================================	
	//Create the constructor
	auto c = builder->createConstructor(true);
	
	c->getStatic("out");
	c->loadStrConst(str1);
	c->callFunc("println", "(Ljava/lang/String;)V", FuncType::Virtual);	
	c->addSingle(JavaCode::RetVoid);
	
	//Create the main func
	auto mainFunc = builder->createMain();
	
	mainFunc->getStatic("out");
	mainFunc->loadStrConst(str1);
	mainFunc->callFunc("println", "(Ljava/lang/String;)V", FuncType::Virtual);	
	
	mainFunc->initClass("Test3", "t");
	mainFunc->callFunc("Test3", "()V", FuncType::Special);
	
	mainFunc->initClass("java/util/Scanner", "scanner");
	mainFunc->getStatic("in");
	mainFunc->callFunc("<init>", "(Ljava/io/InputStream;)V", FuncType::Special);
	mainFunc->storeClassVar("scanner");
	
	mainFunc->loadClassVar("scanner");
	mainFunc->callFunc("nextLine", "()Ljava/lang/String;", FuncType::Virtual);
	
	mainFunc->addSingle(JavaCode::RetVoid);
	
	//Save
	builder->write();

	return 0;
}
