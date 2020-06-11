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
	Special = 0xB7,
	Static = 0xB8
};

enum class JavaCode {
	None = 0x00,
	ALoad0 = 0x2A,
	IAdd = 0x60,
	ISub = 0x64,
	IMul = 0x68,
	IDiv = 0x6C,
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
	void loadDoubleConst(double d);
	void callFunc(std::string name, std::string type, FuncType ftype);
	void addSingle(JavaCode c);
	
	//Class functions
	void initClass(std::string name, std::string v_name);
	void storeClassVar(std::string name);
	void loadClassVar(std::string name);
	
	//Integer functions
	void loadInt(int val);
	void loadIntVar(std::string var);
	void storeIntVar(std::string var);
	void storeIntVar(std::string var, int val);
	void createIntVar(std::string var, int val);
	
	//Double functions
	void loadDouble(double val);
	void loadDoubleVar(std::string var);
	void storeDoubleVar(std::string var);
	void storeDoubleVar(std::string var, double val);
	void createDoubleVar(std::string var, double val);
	
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
	
	//Variable control
	std::map<std::string, int> obj_vars;
	std::map<std::string, int> int_vars;
	std::map<std::string, int> dbl_vars;
	int var_index = 1;
};
