#include "EndProcedure.h"

#include "../AssemblyGenerator.h"

namespace code_generator {

EndProcedure::EndProcedure(std::string name) :
        name { name }
{
}

void EndProcedure::generateCode(AssemblyGenerator& generator) const {
    generator.generateCodeFor(*this);
}

std::string EndProcedure::getName() const {
    return name;
}

} /* namespace code_generator */