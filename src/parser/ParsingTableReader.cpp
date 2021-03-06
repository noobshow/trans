#include "ParsingTableReader.h"

#include <stdexcept>

using std::string;

namespace parser {

const static string CONFIGURATION_DELIMITER = "\%\%";

ParsingTableReader::ParsingTableReader(string fileName) :
        parsingTableStream { fileName }
{
    if (!parsingTableStream.is_open()) {
        throw std::runtime_error("could not open parsing table configuration file for reading. Filename: " + fileName);
    }
}

ParsingTableReader::~ParsingTableReader() {
}

size_t ParsingTableReader::readStateCount() {
    size_t stateCount;
    parsingTableStream >> stateCount;
    return stateCount;
}

void ParsingTableReader::readDelimiter() {
    string delimiter;
    parsingTableStream >> delimiter;
    if (delimiter != CONFIGURATION_DELIMITER) {
        throw std::runtime_error("error in parsing table configuration file: " + CONFIGURATION_DELIMITER + " delimiter expected");
    }
    std::getline(parsingTableStream, delimiter);
}

string ParsingTableReader::readSerializedAction() {
    string serializedAction;
    if (!std::getline(parsingTableStream, serializedAction)) {
        throw std::runtime_error { "error reading parsing table action" };
    }
    return serializedAction;
}

std::tuple<parse_state, std::string, parse_state> ParsingTableReader::readGotoRecord() {
    parse_state from_state;
    std::string onNonterminal;
    parse_state toState;
    parsingTableStream >> from_state >> onNonterminal >> toState;
    return std::make_tuple(from_state, onNonterminal, toState);
}

}

bool parser::ParsingTableReader::endOfFile() const {
    return parsingTableStream.eof();
}
