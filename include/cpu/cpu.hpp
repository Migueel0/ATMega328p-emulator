#pragma once
#include <array>
#include <cstdint>
#include <string>
#include "ProgramCounter.hpp"

class CPU {

private:
    ProgramCounter programCounter;       
    uint16_t stackPointer;   

public:
    CPU();
    void reset();
};
