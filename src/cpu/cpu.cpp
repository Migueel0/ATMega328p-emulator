#include "cpu.hpp"
#include <iostream>

CPU::CPU(Flash* flash,SRAM* sram){
    this->flash = flash;
    this->sram = sram;
    reset();
}

void CPU::reset() {
    regs.clear();
    sr.set(0);
    pc.reset();
    sram->getMem().fill(0);
}

void CPU::step(CPU cpu){
    //Fetch
    uint16_t opcode = cpu.flash->read(pc.get());
    //Decode
    Instruction instruction = instrcutionDecoder.decode(opcode,cpu);
    //Execute
    instruction.execute();
}

void CPU::run(CPU cpu){
    int size = cpu.flash->size();
    while(pc.get() < size){
        step(cpu);
    } 
}

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


int main(){

    return 0;
}
