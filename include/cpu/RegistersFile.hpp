#pragma once
#include <array>
#include <cstdint>

class RegisterFile {
public:
    static constexpr size_t NUM_REGS = 32;

    RegisterFile() { clear(); }

    uint8_t read(size_t index) const;
    void write(size_t index, uint8_t value);
    void clear();

private:
    std::array<uint8_t, NUM_REGS> regs;
};
