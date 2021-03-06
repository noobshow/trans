#ifndef PARSINGTABLEREADER_H_
#define PARSINGTABLEREADER_H_

#include <stddef.h>
#include <fstream>
#include <string>
#include <tuple>

#include "LookaheadActionTable.h"

namespace parser {

class ParsingTableReader {
public:
	ParsingTableReader(std::string fileName);
	virtual ~ParsingTableReader();

	size_t readStateCount();
	void readDelimiter();
	std::string readSerializedAction();
	std::tuple<parse_state, std::string, parse_state> readGotoRecord();
	bool endOfFile() const;

private:
	std::ifstream parsingTableStream;
};

}

#endif /* PARSINGTABLEREADER_H_ */
