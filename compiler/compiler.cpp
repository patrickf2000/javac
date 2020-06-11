#include <iostream>
#include <cstdlib>
#include <map>

#include <javabuilder.hh>
#include <javafile.hh>
#include <javapool.hh>

#include <ast.hh>

JavaBuilder *builder;
JavaFunc *func;
std::map<std::string, DataType> vars;

void buildChildren(AstNode *parent) {
    for (auto child : parent->children) {
        switch (child->type) {
            //Println
            case AstType::Println: {
                auto arg = child->children[0];
                
                switch (arg->type) {
                    //Strings
                    case AstType::String: {
                        auto str = static_cast<AstString *>(arg);
                        builder->addString(str->val);
                        builder->updatePool(func);
                        
                        func->getStatic("out");
                        func->loadStrConst(str->val);
                        func->callFunc("println", "(Ljava/lang/String;)V", FuncType::Virtual);
                    } break;
                    
                    //Integers
                    case AstType::Int: {
                        auto i = static_cast<AstInt *>(arg);
                        
                        func->getStatic("out");
                        func->loadInt(i->val);
                        func->callFunc("println", "(I)V", FuncType::Virtual);
                    } break;
                    
                    //TODO: Add rest
                }
            } break;
            
            //Variable declaration
            case AstType::VarDec: {
                auto vd = static_cast<AstVarDec *>(child);
                vars[vd->name] = vd->dType;
                
                switch (vd->dType) {
                    case DataType::Int: {
                        func->createIntVar(vd->name, 0);
                    } break;
                    
                    //TODO: Add rest
                }
            } break;
            
            //Variable assignment
            case AstType::VarAssign: {
                auto va = static_cast<AstVarAssign *>(child);
                auto var = vars[va->name];
                
                switch (var) {
                    //Integer variables
                    case DataType::Int: {
                        for (auto arg : va->children) {
                            switch (arg->type) {
                                //Integers
                                case AstType::Int: {
                                    auto i = static_cast<AstInt *>(arg);
                                    func->storeIntVar(va->name, i->val);
                                } break;
                            }
                        }
                    } break;
                    
                    //TODO: Add rest
                }
            } break;
        
            //Return
            case AstType::End: func->addSingle(JavaCode::RetVoid); break;
            
            //TODO: Add rest
        }
    }
}

//The main AST translator
void translateAST(AstNode *tree) {
    builder = new JavaBuilder("Test1");
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

