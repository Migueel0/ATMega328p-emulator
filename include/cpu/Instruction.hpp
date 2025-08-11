#pragma once
#include <cstdint>
#include <string>
#include <functional>

struct Instruction {
    uint16_t opcode;
    std::string mnemonic;
    std::function<void()> execute;
};
