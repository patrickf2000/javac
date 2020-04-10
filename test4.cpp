#include <iostream>

#include <javabuilder.hh>
#include <javafile.hh>
#include <javapool.hh>

int main() {
	JavaBuilder *builder = new JavaBuilder("Test4");
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
	ref.name = "nextInt";
	ref.type = "()I";
	builder->jpool->useMethod(ref);
	
	std::string str1 = "Enter a number:";
	builder->addString(str1);
	
	std::string str2 = "Enter another number:";
	builder->addString(str2);
	
//===================================================================		
	//Create the main func
	auto mainFunc = builder->createMain();
	
	mainFunc->initClass("java/util/Scanner", "scanner");
	mainFunc->getStatic("in");
	mainFunc->callFunc("<init>", "(Ljava/io/InputStream;)V", FuncType::Special);
	mainFunc->storeClassVar("scanner");
	
	mainFunc->createIntVar("x", 0);
	//mainFunc->createIntVar("y", 0);
	//mainFunc->createIntVar("answer", 0);
	
	mainFunc->getStatic("out");
	mainFunc->loadStrConst(str1);
	mainFunc->callFunc("println", "(Ljava/lang/String;)V", FuncType::Virtual);
	
	mainFunc->loadClassVar("scanner");
	mainFunc->callFunc("nextInt", "()I", FuncType::Virtual);
	
	mainFunc->storeIntVar("x");
	
	/*mainFunc->getStatic("out");
	mainFunc->loadStrConst(str2);
	mainFunc->callFunc("println", "(Ljava/lang/String;)V", FuncType::Virtual);
	
	mainFunc->loadClassVar("scanner");
	mainFunc->callFunc("nextInt", "()I", FuncType::Virtual);
	
	mainFunc->storeIntVar("y");*/
	
	mainFunc->addSingle(JavaCode::RetVoid);
	
	//Save
	builder->write();

	return 0;
}
