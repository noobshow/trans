#include "io_stmt_node.h"
#include "expr_node.h"

IOStmtNode::IOStmtNode(string l, vector<ParseTreeNode *> &children, Production production, SymbolTable *st, unsigned ln):
NonterminalNode(l, children, production, st, ln)
{
    if (reduction == "'output' <expr> ';'")
    {
        code = ((ExprNode *)subtrees[1])->getCode();
        SymbolEntry *expr_val = ((ExprNode *)subtrees[1])->getPlace();
        code.push_back(new Quadruple(OUT, expr_val, NULL, NULL));
    }
    else if (reduction == "'input' <expr> ';'")
    {
        code = ((ExprNode *)subtrees[1])->getCode();
        SymbolEntry *expr_val = ((ExprNode *)subtrees[1])->getPlace();
        code.push_back(new Quadruple(IN, expr_val, NULL, NULL));
    }
    else
    {
        cerr << "Error! No semantic actions found for rule { " << label << " ::= " << reduction << " }!\n";
        exit(1);
    }
}
