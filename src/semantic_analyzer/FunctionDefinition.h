#ifndef _FUNC_DECL_NODE_H_
#define _FUNC_DECL_NODE_H_

#include <memory>
#include <string>

#include "BasicType.h"
#include "TypeSpecifier.h"

class SymbolTable;

namespace semantic_analyzer {

class FunctionDeclaration;
class TerminalSymbol;

class FunctionDefinition: public AbstractSyntaxTreeNode {
public:
    FunctionDefinition(TypeSpecifier returnType, std::unique_ptr<FunctionDeclaration> declaration,
            std::unique_ptr<AbstractSyntaxTreeNode> body);
    virtual ~FunctionDefinition();

    void accept(AbstractSyntaxTreeVisitor& visitor) override;

    static const std::string ID;

    TypeSpecifier returnType;
    const std::unique_ptr<FunctionDeclaration> declaration;
    const std::unique_ptr<AbstractSyntaxTreeNode> body;

private:
    std::string name;
    BasicType basicType;
    std::string extended_type;
};

}

#endif // _FUNC_DECL_NODE_H_
