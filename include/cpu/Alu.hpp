#pragma once
#include <cstdint>
#include "StatusRegister.hpp"

class ALU {
public:
    uint8_t add(uint8_t a, uint8_t b, bool carry, StatusRegister& sr);
    uint8_t sub(uint8_t a, uint8_t b, bool carry, StatusRegister& sr);
    uint8_t and(uint8_t a, uint8_t b, StatusRegister& sr);
    uint8_t or(uint8_t a, uint8_t b, StatusRegister& sr);
    uint8_t xor(uint8_t a, uint8_t b, StatusRegister& sr);
};
