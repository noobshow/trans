#ifndef GENERATEDPARSINGTABLE_H_
#define GENERATEDPARSINGTABLE_H_

#include <vector>

#include "FirstTable.h"
#include "GoTo.h"
#include "LR1Item.h"
#include "ParsingTable.h"

class Grammar;

class GeneratedParsingTable: public ParsingTable {
public:
	GeneratedParsingTable(const Grammar* grammar);
	virtual ~GeneratedParsingTable();

	void persistToFile(std::string fileName) const;
private:
	void computeActionTable(const std::vector<std::vector<LR1Item>>& canonicalCollectionOfSetsOfItems);
	void computeGotoTable(const std::vector<std::vector<LR1Item>>& canonicalCollectionOfSetsOfItems);
	void computeErrorActions(size_t stateCount);

	void logCanonicalCollection(const std::vector<std::vector<LR1Item>>& canonicalCollectionOfSetsOfItems) const;

	FirstTable firstTable;
	GoTo goTo;
};

#endif /* GENERATEDPARSINGTABLE_H_ */