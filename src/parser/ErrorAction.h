#ifndef ERRORACTION_H_
#define ERRORACTION_H_

#include <iostream>
#include <memory>
#include <stack>
#include <string>

#include "../semantic_analyzer/SemanticAnalyzer.h"
#include "../util/Logger.h"
#include "Action.h"
#include "ParsingTable.h"
#include "TokenStream.h"

class ErrorAction: public Action {
public:
	ErrorAction(parse_state state, std::string forgeToken, std::shared_ptr<const GrammarSymbol> expectedSymbol, Logger logger);
	virtual ~ErrorAction();

	std::unique_ptr<SyntaxTree> perform(std::stack<parse_state>& parsingStack, TokenStream& tokenStream,
			SemanticAnalyzer& semanticAnalyzer) override;

	std::string describe() const override;

private:
	const parse_state state;

	std::string forgeToken;
	std::shared_ptr<const GrammarSymbol> expectedSymbol;
};

#endif /* ERRORACTION_H_ */