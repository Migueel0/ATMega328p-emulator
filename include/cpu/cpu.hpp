#pragma once
#include <array>
#include <cstdint>
#include <string>
#include "ProgramCounter.hpp"
#include "Alu.hpp"
#include "RegistersFile.hpp"
#include "StatusRegister.hpp"
#include "InstructionDecoder.hpp"
#include "Flash.hpp"
#include "SRAM.hpp"

class CPU {

private:
    ALU alu;
    InstructionDecoder instrcutionDecoder;
    RegisterFile regs;
    StatusRegister sr;
    ProgramCounter pc;

    Flash* flash;
    SRAM* sram;
    


public:
    CPU(Flash* flash,SRAM* sram);
    void reset();
    void step(CPU cpu);
    void run(CPU cpu);
    ALU getAlu();
    InstructionDecoder getInstructionDecoder();
    RegisterFile getRegisterFile();
    ProgramCounter getProgramCounter();
    StatusRegister getStatusRegister();
};
