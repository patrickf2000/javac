#include <iostream>
#include <cstdlib>

#include <javabuilder.hh>
#include <javafile.hh>
#include <javapool.hh>

#include <ast.hh>

JavaBuilder *builder;
JavaFunc *func;

void buildChildren(AstNode *parent) {
    for (auto child : parent->children) {
        switch (child->type) {
            //Return
            case AstType::End: func->addSingle(JavaCode::RetVoid); break;
            
            //TODO: Add rest
        }
    }
}

//The main AST translator
void translateAST(AstNode *tree) {
    auto *builder = new JavaBuilder("Test1");
    builder->useOutput();
    
    for (auto child : tree->children) {
        if (child->type != AstType::FuncDec) {
            std::cout << "Fatal: Expected function declaration." << std::endl;
            std::exit(1);
        }
        
        auto fd = static_cast<AstFuncDec *>(child);
        
        if (fd->name == "main") {
            func = builder->createMain();
        } else {
            //TODO: Non-main function
        }
        
        buildChildren(child);
    }
    
    builder->write();
}
