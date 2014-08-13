#include "ForLoopHeader.h"

#include <algorithm>

#include "AbstractSyntaxTreeVisitor.h"

namespace semantic_analyzer {

ForLoopHeader::ForLoopHeader(std::unique_ptr<Expression> initialization, std::unique_ptr<Expression> clause,
        std::unique_ptr<Expression> increment) :
        LoopHeader(std::move(increment)),
        initialization { std::move(initialization) },
        clause { std::move(clause) } {
}

ForLoopHeader::~ForLoopHeader() {
}

void ForLoopHeader::accept(AbstractSyntaxTreeVisitor& visitor) {
    visitor.visit(*this);
}

} /* namespace semantic_analyzer */
