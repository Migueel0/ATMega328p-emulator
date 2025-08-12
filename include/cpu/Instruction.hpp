#pragma once
#include <cstdint>
#include <string>
#include <functional>

struct Instruction {
    uint16_t opcode;
    std::string mnemonic;
    std::vector<uint8_t> operands;
    std::function<void()> execute;
};
