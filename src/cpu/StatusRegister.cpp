#include "StatusRegister.hpp"

void StatusRegister::setFlag(uint8_t mask, bool val) {
    if (val) {
        flags |= mask; 
    } else {
        flags &= ~mask;
    }
}

bool StatusRegister::getFlag(uint8_t mask) {
    return (flags & mask) != 0;
}

uint8_t StatusRegister::get() {
    return flags;
}

void StatusRegister::set(uint8_t val) {
    flags = val;
}

