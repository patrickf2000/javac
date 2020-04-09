#pragma once

#include <string>
#include <vector>
#include <map>

#include "writer.hh"
#include "javafunc.hh"

enum class OpType {
	None = 0x00,
	Ref = 0x07,
	String = 0x08,
	FieldRef = 0x09,
	NameType = 0x0C,
	MethodRef = 0x0A
};

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
	
	void write();
private:
	BinWriter *writer;
	std::string class_name = "";
	std::vector<Constant> consts;
	std::vector<unsigned char> opcodes;
	std::vector<JavaFunc *> funcs;
	std::map<std::string, int> labels;
	int pool_size = 1;
};
