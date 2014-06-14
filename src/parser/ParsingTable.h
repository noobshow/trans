#ifndef _PARSING_TABLE_H_
#define _PARSING_TABLE_H_

#include <map>
#include <memory>
#include <string>
#include <unordered_map>

class Action;
class GrammarSymbol;

using parse_state = size_t;

class ParsingTable {
public:
	virtual ~ParsingTable();

	Action& action(parse_state state, std::string terminal) const;
	parse_state go_to(parse_state state, std::shared_ptr<const GrammarSymbol> nonterminal) const;

protected:
	ParsingTable();

	std::map<std::string, std::unique_ptr<Action>>* terminalActionTables { nullptr };
	std::unordered_map<parse_state, std::map<std::shared_ptr<const GrammarSymbol>, parse_state>> gotoTable;
};

#endif // _PARSING_TABLE_H_
