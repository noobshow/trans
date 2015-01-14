#ifndef DOUBLEOPERANDEXPRESSION_H_
#define DOUBLEOPERANDEXPRESSION_H_

#include <memory>

#include "Expression.h"

namespace ast {

class Operator;

class DoubleOperandExpression: public Expression {
public:
    DoubleOperandExpression(std::unique_ptr<Expression> leftOperand, std::unique_ptr<Expression> rightOperand, std::unique_ptr<Operator> _operator);
    virtual ~DoubleOperandExpression();

    translation_unit::Context getContext() const override;

    Expression* getLeftOperand() const;
    Expression* getRightOperand() const;

    Operator* getOperator() const;

protected:
    const std::unique_ptr<Expression> leftOperand;
    const std::unique_ptr<Expression> rightOperand;

    const std::unique_ptr<Operator> _operator;
};

} /* namespace ast */

#endif /* DOUBLEOPERANDEXPRESSION_H_ */
