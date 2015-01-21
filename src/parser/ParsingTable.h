#ifndef _PARSING_TABLE_H_
#define _PARSING_TABLE_H_

#include <map>
#include <memory>
#include <string>
#include <unordered_map>

#include "LookaheadActionTable.h"

class Token;

namespace parser {

class LR1Item;
class Action;
class Grammar;

class ParsingTable {
public:
	ParsingTable(const Grammar* grammar);
	virtual ~ParsingTable();

	const Action& action(parse_state state, Token lookahead) const;
	parse_state go_to(parse_state state, std::string nonterminal) const;
protected:
	std::unique_ptr<const Grammar> grammar;

	std::unordered_map<parse_state, std::unordered_map<std::string, parse_state>> gotoTable;

	LookaheadActionTable lookaheadActionTable;
};

}

#endif // _PARSING_TABLE_H_
