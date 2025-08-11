#pragma once
#include <array>
#include <cstdint>
#include <string>

class ProgramCounter {
    private:
        uint16_t pc;
    public:
        void reset();
        uint16_t get();
        void set(uint16_t value);
        void increment();
        void decrement();

        ProgramCounter();
        ProgramCounter(uint16_t initialValue);
        ProgramCounter(const ProgramCounter& other);


};