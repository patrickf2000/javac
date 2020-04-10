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

class JavaFunc {
public:
	JavaFunc(std::string name, std::string type);
	void setPool(JavaPool *pool);
	
	void setAttributes(JFuncAttr attr);
	unsigned char getAttributes();
	
	//Functions to add code
	void getStatic(std::string name);
	void loadStrConst(std::string name);
	void callFunc(std::string name, std::string type);
	void loadInt(int val);
	void retVoid();
	
	//The code section
	std::vector<unsigned char> code;
	std::string name = "";
	std::string type = "";
private:
	//Attributes
	unsigned char attributes;
	
	//References to the string pool
	std::map<std::string, int> pool;
	std::vector<FuncRef> refs;
};
