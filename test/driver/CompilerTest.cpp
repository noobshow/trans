#include "driver/Compiler.h"
#include "driver/TranslationUnit.h"
#include "driver/CompilerComponentsFactory.h"
#include "driver/Configuration.h"
#include "semantic_analyzer/SemanticComponentsFactory.h"
#include "semantic_analyzer/SyntaxTreeBuilder.h"
#include "semantic_analyzer/SyntaxTree.h"
#include "parser/Parser.h"
#include "scanner/Scanner.h"
#include "scanner/Token.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace testing;

class MockConfiguration: public Configuration {
public:
	MOCK_CONST_METHOD0(getSourceFileNames, const std::vector<std::string> &());
	MOCK_CONST_METHOD0(getCustomGrammarFileName, const std::string ());
	MOCK_CONST_METHOD0(usingCustomGrammar, bool());
	MOCK_CONST_METHOD0(isParserLoggingEnabled, bool ());
	MOCK_CONST_METHOD0(isScannerLoggingEnabled, bool ());
};

class MockParser: public Parser {
public:
	std::unique_ptr<SyntaxTree> parse(Scanner& scanner) {
		return std::unique_ptr<SyntaxTree> { parseProxy(scanner) };
	}

	MOCK_METHOD1(parseProxy, SyntaxTree*(Scanner&));
};

class MockSemanticComponentsFactory: public SemanticComponentsFactory {
public:
	MOCK_CONST_METHOD0(newSyntaxTreeBuilder, SyntaxTreeBuilder*());
};

class MockSyntaxTreeBuilder: public SyntaxTreeBuilder {
};

TEST(Compiler, throwsForNonExistentFile) {
	MockConfiguration configuration;
	Compiler compiler { new CompilerComponentsFactory { configuration } };

	std::unique_ptr<MockParser> parser { new StrictMock<MockParser> };

	ASSERT_THROW(compiler.compile("nonexistentSourceFileName"), std::runtime_error);
}

TEST(Compiler, compilesExampleProgram) {
	MockConfiguration configuration;
	Compiler compiler { new CompilerComponentsFactory { configuration } };

	std::unique_ptr<MockParser> parser { new StrictMock<MockParser> };

	try {
		compiler.compile("test/programs/test_prog1");
		FAIL();
	} catch (std::runtime_error& err) {
		ASSERT_THAT(err.what(), StrEq("Error creating assembler output file!"));
	}
}