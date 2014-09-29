#ifndef ARRAYDECLARATION_H_
#define ARRAYDECLARATION_H_

#include <memory>

#include "Declaration.h"

namespace ast {

class Expression;

class ArrayDeclaration: public Declaration {
public:
	ArrayDeclaration(std::unique_ptr<Declaration> declaration, std::unique_ptr<Expression> subscriptExpression);
	virtual ~ArrayDeclaration();

	void accept(AbstractSyntaxTreeVisitor& visitor) override;

	const std::unique_ptr<Expression> subscriptExpression;
};

} /* namespace ast */

#endif /* ARRAYDECLARATION_H_ */