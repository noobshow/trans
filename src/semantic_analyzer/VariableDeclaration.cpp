#include "VariableDeclaration.h"

#include <algorithm>

#include "AbstractSyntaxTreeVisitor.h"

#include "DeclarationList.h"
#include "Declaration.h"

namespace semantic_analyzer {

const std::string VariableDeclaration::ID { "<var_decl>" };

VariableDeclaration::VariableDeclaration(TypeSpecifier type, std::unique_ptr<DeclarationList> declarationList) :
        declaredType { type },
        declaredVariables { std::move(declarationList) } {
}

void VariableDeclaration::accept(AbstractSyntaxTreeVisitor& visitor) {
    visitor.visit(*this);
}

}
