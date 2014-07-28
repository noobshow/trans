#include "ParameterList.h"

#include "ParameterDeclaration.h"

#include <algorithm>

#include "AbstractSyntaxTreeVisitor.h"

namespace semantic_analyzer {

const std::string ParameterList::ID { "<param_list>" };

ParameterList::ParameterList(std::unique_ptr<ParameterDeclaration> parameterDeclaration) {
	declaredParameters.push_back(std::move(parameterDeclaration));
}

void ParameterList::addParameterDeclaration(std::unique_ptr<ParameterDeclaration> parameterDeclaration) {
	declaredParameters.push_back(std::move(parameterDeclaration));
}

const vector<std::unique_ptr<ParameterDeclaration>>& ParameterList::getDeclaredParameters() const {
	return declaredParameters;
}

void ParameterList::accept(const AbstractSyntaxTreeVisitor& visitor) const {
	visitor.visit(*this);
}

}

