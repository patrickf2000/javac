#pragma once

#include <string>
#include <vector>
#include <map>

#include "types.hh"
#include "javapool.hh"

enum class JFuncAttr {
	None = 0x00,
	Public = 0x01,
	Static = 0x08
};

enum class FuncType {
	None = 0x00,
	Virtual = 0xB6,
	Static = 0xB8
};

enum class JavaCode {
	None = 0x00,
	IAdd = 0x60,
	RetVoid = 0xB1
};

class JavaFunc {
public:
	JavaFunc(std::string name, std::string type);
	void setPool(JavaPool *pool);
	
	void setAttributes(JFuncAttr attr);
	unsigned char getAttributes();
	
	//Functions to add code
	void getStatic(std::string name);
	void loadStrConst(std::string name);
	void callFunc(std::string name, std::string type, FuncType ftype);
	void addSingle(JavaCode c);
	
	//Integer functions
	void loadInt(int val);
	void storeIntVar(std::string var, int val);
	void createIntVar(std::string var, int val);
	
	//The code section
	std::vector<unsigned char> code;
	int locals = 2;
	std::string name = "";
	std::string type = "";
private:
	//Attributes
	unsigned char attributes;
	
	//References to the string pool
	std::map<std::string, int> pool;
	std::vector<FuncRef> refs;
	
	//References to variables
	std::map<std::string, int> int_vars;
	int int_index = 1;
};
