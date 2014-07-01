#ifndef _UNMATCHED_NODE_H_
#define _UNMATCHED_NODE_H_

#include "../parser/nonterminal_node.h"

/**
 * @author Vaidotas Valuckas
 * input/output sakinio klasė
 **/

class UnmatchedNode : public NonterminalNode
{
    public:
        UnmatchedNode(string l, vector<ParseTreeNode *> &children, Production production, SymbolTable *st, unsigned ln);

    private:
};

#endif // _UNMATCHED_NODE_H_
