#include "LR1Parser.h"

#include <cstdlib>
#include <fstream>

#include "../code_generator/symbol_entry.h"
#include "../code_generator/symbol_table.h"
#include "../driver/TranslationUnit.h"
#include "../scanner/Token.h"
#include "../semantic_analyzer/a_expr_node.h"
#include "../semantic_analyzer/a_expressions_node.h"
#include "../semantic_analyzer/add_expr_node.h"
#include "../semantic_analyzer/and_expr_node.h"
#include "../semantic_analyzer/block_node.h"
#include "../semantic_analyzer/cast_expr_node.h"
#include "../semantic_analyzer/decl_node.h"
#include "../semantic_analyzer/decls_node.h"
#include "../semantic_analyzer/dir_decl_node.h"
#include "../semantic_analyzer/eq_expr_node.h"
#include "../semantic_analyzer/factor_node.h"
#include "../semantic_analyzer/func_decl_node.h"
#include "../semantic_analyzer/io_stmt_node.h"
#include "../semantic_analyzer/jmp_stmt_node.h"
#include "../semantic_analyzer/log_and_expr_node.h"
#include "../semantic_analyzer/loop_hdr_node.h"
#include "../semantic_analyzer/matched_node.h"
#include "../semantic_analyzer/ml_expr_node.h"
#include "../semantic_analyzer/or_expr_node.h"
#include "../semantic_analyzer/param_list_node.h"
#include "../semantic_analyzer/postfix_expr_node.h"
#include "../semantic_analyzer/ptr_node.h"
#include "../semantic_analyzer/s_expr_node.h"
#include "../semantic_analyzer/SyntaxTree.h"
#include "../semantic_analyzer/SemanticComponentsFactory.h"
#include "../semantic_analyzer/SyntaxTreeBuilder.h"
#include "../semantic_analyzer/term_node.h"
#include "../semantic_analyzer/u_expr_node.h"
#include "../semantic_analyzer/unmatched_node.h"
#include "../semantic_analyzer/var_decl_node.h"
#include "../semantic_analyzer/xor_expr_node.h"
#include "action.h"
#include "ParsingTable.h"
#include "rule.h"
#include "terminal_node.h"

#define EVER ;;

using std::unique_ptr;
using std::cerr;
using std::cout;
using std::endl;

bool LR1Parser::log = false;
ofstream LR1Parser::logfile;

LR1Parser::LR1Parser(ParsingTable* parsingTable, SemanticComponentsFactory* semanticComponentsFactory) :
		parsingTable { parsingTable },
		semanticComponentsFactory { semanticComponentsFactory } {
	output = NULL;
	token = NULL;
	next_token = NULL;
	if (log) {
		configure_logging();
	}
	parsing_stack.push(0);
}

LR1Parser::~LR1Parser() {
	if (logfile.is_open()) {
		logfile.close();
	}
}

unique_ptr<SyntaxTree> LR1Parser::parse(TranslationUnit& translationUnit) {
	unique_ptr<SyntaxTreeBuilder> syntaxTreeBuilder { semanticComponentsFactory->newSyntaxTreeBuilder() };
	syntaxTreeBuilder->withSourceFileName(translationUnit.getFileName());
	syntax_tree = new SyntaxTree();
	syntax_tree->setFileName(translationUnit.getFileName().c_str());
	current_scope = syntax_tree->getSymbolTable();
	success = true;
	can_forge = true;
	token = new Token { translationUnit.getNextToken() };
	Action *action = NULL;
	for (EVER) {
		long top = parsing_stack.top();
		// FIXME: adjust parsing table
		if (token->getId() != 0) {
			action = parsingTable->action(top, token->getId());
		} else {
			action = parsingTable->action(top, -1);
		}
		if (action != NULL) {
			switch (action->which()) {
			case 's':
				shift(action, translationUnit);
				continue;
			case 'a':       // accept
				if (success) {
					syntax_tree->setTree(syntax_stack.top());
					if (log) {
						log_syntax_tree();
						syntax_tree->printTables();
						syntax_tree->logCode();
					}
					return unique_ptr<SyntaxTree> { syntax_tree };
				}
				throw std::runtime_error("Parsing failed");
			case 'r':
				reduce(action);
				action = NULL;
				continue;
			case 'e':
				success = false;
				error(action, translationUnit);
				action = NULL;
				continue;
			default:
				action->error(token);
				cerr << "error" << endl;
				if (output != NULL) {
					output->close();
					delete output;
				}
				throw std::runtime_error("Unrecognized action");
			}
		} else {
			throw std::runtime_error("NULL entry in action table!");
		}
	}
	if (output != NULL) {
		output->close();
		delete output;
	}
	throw std::runtime_error("Parsing failed");
}

