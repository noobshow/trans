#ifndef _TERMINAL_NODE_H_
#define _TERMINAL_NODE_H_

#include <string>

#include "ParseTreeNode.h"

namespace parser {

class TerminalNode: public ParseTreeNode {
public:
	TerminalNode(std::string type, std::string value);
	virtual ~TerminalNode() = default;

	std::string getValue() const;

	void accept(const ParseTreeNodeVisitor& visitor) const;

private:
	std::string value;
};

}

#endif // _TERMINAL_NODE_H_
