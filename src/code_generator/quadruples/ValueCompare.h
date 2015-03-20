#ifndef VALUECOMPARE_H_
#define VALUECOMPARE_H_

#include <string>

#include "Quadruple.h"

namespace code_generator {

class ValueCompare: public Quadruple {
public:
    ValueCompare(std::string leftSymbolName, std::string rightSymbolName);
    virtual ~ValueCompare() = default;

    void generateCode(AssemblyGenerator& generator) const override;

    std::string getLeftSymbolName() const;
    std::string getRightSymbolName() const;

private:
    std::string leftSymbolName;
    std::string rightSymbolName;
};

} /* namespace code_generator */

#endif /* VALUECOMPARE_H_ */
