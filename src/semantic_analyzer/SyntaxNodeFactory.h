#ifndef SYNTAX_NODE_FACTORY_H_
#define SYNTAX_NODE_FACTORY_H_

#include <functional>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "AbstractSyntaxTreeBuilderContext.h"

namespace semantic_analyzer {

class AbstractSyntaxTreeBuilderContext;
class AbstractSyntaxTreeNode;

class SyntaxNodeFactory {
public:
	SyntaxNodeFactory();
	virtual ~SyntaxNodeFactory();

	void updateContext(std::string definingSymbol, const std::vector<std::string>& production,
			AbstractSyntaxTreeBuilderContext& context) const;

private:
	static void noCreatorDefined(std::string definingSymbol, const std::vector<std::string>& production);

	static void parenthesizedExpression(AbstractSyntaxTreeBuilderContext& context);
	static void term(AbstractSyntaxTreeBuilderContext& context);

	static void arrayAccess(AbstractSyntaxTreeBuilderContext& context);
	static void functionCall(AbstractSyntaxTreeBuilderContext& context);
	static void noargFunctionCall(AbstractSyntaxTreeBuilderContext& context);


	std::unordered_map<std::string,
			std::map<std::vector<std::string>, std::function<void(AbstractSyntaxTreeBuilderContext&)>>>nodeCreatorRegistry;
};

}
/* namespace semantic_analyzer */

#endif /* SYNTAX_NODE_FACTORY_H_ */