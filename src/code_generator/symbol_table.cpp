#include <stdexcept>
#include <iostream>
#include <sstream>
#include "symbol_table.h"

using std::cerr;
using std::endl;
using std::cout;

SymbolTable::SymbolTable()
{
    nextTemp = "_t0";
    nextLabel = new string("__L0");
    outer_scope = NULL;
    offset = 0;
    paramOffset = 8;
    labels = 0;
}

SymbolTable::SymbolTable(const SymbolTable *outer)
{
    outer_scope = (SymbolTable *) outer;
    nextTemp = "_t0";
    nextLabel = outer->nextLabel;
    offset = 0;
    paramOffset = 8;
    labels = 0;
}

SymbolTable::~SymbolTable()
{
    for (map<string, SymbolEntry *>::iterator it = symbols.begin(); it != symbols.end(); it++)
        delete it->second;
    for (vector<SymbolTable *>::iterator it = inner_scopes.begin(); it != inner_scopes.end(); it++)
        delete *it;
}

int SymbolTable::insert(string name, string basic_type, string extended_type, unsigned line)
{
    SymbolEntry *entry;
    try
    {
        entry = symbols.at(name);
        return entry->getLine();
    }
    catch (std::out_of_range &ex)
    {
        entry = new SymbolEntry(name, basic_type, extended_type, false, line);
        if (basic_type != "label")
        {
            entry->setOffset(offset);
            offset += varSize;
        }
        symbols[name] = entry;
    }
    return 0;
}

int SymbolTable::insertParam(string name, string basic_type, string extended_type, unsigned line)
{
    SymbolEntry *entry;
    try
    {
        entry = symbols.at(name);
        return entry->getLine();
    }
    catch (std::out_of_range &ex)
    {
        entry = new SymbolEntry(name, basic_type, extended_type, false, line);
        if (basic_type != "label")
        {
            entry->setOffset(paramOffset);
            paramOffset += varSize;
            entry->setParam();
        }
        symbols[name] = entry;
    }
    return 0;
}

SymbolEntry *SymbolTable::lookup(string name) const
{
    SymbolEntry *entry = NULL;
    try
    {
        entry = symbols.at(name);
    }
    catch (std::out_of_range &ex)
    {
        if (outer_scope != NULL)
            entry = outer_scope->lookup(name);
    }
    return entry;
}

SymbolEntry *SymbolTable::newTemp(string basic_type, string extended_type)
{
    SymbolEntry *temp;
    for (unsigned long i = 0; i < (unsigned long)(-1); i++)
    {
        try
        {
            generateTempName();
            temp = symbols.at(nextTemp);
        }
        catch (std::out_of_range &ex)
        {
            temp = new SymbolEntry(nextTemp, basic_type, extended_type, true, 0);
            if (basic_type != "label")
            {
                temp->setOffset(offset);
                offset += varSize;
            }
            symbols[nextTemp] = temp;
            return temp;
        }
    }
    return temp;
}

SymbolEntry *SymbolTable::newLabel()
{
    SymbolEntry *label;
    for (unsigned long i = 0; i < (unsigned long)(-1); i++)
    {
        try
        {
            generateLabelName();
            label = symbols.at(*nextLabel);
        }
        catch (std::out_of_range &ex)
        {
            label = new SymbolEntry(*nextLabel, "label", "", true, 0);
            symbols[*nextLabel] = label;
            labels += varSize;
            return label;
        }
    }
    return label;
}

void SymbolTable::generateTempName()
{
    unsigned intVal;
    nextTemp = nextTemp.substr(2, nextTemp.size());
    intVal = atoi(nextTemp.c_str());
    intVal++;
    nextTemp = "_t";
    std::stringstream strStr;
    strStr << intVal;
    nextTemp += strStr.str();
}

void SymbolTable::generateLabelName()
{
    unsigned intVal;
    *nextLabel = nextLabel->substr(3, nextLabel->size());
    intVal = atoi(nextLabel->c_str());
    intVal++;
    *nextLabel = "__L";
    std::stringstream strStr;
    strStr << intVal;
    *nextLabel += strStr.str();
}

