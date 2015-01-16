#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <memory>
#include <sstream>

#include "ast/AbstractSyntaxTreeVisitor.h"
#include "semantic_analyzer/SemanticAnalysisVisitor.h"
#include "ast/types/BaseType.h"
#include "ast/TypeSpecifier.h"
#include "ast/DirectDeclaration.h"
#include "ast/DereferencedDeclaration.h"
#include "ast/ParameterDeclaration.h"

namespace {

using testing::Eq;
using testing::IsEmpty;

using namespace semantic_analyzer;
using namespace ast;

translation_unit::Context context { "file", 42 };

namespace functionArgument {

class DeclarationStub: public DirectDeclaration {
public:

    DeclarationStub() :
            DirectDeclaration { "declarationStub", ::context }
    {
    }

    void accept(ast::AbstractSyntaxTreeVisitor&) override {
    }
};

TEST(SemanticAnalysisVisitor_visitingFunctionArgument, errsForVoidArgument) {
    TypeSpecifier type { BaseType::newVoid(), { } };
    DeclarationStub declaration { };
    ParameterDeclaration functionArgument { type, std::make_unique<DeclarationStub>() };
    std::ostringstream errors;

    SemanticAnalysisVisitor visitor { &errors };

    visitor.visit(functionArgument);

    EXPECT_THAT(visitor.successfulSemanticAnalysis(), Eq(false));
    EXPECT_THAT(errors.str(), Eq("file:42: error: function argument ‘declarationStub’ declared void\n"));
}

TEST(SemanticAnalysisVisitor_visitingFunctionArgument, succeedsForVoidPointerArgument) {
    TypeSpecifier type { BaseType::newVoid(), { } };
    DeclarationStub declaration { };
    ParameterDeclaration functionArgument { type, std::make_unique<DereferencedDeclaration>(std::make_unique<DeclarationStub>()) };
    std::ostringstream errors;

    SemanticAnalysisVisitor visitor { &errors };

    visitor.visit(functionArgument);

    EXPECT_THAT(visitor.successfulSemanticAnalysis(), Eq(true));
    EXPECT_THAT(errors.str(), IsEmpty());
}

}

}
