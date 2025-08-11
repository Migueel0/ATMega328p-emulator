cmake_minimum_required(VERSION 3.10)
project(ATMega328p-emulator)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(ATMega328p-emulator
    src/cpu.cpp
)