SymbolTable *SymbolTable::newScope()
{
    SymbolTable *inner = new SymbolTable(this);
    inner_scopes.push_back(inner);
    scopeIt = inner_scopes.begin();
    return inner;
}

SymbolTable *SymbolTable::next()
{
    SymbolTable *retVal = NULL;
    if (scopeIt != inner_scopes.end())
    {
        retVal = *scopeIt;
        scopeIt++;
        addOffset(retVal->getTableSize());
    }
    else 
    {
        retVal = outer_scope;
        outer_scope->removeOffset(getTableSize());
    }

    return retVal;
}

SymbolTable *SymbolTable::getOuterScope() const
{
    return outer_scope;
}

string SymbolTable::typeCheck(SymbolEntry *v1, SymbolEntry *v2)
{
    string err;
    if (NULL == v1)
        return "error, checking types! v1 is NULL!\n";
    if (NULL == v2)
        return "error, checking types! v2 is NULL!\n";
    string type1 = v1->getBasicType();
    string type2 = v2->getBasicType();
    string etype1 = v1->getExtendedType();
    string etype2 = v2->getExtendedType();
    if (type1 == "" && type2 == "")
    {
        type1 = "int";
        type2 = "int";
        v1->setBasicType(type1);
        v2->setBasicType(type2);
    }
    else if (type1 == "")
    {
        type1 = type2;
        v1->setBasicType(type1);
    }
    else if (type2 == "")
    {
        type2 = type1;
        v2->setBasicType(type2);
    }
    if ( (etype1 == etype2) && (type1 == type2) )
        return "ok";
    if (etype1.size() && etype2.size())
    {   // XXX: čia reiktų giliau patikrint
        if ((etype1[0] == 'p' && etype2[0] == 'a') || (etype1[0] == 'a' && etype2[0] == 'p'))
            return "ok";
    }
    err = "type mismatch: can't convert ";
    err += decorate(type1, etype1);
    err += " to ";
    err += decorate(type2, etype2);
    err += "\n";
    return err;
}

string SymbolTable::decorate(string type, string etype)
{
    for (unsigned i = 0; i < etype.size(); i++)
    {
        switch(etype[i])
        {
            case 'p':
                type += "*";
                break;
            case 'f':
                type += "()";
                break;
            case 'a':
                type += "[]";
                break;
            default:
                cerr << "Type check error! etype " << etype << " not implemented!\n";
        }
    }
    return type;
}

void SymbolTable::printTable() const
{
    if (symbols.size() || inner_scopes.size())
    {
        cout << "BEGIN SCOPE\t" << getTableSize() << endl;
        for (map<string, SymbolEntry *>::const_iterator it = symbols.begin(); it != symbols.end(); it++)
        {
            it->second->print();
        }
        for (unsigned i = 0; i < inner_scopes.size(); i++)
            inner_scopes[i]->printTable();
        cout << "END SCOPE" << endl;
    }
}

vector<SymbolTable *> SymbolTable::getInnerScopes() const
{
    return inner_scopes;
}

unsigned SymbolTable::getTableSize() const
{
    unsigned paramCount = (paramOffset - 8)/4;
    return symbols.size() * varSize - labels - paramCount*4;
}

void SymbolTable::addOffset(unsigned extra)
{
    for (map<string, SymbolEntry *>::iterator it = symbols.begin(); it != symbols.end(); it++)
    {
        SymbolEntry *entry = it->second;
        if (entry->getBasicType() != "label" && !entry->isParam())
            entry->setOffset(entry->getOffset()+extra);
    }
    if (NULL != outer_scope)
        outer_scope->addOffset(extra);
}

void SymbolTable::removeOffset(unsigned extra)
{
    for (map<string, SymbolEntry *>::iterator it = symbols.begin(); it != symbols.end(); it++)
    {
        SymbolEntry *entry = it->second;
        if (entry->getBasicType() != "label" && !entry->isParam())
            entry->setOffset(entry->getOffset()-extra);
    }
    if (NULL != outer_scope)
        outer_scope->removeOffset(extra);
}
