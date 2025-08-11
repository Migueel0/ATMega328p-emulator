#pragma once
#include <array>
#include <cstdint>
#include <string>

class CPU {
public:
    CPU();

    void reset();

private:
    uint16_t programCounter;             
    uint16_t stackPointer;             

};
