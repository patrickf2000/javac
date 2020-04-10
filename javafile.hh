#pragma once

#include <string>
#include <vector>
#include <map>

#include "javapool.hh"
#include "writer.hh"
#include "javafunc.hh"
#include "types.hh"

struct Constant {
	std::string field;
	std::vector<unsigned char> padding;
	int pos;
	OpType type;
};

class JavaFile {
public:
	JavaFile(std::string cname);
	
	std::map<std::string, int> getPool() {
		return labels;
	}
	
	std::vector<FuncRef> getRefs() {
		return funcRefs;
	}
	
	void addString(std::string str);
	void addReference(std::string str);
	void addFieldRef(std::string name);
	void addNameType(std::string name, std::string type);
	void addMethodRef(std::string name);
	void addPoolAttr(std::string name);
	
	void addFunc(JavaFunc *fc) {
		funcs.push_back(fc);
	}
	
	void writeConsts();
	void writeFuncs();
	
	void write(JavaPool *pool);
	
	void printRefs();
	int pool_size = 1;
	std::map<std::string, int> labels;
private:
	BinWriter *writer;
	std::string class_name = "";
	std::vector<Constant> consts;
	std::vector<unsigned char> opcodes;
	std::vector<JavaFunc *> funcs;
	
	std::vector<FuncRef> funcRefs;
};
