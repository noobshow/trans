#ifndef _RULE_H_
#define _RULE_H_

#include <string>
#include <vector>
#include <iostream>

using std::string;
using std::vector;
using std::ostream;

/**
 * @author Vaidotas Valuckas
 * Gramatikos taisyklės klasė
 * [ left -> right ]
 **/

class Rule
{
    public:
        Rule(string *l, int ruleId);
        ~Rule();

        string              *getLeft()   const;
        vector<string *>    *getRight()  const;

        string          rightStr() const;

        unsigned        getId() const;

        void            addRight(string *r);

        void            print()     const;
        void            printAddr()     const;

        void            log(ostream &out) const;

    private:
        unsigned            id;
        string              *left;
        vector<string *>    *right;
};

#endif // _RULE_H_
