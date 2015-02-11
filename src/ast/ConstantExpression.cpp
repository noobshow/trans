#include "ConstantExpression.h"

#include "../translation_unit/Context.h"
#include "AbstractSyntaxTreeVisitor.h"
#include "types/Type.h"

namespace ast {

ConstantExpression::ConstantExpression(Constant constant) :
        constant { constant }
{
    setType(constant.getType());
}

ConstantExpression::~ConstantExpression() {
}

void ConstantExpression::accept(AbstractSyntaxTreeVisitor& visitor) {
    visitor.visit(*this);
}

translation_unit::Context ConstantExpression::getContext() const {
    return constant.getContext();
}

std::string ConstantExpression::getValue() const {
    return constant.getValue();
}

} /* namespace ast */
