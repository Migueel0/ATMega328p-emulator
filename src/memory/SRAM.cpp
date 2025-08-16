#include "SRAM.hpp"

SRAM::SRAM(){
    mem.fill(0);
}

uint8_t SRAM::read(uint16_t addr) const{
    if(addr >= SIZE){
        throw std::out_of_range("Invalid address");
    }
    return mem[addr];
}

void SRAM::write(uint16_t addr, uint8_t val){
    if(addr >= SIZE){
        throw std::out_of_range("Invalid address");
    }
    mem[addr] = val;
}

uint8_t SRAM::readSRAM(uint16_t addr) const{
    if(addr < 0x0100 || addr > 0x08FF){
        throw std::out_of_range("Invalid address");
    }
    return mem[addr]; 
}

RegisterFile& SRAM::getRegs(){
    return this->regs;
}

StatusRegister& SRAM::getStatusRegister(){
    return this->sr;
}


