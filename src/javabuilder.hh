#pragma once

#include <string>

#include "javafile.hh"
#include "javapool.hh"
#include "javafunc.hh"

class JavaBuilder {
public:
	explicit JavaBuilder(std::string name);
	void useOutput();
	void useLibrary(std::string str);
	void addString(std::string str);
	void write();
	
	JavaFunc *createConstructor(bool init=false);
	JavaFunc *createMain();
	
	JavaFile *jfile;
	JavaPool *jpool;
private:
	std::string class_name = "";
};
