#include "Flash.hpp"

Flash::Flash(){
    mem.fill(0);
}

void Flash::load(const std::vector<uint16_t>& program){
    if(program.size() > WORDS){
        throw std::runtime_error("Program too large for flash memory");
    }
    for(size_t i =0;i<program.size(); i++){
        mem[i] = program[i];
    }
}

uint16_t Flash::read(uint16_t addr) const{
    if(addr >= WORDS){
        throw std::out_of_range("Invalid address");
    }
    return mem[addr];
}


void Flash::write(uint16_t addr, uint16_t val){
    if(addr >= WORDS){
        throw std::out_of_range("Invalid address");
    }
    mem[addr] = val;

}

size_t Flash::size() const{
    return mem.size();
}