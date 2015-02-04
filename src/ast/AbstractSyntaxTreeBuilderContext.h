#ifndef ABSTRACTSYNTAXTREEBUILDERCONTEXT_H_
#define ABSTRACTSYNTAXTREEBUILDERCONTEXT_H_

#include <memory>
#include <stack>
#include <utility>
#include <vector>

#include "Constant.h"
#include "DeclarationSpecifiers.h"
#include "FormalArgument.h"
#include "FunctionDeclarator.h"
#include "StorageSpecifier.h"
#include "TerminalSymbol.h"
#include "TypeQualifier.h"
#include "TypeSpecifier.h"

namespace ast {

class Declaration;
class DeclarationSpecifiers;
class Expression;
class ArgumentExpressionList;
class TerminalSymbol;
class Pointer;
class AbstractSyntaxTreeNode;
class LoopHeader;
class Declarator;
class DeclarationList;
class FormalArgument;
class VariableDeclaration;
class ListCarrier;

class AbstractSyntaxTreeBuilderContext {
public:
    AbstractSyntaxTreeBuilderContext();
    virtual ~AbstractSyntaxTreeBuilderContext();

    void pushTerminal(TerminalSymbol terminal);
    TerminalSymbol popTerminal();

    void pushTypeSpecifier(TypeSpecifier typeSpecifier);
    TypeSpecifier popTypeSpecifier();

    void pushStorageSpecifier(StorageSpecifier storageSpecifier);
    StorageSpecifier popStorageSpecifier();

    void pushTypeQualifier(TypeQualifier typeQualifier);
    TypeQualifier popTypeQualifier();

    void newTypeQualifierList(TypeQualifier qualifier);
    void addToTypeQualifierList(TypeQualifier qualifier);
    std::vector<TypeQualifier> popTypeQualifierList();

    void pushConstant(Constant constant);
    Constant popConstant();

    void pushExpression(std::unique_ptr<Expression> expression);
    std::unique_ptr<Expression> popExpression();

    void newActualArgumentsList(std::unique_ptr<Expression> argument);
    void addToActualArgumentsList(std::unique_ptr<Expression> argument);
    std::vector<std::unique_ptr<Expression>> popActualArgumentsList();

    void pushPointer(Pointer pointer);
    Pointer popPointer();

    void pushLoopHeader(std::unique_ptr<LoopHeader> loopHeader);
    std::unique_ptr<LoopHeader> popLoopHeader();

    void pushStatement(std::unique_ptr<AbstractSyntaxTreeNode> statement);
    std::unique_ptr<AbstractSyntaxTreeNode> popStatement();

    void pushDeclarator(std::unique_ptr<Declarator> declarator);
    std::unique_ptr<Declarator> popDeclarator();

    void pushInitializedDeclarators(std::vector<std::unique_ptr<Declarator>> declarators);
    std::vector<std::unique_ptr<Declarator>> popInitializedDeclarators();

    void pushDeclarationList(std::vector<std::unique_ptr<Declaration>> declarationList);
    std::vector<std::unique_ptr<Declaration>> popDeclarationList();

    void pushFunctionDeclaration(std::unique_ptr<FunctionDeclarator> declaration);
    std::unique_ptr<FunctionDeclarator> popFunctionDeclaration();

    void pushFormalArgument(FormalArgument formalArgument);
    FormalArgument popFormalArgument();

    void pushFormalArguments(FormalArguments formalArguments);
    FormalArguments popFormalArguments();

    void pushArgumentsDeclaration(std::pair<FormalArguments, bool> argumentsDeclaration);
    std::pair<FormalArguments, bool> popArgumentsDeclaration();

    void pushListCarrier(std::unique_ptr<ListCarrier> carrier);
    std::unique_ptr<ListCarrier> popListCarrier();

    void pushDeclarationSpecifiers(DeclarationSpecifiers declarationSpecifiers);
    DeclarationSpecifiers popDeclarationSpecifiers();

    void pushDeclaration(std::unique_ptr<Declaration> declaration);
    std::unique_ptr<Declaration> popDeclaration();

    void newStatementList(std::unique_ptr<AbstractSyntaxTreeNode> statement);
    void addToStatementList(std::unique_ptr<AbstractSyntaxTreeNode> statement);
    std::vector<std::unique_ptr<AbstractSyntaxTreeNode>> popStatementList();

    void pushExternalDeclaration(std::unique_ptr<AbstractSyntaxTreeNode> externalDeclaration);
    std::unique_ptr<AbstractSyntaxTreeNode> popExternalDeclaration();

    void addToTranslationUnit(std::unique_ptr<AbstractSyntaxTreeNode> externalDeclaration);
    std::vector<std::unique_ptr<AbstractSyntaxTreeNode>> popTranslationUnit();

private:
    std::stack<TerminalSymbol> terminalSymbols;

    std::stack<TypeSpecifier> typeSpecifiers;
    std::stack<StorageSpecifier> storageSpecifiers;
    std::stack<TypeQualifier> typeQualifiers;
    std::stack<std::vector<TypeQualifier>> typeQualifierLists;
    std::stack<Constant> constants;

    std::stack<DeclarationSpecifiers> declarationSpecifiersStack;

    std::stack<std::unique_ptr<Expression>> expressionStack;
    std::stack<std::vector<std::unique_ptr<Expression>>>actualArgumentLists;
    std::stack<Pointer> pointerStack;
    std::stack<std::unique_ptr<LoopHeader>> loopHeaderStack;
    std::stack<std::unique_ptr<AbstractSyntaxTreeNode>> statementStack;
    std::stack<std::unique_ptr<Declarator>> declarators;
    std::stack<std::vector<std::unique_ptr<Declarator>>>initializedDeclarators;
    std::stack<std::unique_ptr<FunctionDeclarator>> functionDeclarationStack;
    std::stack<FormalArgument> formalArguments;
    std::stack<FormalArguments> formalArgumentLists;
    std::stack<std::pair<FormalArguments, bool>> argumentsDeclarations;
    std::stack<std::unique_ptr<ListCarrier>> listCarrierStack;
    std::stack<std::unique_ptr<Declaration>> declarations;
    std::stack<std::vector<std::unique_ptr<Declaration>>> declarationLists;
    std::stack<std::vector<std::unique_ptr<AbstractSyntaxTreeNode>>> statementLists;
    std::stack<std::unique_ptr<AbstractSyntaxTreeNode>> externalDeclarations;
    std::vector<std::unique_ptr<AbstractSyntaxTreeNode>> translationUnit;
};

}
/* namespace ast */

#endif /* ABSTRACTSYNTAXTREEBUILDERCONTEXT_H_ */
