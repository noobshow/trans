#ifndef _STATE_H_
#define _STATE_H_

#include <iostream>
#include <map>
#include <string>

class State {
public:
	State(std::string stateName, std::string tokenId);
	virtual ~State();

	void addTransition(std::string charactersForTransition, State* state);
	virtual const State* nextStateForCharacter(char c) const;
	std::string getName() const;

	void outputState(std::ostream& ostream) const;

	std::string getTokenId() const;
	virtual bool needsKeywordLookup() const;
	bool isFinal() const;

private:
	std::string stateName;
	std::string tokenId;

	State* wildcardTransition;
	std::map<char, State*> transitions;

	friend std::ostream& operator<<(std::ostream& ostream, const State& state);
};

#endif // _STATE_H_
