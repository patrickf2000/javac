#pragma once

#include <string>
#include <vector>
#include <map>

#include "types.hh"

enum class JFuncAttr {
	None = 0x00,
	Public = 0x01,
	Static = 0x08
};

class JavaFunc {
public:
	JavaFunc() {}
	void setPool(std::map<std::string, int> p) {
		pool = p;
	}
	
	void setRefs(std::vector<FuncRef> r) {
		refs = r;
	}
	
	std::string name = "";
	std::string type = "";
	
	//Attributes
	void setAttributes(JFuncAttr attr) {
		attributes += (unsigned char)attr;
	}
	
	unsigned char getAttributes() {
		return attributes;
	}
	
	//The code section
	std::vector<unsigned char> code;
	
	//Functions to add code
	void getStatic(std::string name);
	void loadConst(std::string name);
	void callFunc(std::string name, std::string type);
	void loadInt(int val);
	void retVoid();
private:
	unsigned char attributes;
	std::map<std::string, int> pool;
	std::vector<FuncRef> refs;
};
