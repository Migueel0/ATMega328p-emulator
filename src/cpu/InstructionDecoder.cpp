#include "InstructionDecoder.hpp"
#include "Alu.hpp"
#include "ProgramCounter.hpp"
#include "StatusRegister.hpp"
#include "RegistersFile.hpp"
#include <stdexcept>

constexpr uint8_t FLAG_I = 0x80;
constexpr uint8_t FLAG_T = 0x40;
constexpr uint8_t FLAG_H = 0x20;
constexpr uint8_t FLAG_S = 0x10;
constexpr uint8_t FLAG_V = 0x08;
constexpr uint8_t FLAG_N = 0x04;
constexpr uint8_t FLAG_Z = 0x02;
constexpr uint8_t FLAG_C = 0x01;


struct InstructionPattern {
    uint16_t mask;
    uint16_t pattern;
    std::function<Instruction(uint16_t, RegisterFile*, ALU*, StatusRegister&,ProgramCounter*)> decoder;
};

std::array<InstructionPattern, 100> instructionTable = {
    {0xFC00, 0x0C00, ADD}
};

Instruction InstructionDecoder::decode(uint16_t opcode, RegisterFile* regs, ALU* alu, StatusRegister& sr, ProgramCounter* pc) {
    for (const auto& entry : instructionTable) {
        if ((opcode & entry.mask) == entry.pattern) {
            return entry.decoder(opcode, regs, alu, sr, pc);
        }
    }
    throw std::runtime_error("Opcode no soportado");
}

//INSTRUCTION SET
//--------------------------------------------Arithmetic and Logic Instructions--------------------------------------------

Instruction ADD(uint16_t opcode, RegisterFile* regs, ALU* alu, StatusRegister& sr, ProgramCounter* pc) {
    Instruction inst;
    uint8_t rd = inst.operands[0] = (opcode >> 4) & 0x1F;
    uint8_t rr = inst.operands[1] = ((opcode >> 5) & 0x10) | (opcode & 0x0F); 
    inst.execute = [regs,rd,rr,alu, &sr, pc](){
        uint8_t val1 = regs->read(rd);
        uint8_t val2 = regs->read(rr);
        uint8_t result = alu->add(val1, val2, false, sr);
        regs->write(rd, result);
        pc->increment();
    };  
    return inst;
}
