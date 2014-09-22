#include "ValueEntry.h"

#include <iostream>
#include <sstream>

#include "../ast/BasicType.h"

using ast::BasicType;

namespace code_generator {

ValueEntry::ValueEntry(std::string name, ast::TypeInfo typeInfo, bool tmp, unsigned l) :
        name { name }, typeInfo { typeInfo }, size { 4 }, temp { tmp }, param { false }, line { l }, offset { 0 }, stored { true } {
}

ValueEntry::~ValueEntry() {
}

ast::TypeInfo ValueEntry::getTypeInfo() const {
    return typeInfo;
}

void ValueEntry::print() const {
    std::string typeStr { };
    BasicType basicType = typeInfo.getBasicType();
    int extended_type = typeInfo.getDereferenceCount();

    switch (basicType) {
    case BasicType::INTEGER:
        typeStr = "integer";
        break;
    case BasicType::CHARACTER:
        typeStr = "character";
        break;
    case BasicType::FLOAT:
        typeStr = "float";
        break;
    case BasicType::VOID:
        typeStr = "void";
        break;
    }

    std::cout << "\t" << name << "\t" << typeStr << "\t" << std::to_string(extended_type) << "\t" << (temp ? "temp" : "") << "\t" << offset << "\t"
            << getStorage() << std::endl;
}

std::vector<ValueEntry*> ValueEntry::getParams() const {
    return params;
}

unsigned ValueEntry::getParamCount() const {
    return params.size();
}

void ValueEntry::addParam(ValueEntry *param) {
    params.push_back(param);
}

bool ValueEntry::isTemp() const {
    return temp;
}

unsigned ValueEntry::getLine() const {
    return line;
}

unsigned ValueEntry::getOffset() const {
    return offset;
}

void ValueEntry::setOffset(unsigned offset) {
    this->offset = offset;
}

std::string ValueEntry::getOffsetReg() const {
    if (param)
        return "ebp";
    else
        return "esp";
}

std::string ValueEntry::getStorage() const {
    std::ostringstream oss;
    if (param)
        oss << "[ebp";
    else
        oss << "[esp";
    if (offset)
        oss << " + " << offset;
    oss << "]";
    return oss.str();
}

void ValueEntry::setParam() {
    param = true;
}

void ValueEntry::update(std::string reg) {
    if ("" != reg) {
        value.push_back(reg);
        stored = false;
    } else {
        value.clear();
        stored = true;
    }
}

std::string ValueEntry::store() {
    std::string ret = "";
    if (!stored) {
        ret = "\tmov ";
        ret += getStorage();
        ret += ", ";
        ret += value.at(0);
    }
    return ret;
}

unsigned ValueEntry::getSize() const {
    return size;
}

bool ValueEntry::isStored() const {
    return stored;
}

void ValueEntry::removeReg(std::string reg) {
    std::vector<std::string> newVal;
    for (unsigned i = 0; i < value.size(); i++)
        if (value.at(i) != reg)
            newVal.push_back(value.at(i));
    value = newVal;
}

bool ValueEntry::isParam() const {
    return param;
}

std::string ValueEntry::getName() const {
    return name;
}


std::string ValueEntry::getValue() const {
    if (value.size()) {
        return value.at(0);
    }
    return "";
}

} /* namespace code_generator */
