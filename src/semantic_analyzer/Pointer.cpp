#include "Pointer.h"

#include "AbstractSyntaxTreeVisitor.h"

namespace semantic_analyzer {

const std::string Pointer::ID { "<ptr>" };

Pointer::Pointer() {
}

void Pointer::dereference() {
    ++dereferenceCount;
}

int Pointer::getDereferenceCount() const {
    return dereferenceCount;
}

void Pointer::accept(AbstractSyntaxTreeVisitor& visitor) {
    visitor.visit(*this);
}

}
