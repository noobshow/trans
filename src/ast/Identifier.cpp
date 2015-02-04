#include "Identifier.h"

#include <string>

#include "AbstractSyntaxTreeVisitor.h"
#include "TerminalSymbol.h"

namespace ast {

Identifier::Identifier(TerminalSymbol identifier) :
        DirectDeclarator(identifier.value, identifier.context) {
}

void Identifier::accept(AbstractSyntaxTreeVisitor& visitor) {
    visitor.visit(*this);
}

} /* namespace semantic_analyzer */
