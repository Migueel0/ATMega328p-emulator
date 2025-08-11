#pragma once
#include <array>
#include <cstdint>

class StatusRegister {


private:

    uint8_t flags;

public:

    void setFlag(uint8_t mask, bool val);
    bool getFlag(uint8_t mask);
    uint8_t get();
    void set(uint8_t val);
};
