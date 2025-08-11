#include "RegistersFile.hpp"
#include <stdexcept>

uint8_t RegisterFile::read(size_t index) const {
    if (index >= NUM_REGS) throw std::out_of_range("Invalid register index");
    return regs[index];
}

void RegisterFile::write(size_t index, uint8_t value) {
    if (index >= NUM_REGS) throw std::out_of_range("Invalid register index");
    regs[index] = value;
}

void RegisterFile::clear() {
    regs.fill(0);
}
