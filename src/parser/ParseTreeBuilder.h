#ifndef PARSETREEBUILDER_H_
#define PARSETREEBUILDER_H_

#include <stddef.h>
#include <memory>
#include <stack>
#include <string>
#include <vector>

#include "ParseTreeNode.h"
#include "Production.h"
#include "SyntaxTreeBuilder.h"

namespace parser {

class ParseTreeBuilder : public SyntaxTreeBuilder {
public:
	ParseTreeBuilder(std::string sourceFileName);
	virtual ~ParseTreeBuilder();

	virtual std::unique_ptr<SyntaxTree> build() override;

	virtual void makeTerminalNode(std::string type, std::string value, const translation_unit::Context& context) override;
	virtual void makeNonterminalNode(std::string definingSymbol, parser::Production production) override;

protected:
	std::vector<std::unique_ptr<ParseTreeNode>> getChildrenForReduction(int childrenCount);

	std::stack<std::unique_ptr<ParseTreeNode>> syntaxStack;

    std::string sourceFileName;
};

}

#endif /* PARSETREEBUILDER_H_ */
