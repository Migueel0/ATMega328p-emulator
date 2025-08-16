#pragma once
#include<array>
#include<cstdint>
#include<stdexcept>
#include "RegistersFile.hpp"
#include "StatusRegister.hpp"


static constexpr size_t SIZE = 2304; //0x0000 - 0x08FF
class SRAM{
    private:
        std::array<uint8_t,SIZE> mem;
        RegisterFile regs;
        StatusRegister sr;
    public:
        SRAM();
        uint8_t read(uint16_t addr) const;
        void write(uint16_t addr, uint8_t val);
        uint8_t readSRAM(uint16_t addr) const;
        RegisterFile& getRegs();
        StatusRegister& getStatusRegister();
};