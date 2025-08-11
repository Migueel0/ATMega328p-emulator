#include "Alu.hpp"

constexpr uint8_t FLAG_I = 0x80;
constexpr uint8_t FLAG_T = 0x40;
constexpr uint8_t FLAG_H = 0x20;
constexpr uint8_t FLAG_S = 0x10;
constexpr uint8_t FLAG_V = 0x08;
constexpr uint8_t FLAG_N = 0x04;
constexpr uint8_t FLAG_Z = 0x02;
constexpr uint8_t FLAG_C = 0x01;

uint8_t ALU::add(uint8_t a, uint8_t b, bool carry, StatusRegister& sr) {

    uint16_t result = a + b + (carry ? 1 : 0);

    bool halfCarry = ((a & 0x0F) + (b & 0x0F) + (carry ? 1 : 0)) > 0x0F;
    bool negative = (result & 0x80) != 0;
    bool overflow = (~(a ^ b) & (a ^ result) & 0x80) != 0;
    bool sign = negative ^ overflow;
    bool zero = (result & 0xFF) == 0;
    bool carryFlag = result > 0xFF;

    sr.setFlag(FLAG_H, halfCarry);
    sr.setFlag(FLAG_N, negative);
    sr.setFlag(FLAG_V, overflow);
    sr.setFlag(FLAG_S, sign);
    sr.setFlag(FLAG_Z, zero);
    sr.setFlag(FLAG_C, carryFlag);

    return static_cast<uint8_t>(result);
}

uint8_t ALU::sub(uint8_t a, uint8_t b, bool carry, StatusRegister& sr) {

    uint16_t result = a - b - (carry ? 1 : 0);

    bool halfCarry = ((a & 0x0F) - (b & 0x0F) - (carry ? 1 : 0)) & 0x10;
    bool negative = (result & 0x80) != 0;
    bool overflow = ((a ^ b) & (a ^ result) & 0x80) != 0;
    bool sign = negative ^ overflow;
    bool zero = (result & 0xFF) == 0;
    bool carryFlag = (a < (b + (carry ? 1 : 0)));

    sr.setFlag(FLAG_H, halfCarry);
    sr.setFlag(FLAG_N, negative);
    sr.setFlag(FLAG_V, overflow);
    sr.setFlag(FLAG_S, sign);
    sr.setFlag(FLAG_Z, zero);
    sr.setFlag(FLAG_C, carryFlag);

    return static_cast<uint8_t>(result);
}

uint8_t ALU::and(uint8_t a, uint8_t b, StatusRegister& sr) {

    uint8_t result = a & b;

    bool negative = (result & 0x80) != 0;
    bool zero = result == 0;
    bool sign = negative ^ false;

    sr.setFlag(FLAG_N, negative);
    sr.setFlag(FLAG_Z, zero);
    sr.setFlag(FLAG_S, sign);
    sr.setFlag(FLAG_V, false); 

    return result;
}

uint8_t ALU::or(uint8_t a, uint8_t b, StatusRegister& sr) {

    uint8_t result = a | b;

    bool negative = (result & 0x80) != 0;
    bool zero = result == 0;
    bool sign = negative ^ false; 

    sr.setFlag(FLAG_N, negative);
    sr.setFlag(FLAG_Z, zero);
    sr.setFlag(FLAG_S, sign);
    sr.setFlag(FLAG_V, false); 

    return result;
}

uint8_t ALU::xor(uint8_t a, uint8_t b, StatusRegister& sr) {

    uint8_t result = a ^ b;

    bool negative = (result & 0x80) != 0;
    bool zero = result == 0;
    bool sign = negative ^ false; 

    sr.setFlag(FLAG_N, negative);
    sr.setFlag(FLAG_Z, zero);
    sr.setFlag(FLAG_S, sign);
    sr.setFlag(FLAG_V, false);

    return result;
}