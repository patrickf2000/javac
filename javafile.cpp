#include <iostream>

#include "javafile.hh"

//Create the java file and write some initial information
JavaFile::JavaFile(std::string cname) {
	class_name = cname;
	writer = new BinWriter(class_name + ".class");
	
	//Write the magic number
	writer->write_opcode(0xCA);
	writer->write_opcode(0xFE);
	writer->write_opcode(0xBA);
	writer->write_opcode(0xBE);
	
	//Write the version number
	writer->write_short(0x00);
	writer->write_opcode(0x00);
	writer->write_opcode(0x34);
	
	//Add the class name to the string pool
	//addReference(class_name);
	
	//Add the super class
	//addReference("java/lang/Object");
}

//Adds a string to the pool
void JavaFile::addString(std::string str) {
	++pool_size;

	Constant c;
	c.field = str;
	c.pos = pool_size;
	c.type = OpType::String;
	
	c.padding.push_back(0x01);
	c.padding.push_back(0x00);
	
	labels[str] = pool_size;

	++pool_size;
	consts.push_back(c);
}

//Adds a library reference
void JavaFile::addReference(std::string str) {
	++pool_size;

	Constant c;
	c.field = str;
	c.pos = pool_size;
	c.type = OpType::Ref;
	
	c.padding.push_back(0x01);
	c.padding.push_back(0x00);
	
	labels[str] = pool_size;

	++pool_size;
	consts.push_back(c);
}

//Adds a field reference
void JavaFile::addFieldRef(std::string name) {
	++pool_size;
	
	Constant c;
	c.field = "";
	c.pos = -1;
	c.type = OpType::FieldRef;
	
	//Look up the index of the name
	int index = 0;
	
	for (auto c : consts) {
		if (c.field == name) {
			index = c.pos;
			break;
		}
	}
	
	c.padding.push_back((unsigned char)index-1);
	c.padding.push_back(0x00);
	c.padding.push_back((unsigned char)pool_size);
	
	++pool_size;
	consts.push_back(c);
}

//Adds a name and type reference
void JavaFile::addNameType(std::string name, std::string type) {
	Constant c;
	c.field = "";
	c.pos = -1;
	c.type = OpType::NameType;
	
	FuncRef ref;
	ref.pos = pool_size - 1;
	ref.type = type;
	ref.name = name;
	funcRefs.push_back(ref);
	
	c.padding.push_back((unsigned char)pool_size);
	c.padding.push_back(0x00);
	c.padding.push_back((unsigned char)pool_size + 1);
	
	labels[name] = pool_size;
	labels[type] = pool_size + 1;
	
	pool_size += 2;
	consts.push_back(c);
	
	c.type = OpType::None;
	c.padding.clear();
	c.padding.push_back(0x01);
	c.padding.push_back(0x00);
	
	c.field = name;
	consts.push_back(c);
	
	c.field = type;
	consts.push_back(c);
}

//Adds a method reference
void JavaFile::addMethodRef(std::string name) {
	++pool_size;
	
	Constant c;
	c.field = "";
	c.pos = -1;
	c.type = OpType::MethodRef;
	
	//Look up the index of the name
	int index = 0;
	
	for (auto c : consts) {
		if (c.field == name) {
			index = c.pos;
			break;
		}
	}
	
	c.padding.push_back((unsigned char)index-1);
	c.padding.push_back(0x00);
	c.padding.push_back((unsigned char)pool_size);
	
	labels[name] = pool_size;
	
	++pool_size;
	consts.push_back(c);
}

//Adds an attribute
void JavaFile::addPoolAttr(std::string name) {
	Constant c;
	c.field = name;
	c.pos = -1;
	c.type = OpType::None;
	
	c.padding.push_back(0x01);
	c.padding.push_back(0x00);
	
	labels[name] = pool_size;

	++pool_size;
	consts.push_back(c);
}

//Write all constants to our file
void JavaFile::writeConsts() {
	for (auto c : consts) {
		if (c.type != OpType::None) {
			writer->write_opcode((unsigned char)c.type);
			writer->write_opcode(0x00);
		}
		
		if (c.pos != -1)
			writer->write_opcode((unsigned char)c.pos);
		
		for (auto c : c.padding)
			writer->write_opcode(c);
		
		if (c.field.size() > 0)
			writer->write_str(c.field.c_str());
	}
}

//Write all functions to our file
void JavaFile::writeFuncs() {
	for (auto func : funcs) {
		writer->write_opcode(0x00);
		writer->write_opcode(func->getAttributes());
		
		//Find the indexes
		int name_index = labels[func->name];
		int type_index = labels[func->type];
		
		//Write the name
		writer->write_opcode(0x00);
		writer->write_opcode((unsigned char)name_index);
		
		//Write the type
		writer->write_opcode(0x00);
		writer->write_opcode((unsigned char)type_index);
		
		//Attribute
		writer->write_opcode(0x00);
		writer->write_opcode(0x01);
		
		//Get the location of the code section in the stack
		int code_index = labels["Code"];
		writer->write_opcode(0x00);
		writer->write_opcode((unsigned char)code_index);
		
		//Code attribute size
		writer->write_int(func->code.size()+12);
		
		//Stack size 2, local var size 1
		writer->write_opcode(0x00);
		writer->write_opcode(0x02);
		writer->write_opcode(0x00);
		writer->write_opcode(0x01);
		
		//Write the code size (is an int)
		writer->write_int(func->code.size());
		
		//Write the code
		for (auto op : func->code)
			writer->write_opcode(op);
		
		//Padding
		//This goes at end
		writer->write_opcode(0x00);
		writer->write_opcode(0x00);
	}
}

//Close and do the final writing
void JavaFile::write(JavaPool *pool) {
	std::cout << "Size: " << pool->getPoolSize() << std::endl;
	//Write the constant pool size
	writer->write_opcode(0x00);
	writer->write_opcode((unsigned char)pool->getPoolSize());
	
	//Write the constant pool
	for (auto c : pool->getCode()) {
		writer->write_opcode(c);
	}
	//writeConsts();
		
	//Access modifier
	writer->write_opcode(0x00);
	writer->write_opcode(0x21);
	
	//Index of class in constant pool
	writer->write_opcode(0x00);
	writer->write_opcode(0x01);
	
	//Index of super class in constant pool
	writer->write_opcode(0x00);
	writer->write_opcode(0x03);
	
	//Zero interfaces
	writer->write_short(0);
	
	//Zero fields
	writer->write_short(0);
	
	//Number of methods
	writer->write_opcode(0x00);
	writer->write_opcode(0x01);
	
	//Write the functions
	writeFuncs();
		
	//EOF padding
	writer->write_int(0);
		
	writer->write();
}

//TODO: Delete this
void JavaFile::printRefs() {
	for (auto ref : funcRefs) {
		std::cout << "[" << ref.pos << "] " << ref.name << " -> " << ref.type << std::endl;
	}
}

