#pragma once

#include <string>

#include "javafile.hh"
#include "javapool.hh"
#include "javafunc.hh"

class JavaBuilder {
public:
	explicit JavaBuilder(std::string name);
	void useOutput();
	void addString(std::string str);
	void write();
	
	JavaFunc *createMain();
private:
	JavaFile *jfile;
	JavaPool *jpool;
};
