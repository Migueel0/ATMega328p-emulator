#include "cpu.hpp"
#include <iostream>

CPU::CPU() {}

ALU CPU::getAlu(){
    return this->alu;
}

InstructionDecoder CPU::getInstructionDecoder(){
    return this->instrcutionDecoder;
}

RegisterFile CPU::getRegisterFile(){
    return this->regs;
}

ProgramCounter CPU::getProgramCounter(){
    return this->pc;
}

StatusRegister CPU::getStatusRegister(){
    return this->sr;
}

void CPU::reset() {
    regs.clear();
    sr.set(0);
    pc.reset();
}

void CPU::step(){
    //TODO: get opcode by program memory
    uint16_t opcode;
    CPU cpu = CPU();
    Instruction instruction = instrcutionDecoder.decode(opcode,cpu);
    instruction.execute();
}

void CPU::run(){
    int size = 0;
    while(pc.get() < size){//TODO: get size by program memory
        step();
    } 
}

int main(){

    return 0;
}
