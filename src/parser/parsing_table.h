#ifndef _PARSING_TABLE_H_
#define _PARSING_TABLE_H_

#include <fstream>
#include <stdlib.h>
#include "grammar.h"
#include "set_of_items.h"
#include "action.h"

using std::ofstream;
using std::ifstream;

/**
 * @author Vaidotas
 * Parserio lentelės klasė
 **/

class Parsing_table
{
    public:
        Parsing_table();
        Parsing_table(const char *bnf);
        ~Parsing_table();

        Action *action(unsigned state, unsigned terminal) const;
        Action *go_to(unsigned state, string nonterminal) const;

        int fill_actions(vector<Set_of_items *> *C);
        int fill_goto(vector<Set_of_items *> *C);
        void fill_errors();

        void print_actions() const;
        void print_goto() const;
        string getTerminalById(unsigned id) const;

        void output_html() const;
        void output_table() const;

        void log(ofstream &out) const;

    private:
        bool v_are_equal(vector<string> v1, vector<string> v2) const;

        void read_table(ifstream &table);

        Grammar *grammar;

        unsigned long    state_count;
        map<string, Action *>   *action_table;
        map<string, Action *>   *goto_table;
        const vector<string *> *nonterminals;
        const map<unsigned, string *> *terminals;

        vector<Set_of_items *> *items;

        map<long, Action *> shifts;
        vector<Action *> reductions;
        map<long, Action *> gotos;
        map<long, Action *> errors;
};

#endif // _PARSING_TABLE_H_