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

//End of a function
class AstEnd : public AstNode {
public:
    AstEnd() {
        type = AstType::End;
    }
};
