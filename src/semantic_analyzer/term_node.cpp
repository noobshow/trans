#include "term_node.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "../code_generator/quadruple.h"
#include "../code_generator/symbol_entry.h"
#include "../code_generator/symbol_table.h"

TermNode::TermNode(string l, vector<ParseTreeNode *> &children, Production production, SymbolTable *st, unsigned ln):
ExprNode(l, children, production, st, ln)
{
    if (reduction == "'id'")
    {
        value = subtrees[0]->getAttr();
        if ( NULL != (place = s_table->lookup(value)) )
        {
            value = "lval";
            basic_type = place->getBasicType();
            extended_type = place->getExtendedType();
        }
        else
        {
            semanticError("symbol " + value + " is not defined\n");
        }
    }
    else if (reduction == "'int_const'")
    {
        value = "rval";
        basic_type = "int";
        place = s_table->newTemp(basic_type, "");
        code.push_back(new Quadruple(subtrees[0]->getAttr(), place));
    }
    else if (reduction == "'float_const'")
    {
        value = "rval";
        basic_type = "float";
        place = s_table->newTemp(basic_type, "");
        code.push_back(new Quadruple(subtrees[0]->getAttr(), place));
    }
    else if (reduction == "'literal'")
    {
        value = "rval";
        basic_type = "char";
        place = s_table->newTemp(basic_type, "");
        code.push_back(new Quadruple(subtrees[0]->getAttr(), place));
    }
    else if (reduction == "'string'")
    {
        value = subtrees[0]->getAttr();
        basic_type = "char";
        extended_type = "a";
        place = s_table->newTemp(basic_type, extended_type);
        code.push_back(new Quadruple(subtrees[0]->getAttr(), place));
    }
    else if (reduction == "'(' <expr> ')'")
        getAttributes(1);
    else
    {
        cerr << "Error! No semantic actions found for rule { " << label << " ::= " << reduction << " }!\n";
        exit(1);
    }
}
