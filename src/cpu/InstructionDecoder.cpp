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



std::array<InstructionPattern, 131> instructionTable = {{
    {0xFC00, 0x0C00, ADD},
    {0xFC00, 0x1C00, ADC},
    {0xFC00, 0x1800, SUB},
    {0xFC00, 0x0800, SBC},
    {0xF000, 0x5000, SUBI},
    {0xF000, 0x4000, SBCI},
    {0xFC00, 0x2000, AND},
    {0xFC00, 0x2200, OR},
    {0xF000, 0x7000, ANDI},
    {0xF000, 0x6000, ORI},
    {0xFC00, 0x2400, EOR},
    {0xFF00, 0x9600, ADIW},

}};

Instruction InstructionDecoder::decode(uint16_t opcode, RegisterFile* regs, ALU* alu, StatusRegister& sr, ProgramCounter* pc) {
    for (const auto& entry : instructionTable) {
        if ((opcode & entry.mask) == entry.pattern) {
            return entry.decoder(opcode, regs, alu, sr, pc);
        }
    }
    throw std::runtime_error("Opcode not supported");
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

Instruction ADC(uint16_t opcode, RegisterFile* regs, ALU* alu, StatusRegister& sr, ProgramCounter* pc) {
    Instruction inst;
    uint8_t rd = inst.operands[0] = (opcode >> 4) & 0x1F;
    uint8_t rr = inst.operands[1] = ((opcode >> 5) & 0x10) | (opcode & 0x0F); 
    inst.execute = [regs,rd,rr,alu, &sr, pc](){
        uint8_t val1 = regs->read(rd);
        uint8_t val2 = regs->read(rr);
        bool carry = sr.getFlag(FLAG_C);
        uint8_t result = alu->add(val1, val2, carry, sr);
        regs->write(rd, result);
        pc->increment();
    };  
    return inst;
}


Instruction SUB(uint16_t opcode, RegisterFile* regs, ALU* alu, StatusRegister& sr, ProgramCounter* pc) {
    Instruction inst;
    uint8_t rd = inst.operands[0] = (opcode >> 4) & 0x1F;
    uint8_t rr = inst.operands[1] = ((opcode >> 5) & 0x10) | (opcode & 0x0F); 
    inst.execute = [regs,rd,rr,alu, &sr, pc](){
        uint8_t val1 = regs->read(rd);
        uint8_t val2 = regs->read(rr);
        uint8_t result = alu->sub(val1, val2, false, sr);
        regs->write(rd, result);
        pc->increment();
    };  
    return inst;
}

Instruction SBC(uint16_t opcode, RegisterFile* regs, ALU* alu, StatusRegister& sr, ProgramCounter* pc) {
    Instruction inst;
    uint8_t rd = inst.operands[0] = (opcode >> 4) & 0x1F;
    uint8_t rr = inst.operands[1] = ((opcode >> 5) & 0x10) | (opcode & 0x0F); 
    inst.execute = [regs,rd,rr,alu, &sr, pc](){
        uint8_t val1 = regs->read(rd);
        uint8_t val2 = regs->read(rr);
        bool carry = sr.getFlag(FLAG_C);
        uint8_t result = alu->sub(val1, val2, carry, sr);
        regs->write(rd, result);
        pc->increment();
    };  
    return inst;
}

Instruction SUBI(uint16_t opcode, RegisterFile* regs, ALU* alu, StatusRegister& sr, ProgramCounter* pc) {
    Instruction inst;
    uint8_t rd = inst.operands[0] = (opcode >> 4) & 0x1F;
    uint8_t K = inst.operands[1] = ((opcode & 0x0F00) >> 4) | (opcode & 0x000F);  
    inst.execute = [regs,rd,K,alu, &sr, pc](){
        uint8_t val1 = regs->read(rd);
        uint8_t result = alu->sub(val1, K, false, sr);
        regs->write(rd, result);
        pc->increment();
    };  
    return inst;
}

Instruction SBCI(uint16_t opcode, RegisterFile* regs, ALU* alu, StatusRegister& sr, ProgramCounter* pc) {
    Instruction inst;
    uint8_t rd = inst.operands[0] = (opcode >> 4) & 0x1F;
    uint8_t K = inst.operands[1] = ((opcode & 0x0F00) >> 4) | (opcode & 0x000F);  
    inst.execute = [regs,rd,K,alu, &sr, pc](){
        uint8_t val1 = regs->read(rd);
        bool carry = sr.getFlag(FLAG_C);
        uint8_t result = alu->sub(val1, K, carry, sr);
        regs->write(rd, result);
        pc->increment();
    };  
    return inst;
}


Instruction AND(uint16_t opcode, RegisterFile* regs, ALU* alu, StatusRegister& sr, ProgramCounter* pc) {
    Instruction inst;
    uint8_t rd = inst.operands[0] = (opcode >> 4) & 0x1F;
    uint8_t rr = inst.operands[1] = ((opcode >> 5) & 0x10) | (opcode & 0x0F); 
    inst.execute = [regs,rd,rr,alu, &sr, pc](){
        uint8_t val1 = regs->read(rd);
        uint8_t val2 = regs->read(rr);
        uint8_t result = alu->and(val1, val2, sr);
        regs->write(rd, result);
        pc->increment();
    };  
    return inst;
}


Instruction OR(uint16_t opcode, RegisterFile* regs, ALU* alu, StatusRegister& sr, ProgramCounter* pc) {
    Instruction inst;
    uint8_t rd = inst.operands[0] = (opcode >> 4) & 0x1F;
    uint8_t rr = inst.operands[1] = ((opcode >> 5) & 0x10) | (opcode & 0x0F); 
    inst.execute = [regs,rd,rr,alu, &sr, pc](){
        uint8_t val1 = regs->read(rd);
        uint8_t val2 = regs->read(rr);
        uint8_t result = alu->or(val1, val2, sr);
        regs->write(rd, result);
        pc->increment();
    };  
    return inst;
}

Instruction ANDI(uint16_t opcode, RegisterFile* regs, ALU* alu, StatusRegister& sr, ProgramCounter* pc) {
    Instruction inst;
    uint8_t rd = inst.operands[0] = (opcode >> 4) & 0x1F;
    uint8_t K = inst.operands[1] = ((opcode & 0x0F00) >> 4) | (opcode & 0x000F);  
    inst.execute = [regs,rd,K,alu, &sr, pc](){
        uint8_t val1 = regs->read(rd);
        uint8_t result = alu->and(val1, K, sr);
        regs->write(rd, result);
        pc->increment();
    };  
    return inst;
}

Instruction ORI(uint16_t opcode, RegisterFile* regs, ALU* alu, StatusRegister& sr, ProgramCounter* pc) {
    Instruction inst;
    uint8_t rd = inst.operands[0] = (opcode >> 4) & 0x1F;
    uint8_t K = inst.operands[1] = ((opcode & 0x0F00) >> 4) | (opcode & 0x000F);  
    inst.execute = [regs,rd,K,alu, &sr, pc](){
        uint8_t val1 = regs->read(rd);
        uint8_t result = alu->or(val1, K, sr);
        regs->write(rd, result);
        pc->increment();
    };  
    return inst;
}

Instruction EOR(uint16_t opcode, RegisterFile* regs, ALU* alu, StatusRegister& sr, ProgramCounter* pc) {
    Instruction inst;
    uint8_t rd = inst.operands[0] = (opcode >> 4) & 0x1F;
    uint8_t rr = inst.operands[1] = ((opcode >> 5) & 0x10) | (opcode & 0x0F); 
    inst.execute = [regs,rd,rr,alu, &sr, pc](){
        uint8_t val1 = regs->read(rd);
        uint8_t val2 = regs->read(rr);
        uint8_t result = alu->xor(val1, val2, sr);
        regs->write(rd, result);
        pc->increment();
    };  
    return inst;
}

Instruction ADIW(uint16_t opcode, RegisterFile* regs, ALU* alu, StatusRegister& sr, ProgramCounter* pc) {
    Instruction inst;


    uint8_t rd = inst.operands[0] + ((opcode >> 4) & 0x03);
    uint8_t K = inst.operands[1] = ((opcode & 0xC0) >> 4) | (opcode & 0x0F);

    inst.execute = [regs,rd,K,alu, &sr, pc](){
        //1st cycle
        uint8_t rdLow = regs->read(rd);
        uint8_t resultLow = alu->add(rdLow, K, false, sr);
        regs->write(rd, resultLow);

        //2nd cycle
        uint8_t rdHigh = regs->read(rd + 1);
        uint8_t resultHigh = alu->add(rdHigh, 0, sr.getFlag(FLAG_C), sr);
        regs->write(rd + 1, resultHigh);

        pc->increment();
    };  
    return inst;
}


Instruction SBIW(uint16_t opcode, RegisterFile* regs, ALU* alu, StatusRegister& sr, ProgramCounter* pc) {
    Instruction inst;
    uint8_t rd = inst.operands[0] + ((opcode >> 4) & 0x03);
    uint8_t K = inst.operands[1] = ((opcode & 0xC0) >> 4) | (opcode & 0x0F);

    inst.execute = [regs,rd,K,alu, &sr, pc](){
        //1st cycle
        uint8_t rdLow = regs->read(rd);
        uint8_t resultLow = alu->sub(rdLow, K, false, sr);
        regs->write(rd, resultLow);

        //2nd cycle
        uint8_t rdHigh = regs->read(rd + 1);
        uint8_t resultHigh = alu->sub(rdHigh, 0, sr.getFlag(FLAG_C), sr);
        regs->write(rd + 1, resultHigh);

        pc->increment();
    };  
    return inst;
}
