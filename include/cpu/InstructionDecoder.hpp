#pragma once
#include <cstdint>
#include "Instruction.hpp"

class InstructionDecoder {
public:
    Instruction decode(uint16_t opcode, RegisterFile* regs, ALU* alu, StatusRegister& sr, ProgramCounter* pc);
};
