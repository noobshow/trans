#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "scanner/StateMachineFactory.h"
#include "scanner/FiniteAutomatonFactory.h"
#include "scanner/StateMachine.h"
#include "scanner/FiniteAutomatonScanner.h"
#include "driver/TranslationUnit.h"
#include "driver/SourceTranslationUnit.h"
#include <memory>

using namespace testing;

class MockStateMachineFactory: public StateMachineFactory {
public:
	std::unique_ptr<StateMachine> createAutomaton() const {
		return std::unique_ptr<StateMachine> { createAutomatonProxy() };
	}

	MOCK_CONST_METHOD0(createAutomatonProxy, StateMachine* ());
};

class MockStateMachine: public StateMachine {
public:
	MOCK_METHOD1(updateState, void (char inputSymbol));
	MOCK_CONST_METHOD0(isAtFinalState, bool ());
	MOCK_METHOD0(getCurrentToken, Token ());
};

class MockTranslationUnit: public TranslationUnit {
public:
	MOCK_CONST_METHOD0(getFileName, std::string ());
	MOCK_METHOD0(getNextToken, Token ());
	MOCK_METHOD0(getNextCharacter, char ());
};

TEST(FiniteAutomatonScanner, usesStateMachineToProcessTranslationUnitCharacters) {
	std::unique_ptr<StrictMock<MockStateMachineFactory>> stateMachineFactoryPtr { new StrictMock<MockStateMachineFactory> };
	StrictMock<MockStateMachineFactory> *stateMachineFactory = stateMachineFactoryPtr.get();
	FiniteAutomatonScanner scanner { std::move(stateMachineFactoryPtr) };

	StrictMock<MockStateMachine>* stateMachine = new StrictMock<MockStateMachine>;
	Token expectedToken = Token { 0, "expected" };

	StrictMock<MockTranslationUnit> translationUnit;
	{
		InSequence sequence;

		EXPECT_CALL(*stateMachineFactory, createAutomatonProxy()).WillOnce(Return(stateMachine));

		EXPECT_CALL(translationUnit, getNextCharacter()).WillOnce(Return('a'));

		EXPECT_CALL(*stateMachine, updateState('a'));
		EXPECT_CALL(*stateMachine, isAtFinalState()).WillOnce(Return(false));
		EXPECT_CALL(translationUnit, getNextCharacter()).WillOnce(Return('b'));
		EXPECT_CALL(*stateMachine, isAtFinalState()).WillOnce(Return(false));

		EXPECT_CALL(*stateMachine, updateState('b'));
		EXPECT_CALL(*stateMachine, isAtFinalState()).WillOnce(Return(true));
		EXPECT_CALL(*stateMachine, isAtFinalState()).WillOnce(Return(true));
		EXPECT_CALL(*stateMachine, getCurrentToken()).WillOnce(Return(expectedToken));
	}

	Token token = scanner.scan(translationUnit);

	ASSERT_THAT(token.value, Eq(expectedToken.value));
}

TEST(FiniteAutomatonScanner, returnsLastTokenAtTheEndOfFile) {
	std::unique_ptr<StrictMock<MockStateMachineFactory>> stateMachineFactoryPtr { new StrictMock<MockStateMachineFactory> };
	StrictMock<MockStateMachineFactory> *stateMachineFactory = stateMachineFactoryPtr.get();
	FiniteAutomatonScanner scanner { std::move(stateMachineFactoryPtr) };

	StrictMock<MockStateMachine>* stateMachine = new StrictMock<MockStateMachine>;
	Token expectedToken = Token { 0, "expected" };

	StrictMock<MockTranslationUnit> translationUnit;
	{
		InSequence sequence;

		EXPECT_CALL(*stateMachineFactory, createAutomatonProxy()).WillOnce(Return(stateMachine));

		EXPECT_CALL(translationUnit, getNextCharacter()).WillOnce(Return('a'));

		EXPECT_CALL(*stateMachine, updateState('a'));
		EXPECT_CALL(*stateMachine, isAtFinalState()).WillOnce(Return(false));
		EXPECT_CALL(translationUnit, getNextCharacter()).WillOnce(Return('\0'));
		EXPECT_CALL(*stateMachine, isAtFinalState()).WillOnce(Return(false));

		EXPECT_CALL(*stateMachine, updateState('\0'));
		EXPECT_CALL(*stateMachine, isAtFinalState()).WillOnce(Return(true));
		EXPECT_CALL(*stateMachine, isAtFinalState()).WillOnce(Return(true));
		EXPECT_CALL(*stateMachine, getCurrentToken()).WillOnce(Return(expectedToken));
	}

	Token token = scanner.scan(translationUnit);

	ASSERT_THAT(token.value, Eq(expectedToken.value));
}

