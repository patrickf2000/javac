#include <javabuilder.hh>
#include <java_ast.hh>
#include <javafunc.hh>

//Builds a virtual method function call
void JavaBuilder::buildVirtualFC(JCode *code, JavaMethod *method, JavaFunc *target) {
	auto fc = static_cast<JVirtualFuncCall *>(code);
				
	if (fc->base != "")
		target->getStatic(fc->base);
		
	//Load arguments
	for (auto arg : fc->args) {
		switch (arg->type) {
			//Load a string
			case JCodeType::String: {
				auto str = static_cast<JString *>(arg);
				target->loadStrConst(str->val);
			} break;
			
			//TODO: Add the rest
		}
	}

	//Call the function
	target->callFunc(fc->name, fc->signature, FuncType::Virtual);
}
