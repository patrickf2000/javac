#include <iostream>

#include <javabuilder.hh>
#include <javafile.hh>
#include <javapool.hh>

int main() {
	JavaBuilder *builder = new JavaBuilder("Test2");
	builder->useOutput();
	
	std::string str1 = "Hello World!";
	builder->addString(str1);
	
//===================================================================	
	//Create the constructor
	auto c = builder->createConstructor();
	c->addSingle(JavaCode::RetVoid);
	
	//Create the main func
	auto mainFunc = builder->createMain();
	
	mainFunc->getStatic("out");
	mainFunc->loadStrConst(str1);
	mainFunc->callFunc("println", "(Ljava/lang/String;)V", FuncType::Virtual);
	
	mainFunc->createIntVar("n1", 1);
	mainFunc->createIntVar("n2", 2);
	mainFunc->createIntVar("n3", 3);
	mainFunc->createIntVar("n4", 4);
	mainFunc->createIntVar("n5", 5);
	
	mainFunc->loadIntVar("n2");
	mainFunc->loadIntVar("n4");
	mainFunc->addSingle(JavaCode::IMul);
	mainFunc->storeIntVar("n5");
	
	mainFunc->getStatic("out");
	mainFunc->loadIntVar("n5");
	mainFunc->callFunc("println", "(I)V", FuncType::Virtual);
	
	mainFunc->addSingle(JavaCode::RetVoid);
	
	//Save
	builder->write();

	return 0;
}