TEST(FiniteAutomatonScannerIntegrationTest, scansTheExampleProgram) {
	std::unique_ptr<FiniteAutomatonFactory> stateMachineFactory { new FiniteAutomatonFactory("resources/configuration/scanner.lex") };
	FiniteAutomatonScanner scanner { std::move(stateMachineFactory) };

	SourceTranslationUnit translationUnit("test/programs/example_prog.src", scanner);

	Token token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("int"));
	ASSERT_THAT(token.type, Eq(1));
	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("MAXLINE"));
	ASSERT_THAT(token.type, Eq(25));
	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("="));
	ASSERT_THAT(token.type, Eq(21));
	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("255"));
	ASSERT_THAT(token.type, Eq(38));
	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq(";"));
	ASSERT_THAT(token.type, Eq(20));

	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("int"));
	ASSERT_THAT(token.type, Eq(1));
	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("write_out"));
	ASSERT_THAT(token.type, Eq(25));
	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("("));
	ASSERT_THAT(token.type, Eq(26));
	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("char"));
	ASSERT_THAT(token.type, Eq(2));
	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("*"));
	ASSERT_THAT(token.type, Eq(24));
	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("type"));
	ASSERT_THAT(token.type, Eq(25));
	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq(","));
	ASSERT_THAT(token.type, Eq(28));
	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("void"));
	ASSERT_THAT(token.type, Eq(3));
	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("*"));
	ASSERT_THAT(token.type, Eq(24));
	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("out"));
	ASSERT_THAT(token.type, Eq(25));
	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq(")"));
	ASSERT_THAT(token.type, Eq(27));

	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("{"));
	ASSERT_THAT(token.type, Eq(22));

	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("if"));
	ASSERT_THAT(token.type, Eq(5));
	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("("));
	ASSERT_THAT(token.type, Eq(26));
	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("type"));
	ASSERT_THAT(token.type, Eq(25));
	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("=="));
	ASSERT_THAT(token.type, Eq(41));
	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("\"%s\""));
	ASSERT_THAT(token.type, Eq(40));
	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq(")"));
	ASSERT_THAT(token.type, Eq(27));

	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("{"));
	ASSERT_THAT(token.type, Eq(22));

	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("char"));
	ASSERT_THAT(token.type, Eq(2));
	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("*"));
	ASSERT_THAT(token.type, Eq(24));
	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("string"));
	ASSERT_THAT(token.type, Eq(25));
	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("="));
	ASSERT_THAT(token.type, Eq(21));
	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("("));
	ASSERT_THAT(token.type, Eq(26));
	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("char"));
	ASSERT_THAT(token.type, Eq(2));
	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("*"));
	ASSERT_THAT(token.type, Eq(24));
	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq(")"));
	ASSERT_THAT(token.type, Eq(27));
	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("out"));
	ASSERT_THAT(token.type, Eq(25));
	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq(";"));
	ASSERT_THAT(token.type, Eq(20));

	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("while"));
	ASSERT_THAT(token.type, Eq(7));
	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("("));
	ASSERT_THAT(token.type, Eq(26));
	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("*"));
	ASSERT_THAT(token.type, Eq(24));
	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("string"));
	ASSERT_THAT(token.type, Eq(25));
	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("!="));
	ASSERT_THAT(token.type, Eq(55));
	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("'\\0'"));
	ASSERT_THAT(token.type, Eq(39));
	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq(")"));
	ASSERT_THAT(token.type, Eq(27));

	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("output"));
	ASSERT_THAT(token.type, Eq(13));
	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("*"));
	ASSERT_THAT(token.type, Eq(24));
	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("string"));
	ASSERT_THAT(token.type, Eq(25));
	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("++"));
	ASSERT_THAT(token.type, Eq(36));
	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq(";"));
	ASSERT_THAT(token.type, Eq(20));

	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("return"));
	ASSERT_THAT(token.type, Eq(11));
	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("1"));
	ASSERT_THAT(token.type, Eq(38));
	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq(";"));
	ASSERT_THAT(token.type, Eq(20));

	token = scanner.scan(translationUnit);
	ASSERT_THAT(token.value, Eq("}"));
	ASSERT_THAT(token.type, Eq(23));

	token = scanner.scan(translationUnit);
	std::cerr << token.value << " " << token.type << std::endl;
	token = scanner.scan(translationUnit);
	std::cerr << token.value << " " << token.type << std::endl;
	token = scanner.scan(translationUnit);
	std::cerr << token.value << " " << token.type << std::endl;
	token = scanner.scan(translationUnit);
	std::cerr << token.value << " " << token.type << std::endl;
	token = scanner.scan(translationUnit);
	std::cerr << token.value << " " << token.type << std::endl;
	token = scanner.scan(translationUnit);
	std::cerr << token.value << " " << token.type << std::endl;
}
