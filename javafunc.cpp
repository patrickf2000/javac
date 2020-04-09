#include "javafunc.hh"

void JavaFunc::getStatic(std::string name) {
	short loco = (short)pool[name];
	loco -= 2;
	
	unsigned char buf[2];
	buf[0] = (loco & 0xff00) >> 8;
	buf[1] = (loco & 0x00ff);
	
	code.push_back(0xB2);
	code.push_back(buf[0]);
	code.push_back(buf[1]);
}

void JavaFunc::loadConst(std::string name) {
	int loco = pool[name] - 1;
	
	code.push_back(0x12);
	code.push_back((unsigned char)loco);
}

void JavaFunc::callFunc(std::string name, std::string type) {
	short loco = 0;

	for (auto func : refs) {
		if (func.name == name && func.type == type) {
			loco = func.pos - 1;
		}
	}
	
	unsigned char buf[2];
	buf[0] = (loco & 0xff00) >> 8;
	buf[1] = (loco & 0x00ff);
	
	code.push_back(0xB6);
	code.push_back(buf[0]);
	code.push_back(buf[1]);
}

void JavaFunc::loadInt(int val) {
	code.push_back(0x10);
	code.push_back((unsigned char)val);
}

void JavaFunc::retVoid() {
	code.push_back(0xB1);
}