void LR1Parser::set_logging(const char *lf) {
	log = true;
	string logpath = "logs/";
	logpath += lf;
	logfile.open(logpath.c_str());
	if (!logfile.is_open()) {
		cerr << "Unable to open log file for writing! Filename: " << lf << endl;
		log = false;
	}
}

void LR1Parser::configure_logging() {
	const char *outfile = "logs/parser.out";
	output = new ofstream;
	output->open(outfile);
	if (!output->is_open()) {
		cerr << "Unable to create parser output file! Filename: " << outfile << endl;
		delete output;
		output = NULL;
	}
}

void LR1Parser::shift(Action *action, TranslationUnit& translationUnit) {
	if (log) {
		*output << "Stack: " << parsing_stack.top() << "\tpush " << action->getState() << "\t\tlookahead: " << token->getLexeme() << endl;
	}
	parsing_stack.push(action->getState());
	if (success) {
		TerminalNode *t_node = new TerminalNode(parsingTable->getTerminalById(token->getId()), token);
		adjustScope();
		line = token->line;
		syntax_tree->setLine(line);
		syntax_stack.push(t_node);
	}
	if (next_token != NULL) {
		token = next_token;
		next_token = NULL;
	} else {
		token = new Token { translationUnit.getNextToken() };
		can_forge = true;
	}
	action = NULL;
}

void LR1Parser::reduce(Action *action) {
	Rule *reduction = NULL;
	Action *gt = NULL;
	reduction = action->getReduction();
	vector<Node *> right_side;
	if (reduction != NULL) {
		if (log) {
			reduction->log(*output);
		}
	} else {
		throw std::runtime_error("NULL reduction found!");
	}
	for (unsigned i = reduction->getRight()->size(); i > 0; i--) {
		if (log) {
			*output << "Stack: " << parsing_stack.top() << "\tpop " << parsing_stack.top() << "\t\t";
			if (i > 1) {
				*output << endl;
			}
		}
		if (success) {
			right_side.push_back(syntax_stack.top());
			syntax_stack.pop();
		}
		parsing_stack.pop();
	}
	gt = parsingTable->go_to(parsing_stack.top(), *reduction->getLeft());
	if (gt != NULL) {
		if (log) {
			*output << "Stack: " << parsing_stack.top() << "\tpush " << gt->getState() << "\t\tlookahead: " << token->getLexeme() << endl;
		}
		if (success) {
			mknode(*reduction->getLeft(), right_side, reduction->rightStr());
		}
		parsing_stack.push(gt->getState());
		action = NULL;
	} else {
		throw std::runtime_error("NULL goto entry found!");
	}
}

void LR1Parser::error(Action *action, TranslationUnit& translationUnit) {
	if (action->error(token)) {
		if (action->getForge() != 0 && can_forge) {
			next_token = token;
			token = new Token(action->getForge(), "");
			if (log) {
				cerr << "Inserting " << action->getExpected() << " into input stream.\n";
			}
			can_forge = false;
		} else {
			parsing_stack.push(action->getState());
			token = new Token { translationUnit.getNextToken() };
			if (log) {
				cerr << "Stack: " << parsing_stack.top() << "\tpush " << action->getState() << "\t\tlookahead: " << token->getLexeme()
						<< endl;
			}
		}
	} else {
		throw std::runtime_error("Parsing failed!");
	}
}

