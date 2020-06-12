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
                    
                    //Floats
                    case AstType::Float: {
                        auto flt = static_cast<AstFloat *>(arg);
                        builder->addDouble(flt->val);
                        builder->updatePool(func);
                        
                        func->getStatic("out");
                        func->loadDoubleConst(flt->val);
                        func->callFunc("println", "(D)V", FuncType::Virtual);
                    } break;
                    
                    //Integers
                    case AstType::Int: {
                        auto i = static_cast<AstInt *>(arg);
                        
                        func->getStatic("out");
                        func->loadInt(i->val);
                        func->callFunc("println", "(I)V", FuncType::Virtual);
                    } break;
                    
                    //IDs
                    case AstType::Id: {
                        auto id = static_cast<AstId *>(arg);
                        auto dType = vars[id->name];
                        
                        func->getStatic("out");
                        
                        switch (dType) {
                            case DataType::Int: {
                                func->loadIntVar(id->name);
                                func->callFunc("println", "(I)V", FuncType::Virtual);
                            } break;
                            
                            case DataType::Double: {
                                func->loadDoubleVar(id->name);
                                func->callFunc("println", "(D)V", FuncType::Virtual);
                            } break;
                        }
                    } break;
                }
            } break;
            
            //Variable declaration
            case AstType::VarDec: {
                auto vd = static_cast<AstVarDec *>(child);
                vars[vd->name] = vd->dType;
                
                switch (vd->dType) {
                    case DataType::Int: {
                        auto i = static_cast<AstInt *>(vd->children[0]);
                        func->createIntVar(vd->name, i->val);
                    } break;
                    
                    case DataType::Double: {
                        auto flt = static_cast<AstFloat *>(vd->children[0]);
                        double val = flt->val;
                    
                        builder->addDouble(val);
                        builder->updatePool(func);
                        func->createDoubleVar(vd->name, val);
                    } break;
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
                                    func->loadInt(i->val);
                                } break;
                                
                                //Variables
                                case AstType::Id: {
                                    auto id = static_cast<AstId *>(arg);
                                    func->loadIntVar(id->name);
                                } break;
                                
                                //Operators
                                case AstType::Add: func->addSingle(JavaCode::IAdd); break;
                                case AstType::Sub: func->addSingle(JavaCode::ISub); break;
                                case AstType::Mul: func->addSingle(JavaCode::IMul); break;
                                case AstType::Div: func->addSingle(JavaCode::IDiv); break;
                            }
                        }
                        
                        func->storeIntVar(va->name);
                    } break;
                    
                    //Double variables
                    case DataType::Double: {
                        for (auto arg : va->children) {
                            switch (arg->type) {
                                //Integers
                                case AstType::Int: {
                                    auto i = static_cast<AstInt *>(arg);
                                    double flt = (double)i->val;
                                    builder->addDouble(flt);
                                    builder->updatePool(func);
                                    func->loadDoubleConst(flt);
                                } break;
                            
                                //Float literals
                                case AstType::Float: {
                                    auto flt = static_cast<AstFloat *>(arg);
                                    builder->addDouble(flt->val);
                                    builder->updatePool(func);
                                    func->loadDoubleConst(flt->val);
                                } break;
                                
                                //Variables
                                case AstType::Id: {
                                    auto id = static_cast<AstId *>(arg);
                                    func->loadDoubleVar(id->name);
                                } break;
                                
                                //Operators
                                case AstType::Add: func->addSingle(JavaCode::DAdd); break;
                                case AstType::Sub: func->addSingle(JavaCode::DSub); break;
                                case AstType::Mul: func->addSingle(JavaCode::DMul); break;
                                case AstType::Div: func->addSingle(JavaCode::DDiv); break;
                            }
                        }
                        
                        func->storeDoubleVar(va->name);
                    } break;
                }
            } break;
        
            //Return
            case AstType::End: func->addSingle(JavaCode::RetVoid); break;
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

