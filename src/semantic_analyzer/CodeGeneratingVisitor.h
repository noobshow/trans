#ifndef CODEGENERATINGVISITOR_H_
#define CODEGENERATINGVISITOR_H_

#include <vector>

#include "../code_generator/quadruple.h"
#include "AbstractSyntaxTreeVisitor.h"
#include "AssignmentExpressionList.h"
#include "Block.h"
#include "DeclarationList.h"
#include "Identifier.h"
#include "JumpStatement.h"
#include "ListCarrier.h"
#include "ParameterList.h"
#include "Pointer.h"
#include "ReturnStatement.h"
#include "TranslationUnit.h"

namespace semantic_analyzer {

class CodeGeneratingVisitor: public AbstractSyntaxTreeVisitor {
public:
    CodeGeneratingVisitor();
    virtual ~CodeGeneratingVisitor();

    void visit(TypeSpecifier& typeSpecifier) override;

    void visit(ParameterList& parameterList) override;
    void visit(AssignmentExpressionList& expressions) override;
    void visit(DeclarationList& declarations) override;
    void visit(ArrayAccess& arrayAccess) override;
    void visit(FunctionCall& functionCall) override;
    void visit(Term& term) override;
    void visit(PostfixExpression& expression) override;
    void visit(PrefixExpression& expression) override;
    void visit(UnaryExpression& expression) override;
    void visit(TypeCast& expression) override;
    void visit(PointerCast& expression) override;
    void visit(ArithmeticExpression& expression) override;
    void visit(ShiftExpression& expression) override;
    void visit(ComparisonExpression& expression) override;
    void visit(BitwiseExpression& expression) override;
    void visit(LogicalAndExpression& expression) override;
    void visit(LogicalOrExpression& expression) override;
    void visit(AssignmentExpression& expression) override;
    void visit(ExpressionList& expression) override;

    void visit(JumpStatement& statement) override;
    void visit(ReturnStatement& statement) override;
    void visit(IOStatement& statement) override;
    void visit(IfStatement& statement) override;
    void visit(IfElseStatement& statement) override;
    void visit(LoopStatement& statement) override;

    void visit(ForLoopHeader& loopHeader) override;
    void visit(WhileLoopHeader& loopHeader) override;

    void visit(Pointer& pointer) override;

    void visit(Identifier& identifier) override;
    void visit(FunctionDeclaration& declaration) override;
    void visit(ArrayDeclaration& declaration) override;

    void visit(ParameterDeclaration& parameter) override;

    void visit(FunctionDefinition& function) override;

    void visit(VariableDeclaration& declaration) override;
    void visit(VariableDefinition& definition) override;

    void visit(Block& block) override;
    void visit(ListCarrier& listCarrier) override;

    void visit(TranslationUnit& translationUnit) override;

    std::vector<Quadruple> getQuadruples() const;

private:
    std::vector<Quadruple> quadruples;
};

} /* namespace semantic_analyzer */

#endif /* CODEGENERATINGVISITOR_H_ */