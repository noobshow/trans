#ifndef _RULE_H_
#define _RULE_H_

#include <iostream>
#include <memory>
#include <string>
#include <vector>

class GrammarSymbol;

class GrammarRule {
public:
	GrammarRule(const std::shared_ptr<GrammarSymbol> nonterminal, const std::vector<std::shared_ptr<GrammarSymbol>> production,
			const int ruleId);
	~GrammarRule();

	std::shared_ptr<GrammarSymbol> getNonterminal() const;
	std::vector<std::shared_ptr<GrammarSymbol>> getProduction() const;

	std::string rightStr() const;

	int getId() const;

	void log(std::ostream &out) const;

private:
	int id;
	std::shared_ptr<GrammarSymbol> nonterminal;
	std::vector<std::shared_ptr<GrammarSymbol>> production;
};

#endif // _RULE_H_
