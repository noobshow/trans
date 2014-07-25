#ifndef _LOOP_HDR_NODE_H_
#define _LOOP_HDR_NODE_H_

#include <string>
#include <vector>

#include "../code_generator/quadruple.h"
#include "NonterminalNode.h"

namespace semantic_analyzer {

class LoopHeader: public NonterminalNode {
public:
	vector<Quadruple *> getBPList() const;
	vector<Quadruple *> getAfterLoop() const;
	SymbolEntry *getLoopLabel() const;

	static const std::string ID;

protected:
	LoopHeader(SymbolTable* symbolTable);

	vector<Quadruple *> backpatchList;
	vector<Quadruple *> afterLoop;
	SymbolEntry *loop_label;
};

}

#endif // _LOOP_HDR_NODE_H_
