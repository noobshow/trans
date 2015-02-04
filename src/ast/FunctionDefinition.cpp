#include "FunctionDefinition.h"

#include <algorithm>

#include "../code_generator/FunctionEntry.h"
#include "../translation_unit/Context.h"
#include "AbstractSyntaxTreeVisitor.h"
#include "Declarator.h"
#include "DirectDeclarator.h"

namespace ast {

const std::string FunctionDefinition::ID { "<function_definition>" };

FunctionDefinition::FunctionDefinition(
        DeclarationSpecifiers returnType,
        std::unique_ptr<Declarator> declarator,
        std::unique_ptr<AbstractSyntaxTreeNode> body)
:
        returnType { returnType },
        declarator { std::move(declarator) },
        body { std::move(body) }
{
}

void FunctionDefinition::accept(AbstractSyntaxTreeVisitor& visitor) {
    visitor.visit(*this);
}

void FunctionDefinition::visitReturnType(AbstractSyntaxTreeVisitor& visitor) {
    returnType.accept(visitor);
}

void FunctionDefinition::visitDeclarator(AbstractSyntaxTreeVisitor& visitor) {
    declarator->accept(visitor);
}

void FunctionDefinition::visitBody(AbstractSyntaxTreeVisitor& visitor) {
    body->accept(visitor);
}

void FunctionDefinition::setSymbol(code_generator::FunctionEntry symbol) {
    this->symbol = std::make_unique<code_generator::FunctionEntry>(symbol);
}

code_generator::FunctionEntry* FunctionDefinition::getSymbol() const {
    return symbol.get();
}

}
