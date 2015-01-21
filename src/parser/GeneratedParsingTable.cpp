#include "GeneratedParsingTable.h"

#include <fstream>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <utility>

#include "AcceptAction.h"
#include "CanonicalCollection.h"
#include "ErrorAction.h"
#include "Grammar.h"
#include "GrammarSymbol.h"
#include "LookaheadActionTable.h"
#include "ReduceAction.h"
#include "ShiftAction.h"

using std::string;
using std::vector;
using std::unique_ptr;

using std::endl;

namespace parser {

GeneratedParsingTable::GeneratedParsingTable(const Grammar* grammar) :
        ParsingTable(grammar),
        firstTable { this->grammar->getNonterminals() }
{
    CanonicalCollection canonicalCollection { firstTable, *this->grammar };

    computeActionTable(canonicalCollection);
    computeGotoTable(canonicalCollection);
    computeErrorActions(canonicalCollection.stateCount());
}

GeneratedParsingTable::~GeneratedParsingTable() {
}

void GeneratedParsingTable::computeActionTable(const CanonicalCollection& canonicalCollection) {
    size_t stateCount = canonicalCollection.stateCount();
    for (parse_state currentState = 0; currentState < stateCount; ++currentState) {
        const auto& setOfItemsForCurrentState = canonicalCollection.setOfItemsAtState(currentState);
        for (const auto& item : setOfItemsForCurrentState) {
            if (item.hasUnvisitedSymbols()) {
                const auto nextExpectedSymbolForItem = item.nextUnvisitedSymbol();
                if (nextExpectedSymbolForItem->isTerminal()) {
                    lookaheadActionTable.addAction(
                            currentState,
                            nextExpectedSymbolForItem->getDefinition(),
                            std::make_unique<ShiftAction>(canonicalCollection.goTo(currentState, nextExpectedSymbolForItem->getDefinition())));
                }
            } else if ((item.getDefiningSymbol() == grammar->getStartSymbol()) && (item.getLookaheads().front() == grammar->getEndSymbol())) {
                lookaheadActionTable.addAction(
                        currentState,
                        grammar->getEndSymbol()->getDefinition(),
                        std::make_unique<AcceptAction>());
            } else {
                for (const auto lookahead : item.getLookaheads()) {
                    lookaheadActionTable.addAction(
                            currentState,
                            lookahead->getDefinition(),
                            std::make_unique<ReduceAction>(item.getDefiningSymbol(), item.getProductionNumber(), this));
                }
            }
        }
    }
}

void GeneratedParsingTable::computeGotoTable(const CanonicalCollection& canonicalCollection) {
    size_t stateCount = canonicalCollection.stateCount();
    for (parse_state state = 0; state < stateCount; ++state) {
        vector<LR1Item> setOfItems = canonicalCollection.setOfItemsAtState(state);
        for (auto& nonterminal : grammar->getNonterminals()) {
            try {
                auto stateTo = canonicalCollection.goTo(state, nonterminal->getDefinition());
                gotoTable[state][nonterminal->getDefinition()] = stateTo;
            } catch (std::out_of_range&) {
            }
        }
    }
}

// FIXME: this is fucked
void GeneratedParsingTable::computeErrorActions(size_t stateCount) {
    const GrammarSymbol* expected;
    string forge_token;
    for (int state = 0; state < stateCount; state++) {        // for each state
        unsigned term_size = 9999;
        forge_token.clear();
        int errorState = 0;
        for (auto& terminal : grammar->getTerminals()) { // surandam galimą teisingą veiksmą
            try {
                auto& error_action = lookaheadActionTable.action(state, terminal->getDefinition());
                //errorState = error_action.getState();
                if (terminal->getDefinition().size() < term_size) {
                    expected = terminal;
                    term_size = terminal->getDefinition().size();
                    //if (error_action.which() == 'r') {
                    //	forge_token = terminal->getName();
                    //}
                }
            } catch (std::out_of_range&) {
            }
        }
        if (expected) {
            try {
                auto& error_action = lookaheadActionTable.action(state, expected->getDefinition());
                //errorState = error_action.getState();
            } catch (std::out_of_range&) {
            }
        }

        for (auto& terminal : grammar->getTerminals()) { // for each terminal
            try {
                lookaheadActionTable.action(state, terminal->getDefinition());
            } catch (std::out_of_range&) {
                lookaheadActionTable.addAction(
                        state,
                        terminal->getDefinition(),
                        std::make_unique<ErrorAction>(errorState, forge_token, expected->getDefinition()));
            }
        }
    }
}

void GeneratedParsingTable::persistToFile(string fileName) const {
    std::ofstream table_out { fileName };
    if (!table_out.is_open()) {
        throw std::runtime_error { "Unable to create parsing table output file!\n" };
    }

    size_t stateCount = lookaheadActionTable.size();
    table_out << stateCount << endl;
    table_out << "\%\%" << endl;
    for (int i = 0; i < stateCount; i++) {
        for (auto& terminal : grammar->getTerminals()) {
            auto& act = lookaheadActionTable.action(i, terminal->getDefinition());
            table_out << act.serialize() << "\n";
        }
    }
    table_out << "\%\%" << endl;

    for (const auto& stateGotos : gotoTable) {
        for (const auto& nonterminalGotoState : stateGotos.second) {
            table_out << stateGotos.first << " " << nonterminalGotoState.first << " " << nonterminalGotoState.second << "\n";
        }
    }
}

}
