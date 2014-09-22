#include "quadruple.h"

#include "LabelEntry.h"
#include "ValueEntry.h"

namespace code_generator {

Quadruple::Quadruple(unsigned op, ValueEntry *arg1, ValueEntry *arg2, ValueEntry *res) {
    this->op = op;
    this->arg1 = arg1;
    this->arg2 = arg2;
    this->res = res;
}

Quadruple::Quadruple(std::string val, ValueEntry *res) {
    this->op = ASSIGN;
    this->constant = val;
    this->res = res;
}

Quadruple::Quadruple(unsigned op, LabelEntry *label) :
        op { op }, label { label } {
}

void Quadruple::output(std::ostream &of) const {
    switch (op) {
    case ASSIGN:
        of << "\t" << res->getName() << " := " << (arg1 ? arg1->getName() : constant) << std::endl;
        break;
    case ADD:
        of << "\t" << res->getName() << " := " << arg1->getName() << " + " << arg2->getName() << std::endl;
        break;
    case SUB:
        of << "\t" << res->getName() << " := " << arg1->getName() << " - " << arg2->getName() << std::endl;
        break;
    case INC:
        of << "\t" << "INC " << arg1->getName() << std::endl;
        break;
    case DEC:
        of << "\t" << "DEC " << arg1->getName() << std::endl;
        break;
    case MUL:
        of << "\t" << res->getName() << " := " << arg1->getName() << " * " << arg2->getName() << std::endl;
        break;
    case DIV:
        of << "\t" << res->getName() << " := " << arg1->getName() << " / " << arg2->getName() << std::endl;
        break;
    case MOD:
        of << "\t" << res->getName() << " := " << arg1->getName() << " % " << arg2->getName() << std::endl;
        break;
    case AND:
        of << "\t" << res->getName() << " := " << arg1->getName() << " AND " << arg2->getName() << std::endl;
        break;
    case OR:
        of << "\t" << res->getName() << " := " << arg1->getName() << " OR " << arg2->getName() << std::endl;
        break;
    case XOR:
        of << "\t" << res->getName() << " := " << arg1->getName() << " XOR " << arg2->getName() << std::endl;
        break;
    case NOT:
        break;
    case PARAM:
        of << "\t" << "PARAM " << arg1->getName() << std::endl;
        break;
    case CALL:
        of << "\t" << "CALL " << arg1->getName() << ", " << ((ValueEntry*) arg1)->getParamCount() << std::endl;
        break;
    case RETURN:
        of << "\t" << "RETURN " << arg1->getName() << std::endl;
        break;
    case RETRIEVE:
        of << "\t" << "RETRIEVE " << arg1->getName() << std::endl;
        break;
    case GOTO:
        of << "\t" << "GOTO " << (arg1 ? arg1->getName() : "_") << std::endl;
        break;
    case ADDR:
        of << "\t" << res->getName() << " := &" << arg1->getName() << std::endl;
        break;
    case DEREF:
        of << "\t" << res->getName() << " := *" << arg1->getName() << std::endl;
        break;
    case DEREF_LVAL:
        of << "\t" << "*" << res->getName() << " := " << arg1->getName() << std::endl;
        break;
    case UMINUS:
        of << "\t" << res->getName() << " := -" << arg1->getName() << std::endl;
        break;
    case SHR:
        of << "\t" << res->getName() << " := " << arg1->getName() << " >> " << arg2->getName() << std::endl;
        break;
    case SHL:
        of << "\t" << res->getName() << " := " << arg1->getName() << " << " << arg2->getName() << std::endl;
        break;
    case CMP:
        of << "\t" << "CMP " << arg1->getName() << ", " << (arg2 ? arg2->getName() : "0") << std::endl;
        break;
    case JE:
        of << "\t" << "JE " << (arg1 ? arg1->getName() : "_") << std::endl;
        break;
    case JNE:
        of << "\t" << "JNE " << (arg1 ? arg1->getName() : "_") << std::endl;
        break;
    case JA:
        of << "\t" << "JA " << (arg1 ? arg1->getName() : "_") << std::endl;
        break;
    case JB:
        of << "\t" << "JB " << (arg1 ? arg1->getName() : "_") << std::endl;
        break;
    case JAE:
        of << "\t" << "JAE " << (arg1 ? arg1->getName() : "_") << std::endl;
        break;
    case JBE:
        of << "\t" << "JBE" << (arg1 ? arg1->getName() : "_") << std::endl;
        break;
    case LABEL:
        of << label->getName() << ":" << std::endl;
        break;
    case INDEX:
        of << "\t" << (res ? res->getName() : "NULL") << " := " << (arg1 ? arg1->getName() : "NULL") << "[" << (arg2 ? arg2->getName() : "NULL") << "]"
                << std::endl;
        break;
    case INDEX_ADDR:
        of << "\t" << (res ? res->getName() : "NULL") << " := " << "[" << (arg2 ? arg2->getName() : "NULL") << "]" << (arg1 ? arg1->getName() : "NULL")
                << std::endl;
        break;
    case PROC:
        of << "PROC " << arg1->getName() << std::endl;
        break;
    case ENDPROC:
        of << "ENDPROC " << arg1->getName() << std::endl;
        break;
    case IN:
        of << "\tINPUT " << arg1->getName() << std::endl;
        break;
    case OUT:
        of << "\tOUTPUT " << arg1->getName() << std::endl;
        break;
    case SCOPE:
        of << "\tBEGIN SCOPE\n";
        break;
    case ENDSCOPE:
        of << "\tEND SCOPE\n";
        break;
    default:
        std::cerr << "Unidentified quadruple! (" << op << ", " << (arg1 ? arg1->getName() : "NULL") << ", " << (arg2 ? arg2->getName() : "NULL") << ", "
                << (res ? res->getName() : "NULL") << ")\n";
    }
}

void Quadruple::setArg1(ValueEntry *arg1) {
    this->arg1 = arg1;
}

unsigned Quadruple::getOp() const {
    return op;
}

ValueEntry *Quadruple::getArg1() const {
    return arg1;
}

ValueEntry *Quadruple::getArg2() const {
    return arg2;
}

ValueEntry *Quadruple::getRes() const {
    return res;
}

std::string Quadruple::getConstant() const {
    return constant;
}

LabelEntry* Quadruple::getLabel() const {
    return label;
}

void Quadruple::setOp(unsigned op) {
    this->op = op;
}

}
