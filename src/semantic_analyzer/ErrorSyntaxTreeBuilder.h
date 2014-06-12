#ifndef ERRORSYNTAXTREEBUILDER_H_
#define ERRORSYNTAXTREEBUILDER_H_

#include <memory>
#include <string>

#include "ParseTreeBuilder.h"

class ErrorSyntaxTreeBuilder: public ParseTreeBuilder {
public:
	ErrorSyntaxTreeBuilder();
	virtual ~ErrorSyntaxTreeBuilder();

	std::unique_ptr<SyntaxTree> build();

	void makeTerminalNode(std::string, std::string);
	void makeNonTerminalNode(std::string, int, std::string);
};

#endif /* ERRORSYNTAXTREEBUILDER_H_ */
