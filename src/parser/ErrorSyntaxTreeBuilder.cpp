#include "ErrorSyntaxTreeBuilder.h"

#include <memory>
#include <stdexcept>

using std::string;

namespace parser {

ErrorSyntaxTreeBuilder::ErrorSyntaxTreeBuilder() {
}

ErrorSyntaxTreeBuilder::~ErrorSyntaxTreeBuilder() {
}

std::unique_ptr<SyntaxTree> ErrorSyntaxTreeBuilder::build() {
	throw std::runtime_error { "parsing failed with syntax errors" };
}

void ErrorSyntaxTreeBuilder::makeTerminalNode(std::string, std::string, const TranslationUnitContext&) {
}

void ErrorSyntaxTreeBuilder::makeNonterminalNode(string, Production) {
}

}
