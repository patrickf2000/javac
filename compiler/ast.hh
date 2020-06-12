#pragma once

#include <string>
#include <vector>

enum class AstType {
    None,
    
    //Function stuff
    FuncDec,
    Println,
    FuncCall,
    End,
    
    //Variable stuff
    VarDec,
    VarAssign,
    
    //Comparison stuff
    If,
    Elif,
    Else,
    
    //Operators
    Add,
    Sub,
    Mul,
    Div,
    
    //Comparison operators
    Equal,
    NotEqual,
    Greater,
    Less,
    
    //Data types
    Int,
    Float,
    String,
    Id
};

enum class DataType {
    None,
    Int,
    Double
};

// Base Node
class AstNode {
public:
    explicit AstNode() { }
    explicit AstNode(AstType t) {
        type = t;
    }
    
    AstType type = AstType::None;
    std::vector<AstNode *> children;
};

//Function declaration
class AstFuncDec : public AstNode {
public:
    explicit AstFuncDec(std::string n) {
        type = AstType::FuncDec;
        name = n;
    }

    std::string sig = "()V";
    std::string name = "";
    std::vector<std::string> args;
};

//Println
class AstPrintln : public AstNode {
public:
    AstPrintln() {
        type = AstType::Println;
    }
};

//Function call
class AstFuncCall : public AstNode {
public:
    explicit AstFuncCall(std::string name) {
        type = AstType::FuncCall;
        this->name = name;
    }
    
    std::string name = "";
};

//End of a function
class AstEnd : public AstNode {
public:
    AstEnd() {
        type = AstType::End;
    }
};

//Variable declarations
class AstVarDec : public AstNode {
public:
    explicit AstVarDec(std::string name, DataType t) {
        type = AstType::VarDec;
        dType = t;
        this->name = name;
    }
    
    DataType dType = DataType::None;
    std::string name = "";
};

//Variable assignments
class AstVarAssign : public AstNode {
public:
    explicit AstVarAssign(std::string name) {
        type = AstType::VarAssign;
        this->name = name;
    }
    
    std::string name = "";
};

//Integers
class AstInt : public AstNode {
public:
    explicit AstInt(int i) {
        type = AstType::Int;
        val = i;
    }
    
    int val = 0;
};

//Float variables
class AstFloat : public AstNode {
public:
    explicit AstFloat(float i) {
        type = AstType::Float;
        val = i;
    }
    
    float val = 0;
};

//Strings
class AstString : public AstNode {
public:
    explicit AstString(std::string s) {
        type = AstType::String;
        val = s;
    }
    
    std::string val = "";
};

//ID-> Variables
class AstId : public AstNode {
public:
    explicit AstId(std::string name) {
        type = AstType::Id;
        this->name = name;
    }
    
    std::string name = "";
};

