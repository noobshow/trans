#ifndef FINITEAUTOMATONFACTORY_H_
#define FINITEAUTOMATONFACTORY_H_

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "StateMachineFactory.h"

class State;

class FiniteAutomatonFactory: public StateMachineFactory {
public:
	FiniteAutomatonFactory(std::string configurationFileName);
	virtual ~FiniteAutomatonFactory();

	std::unique_ptr<StateMachine> createAutomaton() const;

private:
	std::shared_ptr<State> startState { nullptr };
	std::map<std::string, unsigned> keywordIds;

	int nextKeywordId { 1 };
	std::map<std::string, std::shared_ptr<State>> namedStates;

	std::shared_ptr<State> createNewState(std::string stateDefinitionRecord);
	std::pair<std::string, std::string> createNamedTransitionPair(std::string transitionDefinitionRecord);
	void parseKeywords(std::string keywordsRecord);

	friend std::ostream& operator<<(std::ostream& os , const FiniteAutomatonFactory& factory);
};

#endif /* FINITEAUTOMATONFACTORY_H_ */