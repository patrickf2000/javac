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
	
	//Create the main func
	auto mainFunc = builder->createMain();
	
	mainFunc->getStatic("out");
	mainFunc->loadStrConst(str1);
	mainFunc->callFunc("println", "(Ljava/lang/String;)V", FuncType::Virtual);
	
	mainFunc->addSingle(JavaCode::RetVoid);
	
	//Save
	builder->write();

	return 0;
}
