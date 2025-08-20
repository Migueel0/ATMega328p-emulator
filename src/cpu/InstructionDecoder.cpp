#include "InstructionDecoder.hpp"
#include "Alu.hpp"
#include "ProgramCounter.hpp"
#include "StatusRegister.hpp"
#include "RegistersFile.hpp"
#include <stdexcept>
#include "cpu.hpp"

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
    std::function<Instruction(uint16_t,CPU)> decoder;
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
    {0XFF00, 0x9700, SBIW},
    {0xF000, 0xC000, RJMP},
    {0xFFFF, 0x9609, IJMP},
    {0xF000, 0xE000, LDI},
    {0xEE00, 0x8000, LD},
    {0xFC00, 0x2C00, MOV}


}};

Instruction InstructionDecoder::decode(uint16_t opcode,CPU cpu) {
    for (const auto& entry : instructionTable) {
        if ((opcode & entry.mask) == entry.pattern) {
            return entry.decoder(opcode,cpu);
        }
    }
    throw std::runtime_error("Opcode not supported");
}

//INSTRUCTION SET
//--------------------------------------------Arithmetic and Logic Instructions--------------------------------------------

Instruction ADD(uint16_t opcode, CPU cpu) {

    RegisterFile* regs  = &cpu.getRegisterFile();
    ALU* alu  = &cpu.getAlu();
    StatusRegister& sr = cpu.getStatusRegister();
    ProgramCounter* pc = &cpu.getProgramCounter();

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

Instruction ADC(uint16_t opcode,CPU cpu) {

    RegisterFile* regs  = &cpu.getRegisterFile();
    ALU* alu  = &cpu.getAlu();
    StatusRegister& sr = cpu.getStatusRegister();
    ProgramCounter* pc = &cpu.getProgramCounter();

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


Instruction SUB(uint16_t opcode,CPU cpu) {

    RegisterFile* regs  = &cpu.getRegisterFile();
    ALU* alu  = &cpu.getAlu();
    StatusRegister& sr = cpu.getStatusRegister();
    ProgramCounter* pc = &cpu.getProgramCounter();

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

Instruction SBC(uint16_t opcode,CPU cpu) {

    RegisterFile* regs  = &cpu.getRegisterFile();
    ALU* alu  = &cpu.getAlu();
    StatusRegister& sr = cpu.getStatusRegister();
    ProgramCounter* pc = &cpu.getProgramCounter();

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

Instruction SUBI(uint16_t opcode,CPU cpu) {

    RegisterFile* regs  = &cpu.getRegisterFile();
    ALU* alu  = &cpu.getAlu();
    StatusRegister& sr = cpu.getStatusRegister();
    ProgramCounter* pc = &cpu.getProgramCounter();

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

Instruction SBCI(uint16_t opcode,CPU cpu) {

    RegisterFile* regs  = &cpu.getRegisterFile();
    ALU* alu  = &cpu.getAlu();
    StatusRegister& sr = cpu.getStatusRegister();
    ProgramCounter* pc = &cpu.getProgramCounter();

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


Instruction AND(uint16_t opcode,CPU cpu) {

    RegisterFile* regs  = &cpu.getRegisterFile();
    ALU* alu  = &cpu.getAlu();
    StatusRegister& sr = cpu.getStatusRegister();
    ProgramCounter* pc = &cpu.getProgramCounter();

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


Instruction OR(uint16_t opcode,CPU cpu) {

    RegisterFile* regs  = &cpu.getRegisterFile();
    ALU* alu  = &cpu.getAlu();
    StatusRegister& sr = cpu.getStatusRegister();
    ProgramCounter* pc = &cpu.getProgramCounter();

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

Instruction ANDI(uint16_t opcode,CPU cpu) {

    RegisterFile* regs  = &cpu.getRegisterFile();
    ALU* alu  = &cpu.getAlu();
    StatusRegister& sr = cpu.getStatusRegister();
    ProgramCounter* pc = &cpu.getProgramCounter();

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

Instruction ORI(uint16_t opcode,CPU cpu) {
    RegisterFile* regs  = &cpu.getRegisterFile();
    ALU* alu  = &cpu.getAlu();
    StatusRegister& sr = cpu.getStatusRegister();
    ProgramCounter* pc = &cpu.getProgramCounter();

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

Instruction EOR(uint16_t opcode,CPU cpu) {

    RegisterFile* regs  = &cpu.getRegisterFile();
    ALU* alu  = &cpu.getAlu();
    StatusRegister& sr = cpu.getStatusRegister();
    ProgramCounter* pc = &cpu.getProgramCounter();

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

Instruction ADIW(uint16_t opcode,CPU cpu) {

    RegisterFile* regs  = &cpu.getRegisterFile();
    ALU* alu  = &cpu.getAlu();
    StatusRegister& sr = cpu.getStatusRegister();
    ProgramCounter* pc = &cpu.getProgramCounter();

    Instruction inst;
    uint8_t rd = inst.operands[0] = ((opcode >> 4) & 0x03);
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


Instruction SBIW(uint16_t opcode,CPU cpu) {

    RegisterFile* regs  = &cpu.getRegisterFile();
    ALU* alu  = &cpu.getAlu();
    StatusRegister& sr = cpu.getStatusRegister();
    ProgramCounter* pc = &cpu.getProgramCounter();

    Instruction inst;
    uint8_t rd = inst.operands[0] = ((opcode >> 4) & 0x03);
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

//--------------------------------------------Branch Instructions--------------------------------------------

Instruction RJMP(uint16_t opcode, CPU cpu){
    Instruction inst;
    ProgramCounter* pc = &cpu.getProgramCounter();
    uint16_t K = inst.operands[0] = opcode & 0x0FFF;
    if (K & 0x0800) {
        K |= 0xF000;  
    }

    inst.execute = [pc,K](){
        uint16_t currentPc = pc->get();
        pc->set(currentPc + K + 1);
    };

    return inst;
}

Instruction IJMP(uint16_t opcode, CPU cpu){
    ProgramCounter* pc = &cpu.getProgramCounter();
    RegisterFile* regs = &cpu.getRegisterFile();

    uint16_t Z = (regs->read(31) << 8) | regs->read(30);

    Instruction inst;
    inst.execute = [pc,Z](){
        pc->set(Z);
    };

    return inst;
}

//--------------------------------------------Data Transfer Instructions--------------------------------------------

Instruction MOV(uint16_t opcode, CPU cpu){

    ProgramCounter* pc = &cpu.getProgramCounter();
    RegisterFile* regs = &cpu.getRegisterFile();

    Instruction inst;
    uint8_t rd = inst.operands[0] = (opcode >> 4) & 0x1F;
    uint8_t rr = inst.operands[1] = (opcode & 0x0F) | ((opcode >> 5) & 0x10);  


    inst.execute = [rd,rr,pc,regs](){
        uint8_t val = regs->read(rr);
        regs->write(rd,val);
        pc->increment();
    };

    return inst;
}

Instruction LDI(uint16_t opcode, CPU cpu){
    ProgramCounter* pc = &cpu.getProgramCounter();
    RegisterFile* regs = &cpu.getRegisterFile();
    Instruction inst;

    uint8_t K = inst.operands[0] = ((opcode >> 4 )& 0xF0) | (opcode & 0x0F);
    uint8_t rd = inst.operands[1] = 16 + (opcode & 0x00F0 >> 4);

    inst.execute = [K,rd,pc,regs](){
        regs->write(rd,K);
        pc->increment();
    };
}

Instruction LD(uint16_t opcode, CPU cpu){
    ProgramCounter* pc = &cpu.getProgramCounter();
    RegisterFile* regs = &cpu.getRegisterFile();
    Instruction inst;

    uint8_t rd;
    uint8_t N; //N represents the value of X,Y or Z 16-bit pointers

    if(opcode & 0xFE00 == 0x9000){
        switch (opcode & 0x000F){

            case 0x000C: // LD Rd,X
                LD_N(26,27,rd,inst,opcode,regs,pc,N);
                break;
            case 0x000D: // LD Rd,X+
                LD_NPostInc(26,27,rd,inst,opcode,regs,pc,N);
                break;
            case 0x000E: // LD Rd,-X
                LD_NPreDec(26,27,rd,inst,opcode,regs,pc,N);
                break;
            case 0x0009: // LD Rd,Y+
                LD_NPostInc(28,29,rd,inst,opcode,regs,pc,N);
                break;
            case 0x000A: // LD Rd,-Y
                LD_NPreDec(28,29,rd,inst,opcode,regs,pc,N);
                break;
            case 0x0001: // LD Rd,Z+
                LD_NPostInc(30,31,rd,inst,opcode,regs,pc,N);
                break;
            case 0x0002: // LD Rd,-Z
                LD_NPreDec(30,31,rd,inst,opcode,regs,pc,N);
                break;
        }
    }else if(opcode & 0xFE00 == 0x8000){
        switch (opcode & 0x000F){
            case 0x0008: // LD Rd,Y
                LD_N(28,29,rd,inst,opcode,regs,pc,N);
                break;
            case 0x0000: // LD Rd,Z
                LD_N(30,31,rd,inst,opcode,regs,pc,N);
                break;
        }
    }
}


void LD_N(int lowByte, int highByte,uint16_t rd, Instruction inst,uint16_t opcode,RegisterFile* regs,ProgramCounter* pc,uint16_t N){
    rd = inst.operands[0] = (opcode >> 4) & 0x001F;
    uint16_t X = (regs->read(highByte) << 8) | regs->read(lowByte);
    N = inst.operands[1] =  X;

    inst.execute = [regs,rd,N,pc](){
        regs->write(rd,N);
        pc->increment();
    };
}

void LD_NPostInc(int lowByte, int highByte, uint16_t rd, Instruction inst,uint16_t opcode,RegisterFile* regs,ProgramCounter* pc,uint16_t N){
    rd = inst.operands[0] = (opcode >> 4) & 0x001F;
    uint16_t pointer = (regs->read(highByte) << 8) | regs->read(lowByte);
    N = inst.operands[1] =  pointer;

    inst.execute = [regs,rd,N,pc](){
        //1st cycle
        regs->write(rd,N);

        //2nd cycle
        uint16_t postInc = N + 1;
        regs->write(26,postInc & 0xFF);
        regs->write(27,(postInc >> 8) & 0xFF);

        pc->increment();
    };

}

void LD_NPreDec(int lowByte, int highByte, uint16_t rd, Instruction inst,uint16_t opcode,RegisterFile* regs,ProgramCounter* pc,uint16_t N){
    rd = inst.operands[0] = (opcode >> 4) & 0x001F;
    uint16_t pointer = (regs->read(26) << 8) | regs->read(27);
    N = inst.operands[1] =  pointer;

    inst.execute = [regs,rd,N,pc](){
        
        //1st cycle
        uint16_t preDec = N - 1;
        regs->write(26,preDec & 0xFF);
        regs->write(27,(preDec >> 8) & 0xFF);
        uint16_t data = (regs->read(26) << 8) | regs->read(27);

        //2nd cycle
        regs->write(rd,N);

        pc->increment();
    };

}

