#include "javafunc.hh"

//The constructor and setup functions
JavaFunc::JavaFunc(std::string name, std::string type) {
	this->name = name;
	this->type = type;
}
	
void JavaFunc::setPool(JavaPool *pool) {
	this->pool = pool->pool;
	refs = pool->funcRefs;
}

//Attributes
void JavaFunc::setAttributes(JFuncAttr attr) {
	attributes += (unsigned char)attr;
}

unsigned char JavaFunc::getAttributes() {
	return attributes;
}

//Load a static reference from the pool
void JavaFunc::getStatic(std::string name) {
	short loco = (short)pool[name];
	
	unsigned char buf[2];
	buf[0] = (loco & 0xff00) >> 8;
	buf[1] = (loco & 0x00ff);
	
	code.push_back(0xB2);
	code.push_back(buf[0]);
	code.push_back(buf[1]);
}

//Load a string constant to the stack
void JavaFunc::loadStrConst(std::string name) {
	int loco = pool[name];
	
	code.push_back(0x12);
	code.push_back((unsigned char)loco);
}

//Invoke a function
void JavaFunc::callFunc(std::string name, std::string type) {
	short loco = 0;

	for (auto func : refs) {
		if (func.name == name && func.type == type) {
			loco = func.pos;
		}
	}
	
	unsigned char buf[2];
	buf[0] = (loco & 0xff00) >> 8;
	buf[1] = (loco & 0x00ff);
	
	code.push_back(0xB6);
	code.push_back(buf[0]);
	code.push_back(buf[1]);
}

//Invoke a static method
void JavaFunc::callStaticFunc(std::string name, std::string type) {
	short loco = 0;

	for (auto func : refs) {
		if (func.name == name && func.type == type) {
			loco = func.pos;
		}
	}
	
	unsigned char buf[2];
	buf[0] = (loco & 0xff00) >> 8;
	buf[1] = (loco & 0x00ff);
	
	code.push_back(0xB8);
	code.push_back(buf[0]);
	code.push_back(buf[1]);
}

//Load an integer
void JavaFunc::loadInt(int val) {
	code.push_back(0x10);
	code.push_back((unsigned char)val);
}

//Return void
void JavaFunc::retVoid() {
	code.push_back(0xB1);
}
