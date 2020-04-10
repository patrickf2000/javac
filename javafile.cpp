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
	labels = pool->pool;

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


