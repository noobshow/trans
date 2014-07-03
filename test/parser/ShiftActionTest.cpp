#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <memory>
#include <stack>
#include <vector>

#include "parser/ShiftAction.h"
#include "parser/Grammar.h"
#include "parser/ParseTreeBuilder.h"
#include "parser/SyntaxTree.h"
#include "scanner/Scanner.h"
#include "scanner/Token.h"

#include "../scanner/TokenMatcher.h"

namespace {

using testing::Eq;
using std::unique_ptr;

class GrammarStub: public Grammar {
public:
	std::vector<const GrammarSymbol*> getTerminals() const override {
		return {};
	}
	std::vector<const GrammarSymbol*> getNonterminals() const override {
		return {};
	}
};

class ParsingTableStub: public ParsingTable {
public:
	ParsingTableStub() :
			ParsingTable { new GrammarStub { } } {
	}
};

class ScannerStub: public Scanner {
public:
	Token nextToken() override {
		return tokens.at(currentToken++);
	}

private:
	std::vector<Token> tokens { Token { "a", "a", 0 }, { "b", "b", 1 } };
	size_t currentToken { 0 };
};

class ParseTreeBuilderMock: public ParseTreeBuilder {
public:
	std::unique_ptr<SyntaxTree> build() {
		return {nullptr};
	}

	MOCK_METHOD1(makeTerminalNode, void(const Token& token));
	MOCK_METHOD2(makeNonterminalNode, void(std::string definingSymbol, Production production));
};

TEST(ShiftAction, isSerializedAsShiftWithState) {
	ShiftAction shiftAction { 42 };

	ASSERT_THAT(shiftAction.serialize(), Eq("s 42"));
}

TEST(ShiftAction, isDeserializedFromString) {
	ParsingTableStub parsingTable;
	GrammarStub grammar;
	unique_ptr<Action> action { Action::deserialize(std::string { "s 42" }, parsingTable, grammar) };

	ASSERT_THAT(action->serialize(), Eq("s 42"));
}

TEST(ShiftAction, pushesItsStateOnStackAndAdvancesTokenStream) {
	ShiftAction shiftAction { 42 };
	std::stack<parse_state> parsingStack;
	TokenStream tokenStream { new ScannerStub { } };
	ParseTreeBuilderMock* parseTreeBuilderMock { new ParseTreeBuilderMock { } };
	EXPECT_CALL(*parseTreeBuilderMock, makeTerminalNode( tokenMatches(Token{ "a", "a",0 })));
	SemanticAnalyzer semanticAnalyzer { parseTreeBuilderMock };

	bool parsingDone = shiftAction.parse(parsingStack, tokenStream, semanticAnalyzer);

	ASSERT_THAT(tokenStream.getCurrentToken(), tokenMatches(Token { "b", "b", 1 }));
	ASSERT_THAT(parsingStack.top(), Eq(42));
	ASSERT_THAT(parsingDone, Eq(false));
}

}
