#include "codegen/ATandTInstructionSet.h"

#include "Register.h"

#include <stdexcept>
#include <string>

namespace {

using codegen::Register;

std::string memoryOffsetMnemonic(const Register& memoryBase, int memoryOffset) {
    return (memoryOffset ? "-" + std::to_string(memoryOffset) : "") + "(%" + memoryBase.getName() + ")";
}

}

namespace codegen {

std::string ATandTInstructionSet::preamble() const {
    return ".extern scanf\n"
            ".extern printf\n\n"
            ".data\n"
            "sfmt: .string \"%d\"\n"
            "fmt: .string \"%d\n\"\n\n"
            ".text\n"
            ".globl main\n\n";
}

std::string ATandTInstructionSet::call(std::string procedureName) const {
    return "call " + procedureName + "\n";
}

std::string ATandTInstructionSet::push(const Register& reg) const {
    throw std::runtime_error { "not implemented ATandTInstructionSet::push(const Register& reg)" };
}

std::string ATandTInstructionSet::pop(const Register& reg) const {
    throw std::runtime_error { "not implemented ATandTInstructionSet::pop(const Register& reg)" };
}

std::string ATandTInstructionSet::add(const Register& reg, int constant) const {
    throw std::runtime_error { "not implemented ATandTInstructionSet::add(const Register& reg, int constant)" };
}

std::string ATandTInstructionSet::sub(const Register& reg, int constant) const {
    throw std::runtime_error { "not implemented ATandTInstructionSet::sub(const Register& reg, int constant)" };
}

std::string ATandTInstructionSet::not_(const Register& reg) const {
    throw std::runtime_error { "not implemented ATandTInstructionSet::not_(const Register& reg)" };
}

std::string ATandTInstructionSet::mov(const Register& source, const Register& memoryBase, int memoryOffset) const {
    return "movq %" + source.getName() + ", " + memoryOffsetMnemonic(memoryBase, memoryOffset) + "\n";
}

std::string ATandTInstructionSet::mov(const Register& source, const Register& destination) const {
    throw std::runtime_error { "not implemented ATandTInstructionSet::mov(const Register& source, const Register& destination)" };
}

std::string ATandTInstructionSet::mov(const Register& memoryBase, int memoryOffset, const Register& destination) const {
    throw std::runtime_error {
            "not implemented ATandTInstructionSet::mov(const Register& memoryBase, int memoryOffset, const Register& destination)" };
}

std::string ATandTInstructionSet::mov(std::string constant, const Register& memoryBase, int memoryOffset) const {
    throw std::runtime_error {
            "not implemented ATandTInstructionSet::mov(std::string constant, const Register& memoryBase, int memoryOffset)" };
}

std::string ATandTInstructionSet::mov(std::string constant, const Register& destination) const {
    throw std::runtime_error { "not implemented ATandTInstructionSet::mov(std::string constant, const Register& destination)" };
}

std::string ATandTInstructionSet::cmp(const Register& leftArgument, const Register& memoryBase, int memoryOffset) const {
    throw std::runtime_error {
            "not implemented ATandTInstructionSet::cmp(const Register& leftArgument, const Register& memoryBase, int memoryOffset)" };
}

std::string ATandTInstructionSet::cmp(const Register& leftArgument, const Register& rightArgument) const {
    throw std::runtime_error { "not implemented ATandTInstructionSet::cmp(const Register& leftArgument, const Register& rightArgument)" };
}

std::string ATandTInstructionSet::cmp(const Register& memoryBase, int memoryOffset, const Register& rightArgument) const {
    throw std::runtime_error {
            "not implemented ATandTInstructionSet::cmp(const Register& memoryBase, int memoryOffset, const Register& rightArgument)" };
}

std::string ATandTInstructionSet::cmp(const Register& argument, int constant) const {
    throw std::runtime_error { "not implemented ATandTInstructionSet::cmp(const Register& argument, int constant)" };
}

std::string ATandTInstructionSet::cmp(const Register& memoryBase, int memoryOffset, int constant) const {
    throw std::runtime_error { "not implemented ATandTInstructionSet::cmp(const Register& memoryBase, int memoryOffset, int constant)" };
}

std::string ATandTInstructionSet::label(std::string name) const {
    throw std::runtime_error { "not implemented ATandTInstructionSet::label(std::string name)" };
}

std::string ATandTInstructionSet::jmp(std::string label) const {
    throw std::runtime_error { "not implemented ATandTInstructionSet::jmp(std::string label)" };
}

std::string ATandTInstructionSet::je(std::string label) const {
    throw std::runtime_error { "not implemented ATandTInstructionSet::je(std::string label)" };
}

std::string ATandTInstructionSet::jne(std::string label) const {
    throw std::runtime_error { "not implemented ATandTInstructionSet::jne(std::string label)" };
}

std::string ATandTInstructionSet::jg(std::string label) const {
    throw std::runtime_error { "not implemented ATandTInstructionSet::jg(std::string label)" };
}

std::string ATandTInstructionSet::jl(std::string label) const {
    throw std::runtime_error { "not implemented ATandTInstructionSet::jl(std::string label)" };
}

std::string ATandTInstructionSet::jge(std::string label) const {
    throw std::runtime_error { "not implemented ATandTInstructionSet::jge(std::string label)" };
}

std::string ATandTInstructionSet::jle(std::string label) const {
    throw std::runtime_error { "not implemented ATandTInstructionSet::jle(std::string label)" };
}

std::string ATandTInstructionSet::syscall() const {
    throw std::runtime_error { "not implemented ATandTInstructionSet::syscall()" };
}

std::string ATandTInstructionSet::leave() const {
    throw std::runtime_error { "not implemented ATandTInstructionSet::leave()" };
}

std::string ATandTInstructionSet::ret() const {
    throw std::runtime_error { "not implemented ATandTInstructionSet::ret()" };
}

std::string ATandTInstructionSet::xor_(const Register& operand, const Register& result) const {
    throw std::runtime_error { "not implemented ATandTInstructionSet::xor_(const Register& operand, const Register& result) " };
}

std::string ATandTInstructionSet::xor_(const Register& operandBase, int operandOffset, const Register& result) const {
    throw std::runtime_error {
            "not implemented ATandTInstructionSet::xor_(const Register& operandBase, int operandOffset, const Register& result)" };
}

std::string ATandTInstructionSet::or_(const Register& operand, const Register& result) const {
    throw std::runtime_error { "not implemented ATandTInstructionSet::or_(const Register& operand, const Register& result)" };
}

std::string ATandTInstructionSet::or_(const Register& operandBase, int operandOffset, const Register& result) const {
    throw std::runtime_error {
            "not implemented ATandTInstructionSet::or_(const Register& operandBase, int operandOffset, const Register& result)" };
}

std::string ATandTInstructionSet::and_(const Register& operand, const Register& result) const {
    throw std::runtime_error { "not implemented ATandTInstructionSet::and_(const Register& operand, const Register& result)" };
}

std::string ATandTInstructionSet::and_(const Register& operandBase, int operandOffset, const Register& result) const {
    throw std::runtime_error {
            "not implemented ATandTInstructionSet::and_(const Register& operandBase, int operandOffset, const Register& result)" };
}

std::string ATandTInstructionSet::add(const Register& operand, const Register& result) const {
    throw std::runtime_error { "not implemented ATandTInstructionSet::add(const Register& operand, const Register& result)" };
}

std::string ATandTInstructionSet::add(const Register& operandBase, int operandOffset, const Register& result) const {
    throw std::runtime_error {
            "not implemented ATandTInstructionSet::add(const Register& operandBase, int operandOffset, const Register& result)" };
}

std::string ATandTInstructionSet::sub(const Register& operand, const Register& result) const {
    throw std::runtime_error { "not implemented ATandTInstructionSet::sub(const Register& operand, const Register& result)" };
}

std::string ATandTInstructionSet::sub(const Register& operandBase, int operandOffset, const Register& result) const {
    throw std::runtime_error {
            "not implemented ATandTInstructionSet::sub(const Register& operandBase, int operandOffset, const Register& result)" };
}

std::string ATandTInstructionSet::imul(const Register& operand) const {
    throw std::runtime_error { "not implemented ATandTInstructionSet::imul(const Register& operand)" };
}

std::string ATandTInstructionSet::imul(const Register& operandBase, int operandOffset) const {
    throw std::runtime_error { "not implemented ATandTInstructionSet::imul(const Register& operandBase, int operandOffset)" };
}

std::string ATandTInstructionSet::idiv(const Register& operand) const {
    throw std::runtime_error { "not implemented ATandTInstructionSet::idiv(const Register& operand)" };
}

std::string ATandTInstructionSet::idiv(const Register& operandBase, int operandOffset) const {
    throw std::runtime_error { "not implemented ATandTInstructionSet::idiv(const Register& operandBase, int operandOffset)" };
}

std::string ATandTInstructionSet::inc(const Register& operand) const {
    throw std::runtime_error { "not implemented ATandTInstructionSet::inc(const Register& operand)" };
}

std::string ATandTInstructionSet::inc(const Register& operandBase, int operandOffset) const {
    throw std::runtime_error { "not implemented ATandTInstructionSet::inc(const Register& operandBase, int operandOffset)" };
}

std::string ATandTInstructionSet::dec(const Register& operand) const {
    throw std::runtime_error { "not implemented ATandTInstructionSet::dec(const Register& operand)" };
}

std::string ATandTInstructionSet::dec(const Register& operandBase, int operandOffset) const {
    throw std::runtime_error { "not implemented ATandTInstructionSet::dec(const Register& operandBase, int operandOffset)" };
}

std::string ATandTInstructionSet::neg(const Register& operand) const {
    throw std::runtime_error { "not implemented ATandTInstructionSet::neg(const Register& operand)" };
}

} /* namespace codegen */
