#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "scanner/IdentifierState.h"
#include "scanner/StringLiteralState.h"
#include "scanner/State.h"

#include <memory>
#include <stdexcept>
#include <string>

using namespace testing;
using std::string;
using std::shared_ptr;

TEST(State, doesNotNeedKeywordLookup) {
	shared_ptr<State> state { new State { "stateName", 123 } };

	ASSERT_THAT(state->getTokenId(), Eq(123));
	ASSERT_THAT(state->getName(), Eq("stateName"));
	ASSERT_THAT(state->needsKeywordLookup(), Eq(false));
}

TEST(State, doesNotRequireTokenIdToBeSpecified) {
	shared_ptr<State> state { new State { "stateName", 0 } };

	ASSERT_THAT(state->getTokenId(), Eq(0));
	ASSERT_THAT(state->getName(), Eq("stateName"));
}

TEST(State, constructsStateTransitionsFromStringOfCharacters) {
	shared_ptr<State> startState { new State { "startState", 0 } };

	std::shared_ptr<State> transitionState { new State { "transitionState", 456 } };

	startState->addTransition("abcd123", transitionState);

	ASSERT_THAT(startState->nextStateForCharacter('a'), Eq(transitionState));
	ASSERT_THAT(startState->nextStateForCharacter('b'), Eq(transitionState));
	ASSERT_THAT(startState->nextStateForCharacter('c'), Eq(transitionState));
	ASSERT_THAT(startState->nextStateForCharacter('d'), Eq(transitionState));
	ASSERT_THAT(startState->nextStateForCharacter('1'), Eq(transitionState));
	ASSERT_THAT(startState->nextStateForCharacter('2'), Eq(transitionState));
	ASSERT_THAT(startState->nextStateForCharacter('3'), Eq(transitionState));
	ASSERT_THROW(startState->nextStateForCharacter(' '), std::runtime_error);
	ASSERT_THROW(startState->nextStateForCharacter('\t'), std::runtime_error);
	ASSERT_THROW(startState->nextStateForCharacter('\n'), std::runtime_error);
	ASSERT_THROW(startState->nextStateForCharacter('-'), std::runtime_error);
}

TEST(State, constructsTransitionForAnyCharacter) {
	shared_ptr<State> startState { new State { "startState", 0 } };

	std::shared_ptr<State> transitionState { new State { "transitionState", 456 } };

	startState->addTransition("", transitionState);

	ASSERT_THAT(startState->nextStateForCharacter('a'), Eq(transitionState));
	ASSERT_THAT(startState->nextStateForCharacter('b'), Eq(transitionState));
	ASSERT_THAT(startState->nextStateForCharacter('3'), Eq(transitionState));
	ASSERT_THAT(startState->nextStateForCharacter('-'), Eq(transitionState));
	ASSERT_THAT(startState->nextStateForCharacter(' '), Eq(transitionState));
	ASSERT_THAT(startState->nextStateForCharacter('\n'), Eq(transitionState));
	ASSERT_THAT(startState->nextStateForCharacter('\t'), Eq(transitionState));
	ASSERT_THAT(startState->nextStateForCharacter('\0'), Eq(transitionState));
}

TEST(IdentifierState, needsLookup) {
	shared_ptr<State> state { new IdentifierState { "identifier", 123 } };

	ASSERT_THAT(state->getTokenId(), Eq(123));
	ASSERT_THAT(state->getName(), Eq("identifier"));
	ASSERT_THAT(state->needsKeywordLookup(), Eq(true));
}

TEST(IdentifierState, doesNotAcceptSpaces) {
	shared_ptr<State> state { new IdentifierState { "identifier", 123 } };

	ASSERT_THROW(state->nextStateForCharacter(' '), std::runtime_error);
}

TEST(StringLiteralState, consumesSpaces) {
	shared_ptr<State> state { new StringLiteralState { "stringLiteral", 123 } };

	ASSERT_THAT(state->getTokenId(), Eq(123));
	ASSERT_THAT(state->getName(), Eq("stringLiteral"));

	ASSERT_THAT(state->nextStateForCharacter(' '), Eq(state));
}

TEST(StringLiteralState, rejectsNewLines) {
	string definitionRecord = "\"stringLiteral 123";
	shared_ptr<State> state { new StringLiteralState { "stringLiteral", 123 } };

	ASSERT_THAT(state->getTokenId(), Eq(123));
	ASSERT_THAT(state->getName(), Eq("stringLiteral"));

	ASSERT_THROW(state->nextStateForCharacter('\n'), std::runtime_error);
}