#pragma once
#include <array>
#include<cstdint>
#include<stdexcept>
#include<iostream>
#include <vector>

static constexpr size_t WORDS = 16384;
class Flash{

    private:
        std::array<uint16_t,WORDS> mem;
    
    public:
        Flash();
        void load(const std::vector<uint16_t>& program);
        uint16_t read(uint16_t address) const;
        void write(uint16_t addr, uint16_t val);
        size_t size() const;

};