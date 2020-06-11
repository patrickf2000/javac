#pragma once

#include <string>
#include <vector>

enum class AstType {
    None,
    
    //Function stuff
    FuncDec,
    Println,
    End,
    
    //Data types
    String
};

// Base Node
class AstNode {
public:
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

    std::string name = "";
};

//Println
class AstPrintln : public AstNode {
public:
    AstPrintln() {
        type = AstType::Println;
    }
};

//End of a function
class AstEnd : public AstNode {
public:
    AstEnd() {
        type = AstType::End;
    }
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

