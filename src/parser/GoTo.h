#ifndef GOTO_H_
#define GOTO_H_

#include <vector>

#include "Closure.h"
#include "LR1Item.h"

#include "GrammarSymbol.h"

namespace parser {

class GoTo {
public:
	GoTo(const Closure& closure);
	virtual ~GoTo();

	std::vector<LR1Item> operator()(const std::vector<LR1Item>& I, const GrammarSymbol& X) const;

private:
	const Closure closure;
};

}

#endif /* GOTO_H_ */
