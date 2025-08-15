#pragma once
#include <array>
#include <cstdint>
#include <string>
#include "ProgramCounter.hpp"
#include "Alu.hpp"
#include "RegistersFile.hpp"
#include "StatusRegister.hpp"
#include "InstructionDecoder.hpp"

class CPU {

private:
    ALU alu;
    InstructionDecoder instrcutionDecoder;
    RegisterFile regs;
    StatusRegister sr;
    ProgramCounter pc;
    


public:
    CPU();
    void reset();
    void step();
    void run();
    ALU getAlu();
    InstructionDecoder getInstructionDecoder();
    RegisterFile getRegisterFile();
    ProgramCounter getProgramCounter();
    StatusRegister getStatusRegister();
};
