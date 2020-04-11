#pragma once

#include <string>

#include "java_ast.hh"
#include "javafile.hh"
#include "javapool.hh"
#include "javafunc.hh"

class JavaBuilder {
public:
	explicit JavaBuilder(std::string name);
	explicit JavaBuilder(JavaClass *clazz);
	
	void useOutput();
	void useLibrary(std::string str);
	void addString(std::string str);
	void write();
	
	JavaFunc *createConstructor(bool init=false);
	JavaFunc *createMain();
	
	JavaFile *jfile;
	JavaPool *jpool;
protected:
	void init(std::string name);
	void assemble(JavaClass *clazz);
	void buildMethod(JavaMethod *method, JavaFunc *target);
private:
	std::string class_name = "";
};
