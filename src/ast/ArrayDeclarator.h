#ifndef ARRAYDECLARATION_H_
#define ARRAYDECLARATION_H_

#include <memory>

#include "DirectDeclarator.h"

namespace ast {

class Expression;

class ArrayDeclarator: public DirectDeclarator {
public:
    ArrayDeclarator(std::unique_ptr<DirectDeclarator> declarator, std::unique_ptr<Expression> subscriptExpression);
    virtual ~ArrayDeclarator() = default;

    void accept(AbstractSyntaxTreeVisitor& visitor) override;

    const std::unique_ptr<Expression> subscriptExpression;
};

} /* namespace ast */

#endif /* ARRAYDECLARATION_H_ */
