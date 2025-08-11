#include "ProgramCounter.hpp"
#include <iostream>

void ProgramCounter::reset() {
    pc = 0;
}

uint16_t ProgramCounter::get(){
    return pc;
}

void ProgramCounter::set(uint16_t value) {
    pc = value;
}

void ProgramCounter::increment() {
    pc++;
}

void ProgramCounter::decrement() {
    if (pc > 0) {
        pc--;
    }
}
ProgramCounter::ProgramCounter() : pc(0) {}
ProgramCounter::ProgramCounter(uint16_t initialValue) : pc(initialValue) {}
ProgramCounter::ProgramCounter(const ProgramCounter& other) : pc(other.pc) {}