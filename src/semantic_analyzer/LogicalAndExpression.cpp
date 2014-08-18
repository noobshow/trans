#include "LogicalAndExpression.h"

#include <algorithm>

#include "AbstractSyntaxTreeVisitor.h"

namespace semantic_analyzer {

const std::string LogicalAndExpression::ID { "<log_and_expr>" };

LogicalAndExpression::LogicalAndExpression(std::unique_ptr<Expression> leftHandSide, std::unique_ptr<Expression> rightHandSide) :
        LogicalExpression(std::move(leftHandSide), std::move(rightHandSide)) {
    basicType = this->leftHandSide->getBasicType();
    extended_type = this->leftHandSide->getExtendedType();
}

void LogicalAndExpression::accept(AbstractSyntaxTreeVisitor& visitor) {
    visitor.visit(*this);
}

}