void LR1Parser::mknode(string left, vector<Node *> children, string reduction) {
	Node *n_node = NULL;
	if (parsingTable->isCustomGrammar()) {
		n_node = new NonterminalNode(left, children, reduction);
	} else {
		if (left == "<u_op>")
			n_node = new CarrierNode(left, children);
		else if (left == "<m_op>")
			n_node = new CarrierNode(left, children);
		else if (left == "<add_op>")
			n_node = new CarrierNode(left, children);
		else if (left == "<s_op>")
			n_node = new CarrierNode(left, children);
		else if (left == "<ml_op>")
			n_node = new CarrierNode(left, children);
		else if (left == "<eq_op>")
			n_node = new CarrierNode(left, children);
		else if (left == "<a_op>")
			n_node = new CarrierNode(left, children);
		else if (left == "<term>")
			n_node = new TermNode(left, children, reduction, current_scope, line);
		else if (left == "<postfix_expr>")
			n_node = new PostfixExprNode(left, children, reduction, current_scope, line);
		else if (left == "<u_expr>")
			n_node = new UExprNode(left, children, reduction, current_scope, line);
		else if (left == "<cast_expr>")
			n_node = new CastExprNode(left, children, reduction, current_scope, line);
		else if (left == "<factor>")
			n_node = new FactorNode(left, children, reduction, current_scope, line);
		else if (left == "<add_expr>")
			n_node = new AddExprNode(left, children, reduction, current_scope, line);
		else if (left == "<s_expr>")
			n_node = new SExprNode(left, children, reduction, current_scope, line);
		else if (left == "<ml_expr>")
			n_node = new MLExprNode(left, children, reduction, current_scope, line);
		else if (left == "<eq_expr>")
			n_node = new EQExprNode(left, children, reduction, current_scope, line);
		else if (left == "<and_expr>")
			n_node = new AndExprNode(left, children, reduction, current_scope, line);
		else if (left == "<xor_expr>")
			n_node = new XorExprNode(left, children, reduction, current_scope, line);
		else if (left == "<or_expr>")
			n_node = new OrExprNode(left, children, reduction, current_scope, line);
		else if (left == "<log_and_expr>")
			n_node = new LogAndExprNode(left, children, reduction, current_scope, line);
		else if (left == "<log_expr>")
			n_node = new LogExprNode(left, children, reduction, current_scope, line);
		else if (left == "<a_expressions>")
			n_node = new AExpressionsNode(left, children, reduction);
		else if (left == "<a_expr>")
			n_node = new AExprNode(left, children, reduction, current_scope, line);
		else if (left == "<expr>")
			n_node = new ExprNode(left, children, reduction, current_scope, line);
		else if (left == "<jmp_stmt>")
			n_node = new JmpStmtNode(left, children, reduction, current_scope, line);
		else if (left == "<io_stmt>")
			n_node = new IOStmtNode(left, children, reduction, current_scope, line);
		else if (left == "<loop_hdr>")
			n_node = new LoopHdrNode(left, children, reduction, current_scope, line);
		else if (left == "<unmatched>")
			n_node = new UnmatchedNode(left, children, reduction, current_scope, line);
		else if (left == "<matched>")
			n_node = new MatchedNode(left, children, reduction, current_scope, line);
		else if (left == "<stmt>")
			n_node = new CarrierNode(left, children);
		else if (left == "<statements>")
			n_node = new CarrierNode(left, children);
		else if (left == "<param_decl>")
			n_node = new ParamDeclNode(left, children, reduction, current_scope, line);
		else if (left == "<param_list>")
			n_node = new ParamListNode(left, children, reduction);
		else if (left == "<dir_decl>") {
			n_node = new DirDeclNode(left, children, reduction, current_scope, line);
			params = ((DirDeclNode *) n_node)->getParams();
		} else if (left == "<ptr>")
			n_node = new PtrNode(left, children, reduction);
		else if (left == "<block>")
			n_node = new BlockNode(left, children);
		else if (left == "<decl>")
			n_node = new DeclNode(left, children, reduction);
		else if (left == "<decls>")
			n_node = new DeclsNode(left, children, reduction);
		else if (left == "<type_spec>")
			n_node = new CarrierNode(left, children);
		else if (left == "<var_decl>")
			n_node = new VarDeclNode(left, children, reduction, current_scope, line);
		else if (left == "<func_decl>")
			n_node = new FuncDeclNode(left, children, reduction, current_scope, line);
		else if (left == "<var_decls>")
			n_node = new CarrierNode(left, children);
		else if (left == "<func_decls>")
			n_node = new CarrierNode(left, children);
		else if (left == "<program>")
			n_node = new CarrierNode(left, children);
		else {
			cerr << "Error! Syntax node matching nonterminal " << left << "found!\n";
			return;
		}
	}
	if (true == n_node->getErrorFlag()) {
		syntax_tree->setErrorFlag();
	}
	syntax_stack.push(n_node);
}

void LR1Parser::adjustScope() {
	if (!parsingTable->isCustomGrammar()) {
		if (token->getLexeme() == "{") {
			current_scope = current_scope->newScope();
			if (params.size()) {
				for (unsigned i = 0; i < params.size(); i++) {
					current_scope->insertParam(params[i]->getPlace()->getName(), params[i]->getPlace()->getBasicType(),
							params[i]->getPlace()->getExtendedType(), line);
				}
				params.clear();
			}
		} else if (token->getLexeme() == "}") {
			current_scope = current_scope->getOuterScope();
		}
	}
}

void LR1Parser::log_syntax_tree() const {
	const char *outfile = "logs/syntax_tree.xml";
	ofstream xmlfile;
	xmlfile.open(outfile);
	if (!xmlfile.is_open()) {
		cerr << "Unable to create syntax tree xml file! Filename: " << outfile << endl;
		return;
	}
	xmlfile << syntax_tree->asXml();
}
