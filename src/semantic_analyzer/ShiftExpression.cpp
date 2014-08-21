#include "ShiftExpression.h"

#include <algorithm>

#include "AbstractSyntaxTreeVisitor.h"
#include "BasicType.h"

namespace semantic_analyzer {

const std::string ShiftExpression::ID { "<s_expr>" };

ShiftExpression::ShiftExpression(std::unique_ptr<Expression> shiftExpression, TerminalSymbol shiftOperator,
        std::unique_ptr<Expression> additionExpression) :
        shiftExpression { std::move(shiftExpression) },
        shiftOperator { shiftOperator },
        additionExpression { std::move(additionExpression) } {
}

}

void semantic_analyzer::ShiftExpression::accept(AbstractSyntaxTreeVisitor& visitor) {
    visitor.visit(*this);
}
