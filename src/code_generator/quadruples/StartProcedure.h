#ifndef STARTPROCEDURE_H_
#define STARTPROCEDURE_H_

#include <string>

#include "Quadruple.h"

namespace code_generator {

class StartProcedure: public Quadruple {
public:
    StartProcedure(std::string name);
    virtual ~StartProcedure() = default;

    void generateCode(AssemblyGenerator& generator) const override;

    std::string getName() const;

private:
    std::string name;
};

} /* namespace code_generator */

#endif /* STARTPROCEDURE_H_ */