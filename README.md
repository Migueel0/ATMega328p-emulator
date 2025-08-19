# ATMega328P Emulator

A **C++ emulator** for the **ATmega328P microcontroller** (the one used in Arduino Uno).  
The project models the main hardware blocks:

- **CPU**: General-purpose registers, ALU, Program Counter, Status Register, Instruction Decoder  
- **Memory**: Program Memory (Flash) and Data Memory (SRAM + register file + I/O space)  
- **Future work**: peripherals, interrupts, bootloader  

---

## Project structure

```
ATMega328p-emulator
├── cpu/
│   ├── CPU.hpp / CPU.cpp
│   ├── ALU.hpp / ALU.cpp
│   ├── RegisterFile.hpp / RegisterFile.cpp
│   ├── ProgramCounter.hpp / ProgramCounter.cpp
│   ├── StatusRegister.hpp / StatusRegister.cpp
│   ├── InstructionDecoder.hpp / InstructionDecoder.cpp
│   └── Instruction.hpp / Instruction.cpp
├── memory/
│   ├── Flash.hpp / Flash.cpp
│   └── SRAM.hpp / SRAM.cpp
└── CMakeLists.txt           ← Build configuration
```


---

## Design overview

The emulator follows the **Harvard architecture** of the ATmega328P:

- **Program Memory**: 32 KB Flash, organized as 16K × 16-bit words, addressed by a 14-bit Program Counter  
- **Data Memory**: a flat space that includes:
  - 32 General-purpose registers (R0–R31)  
  - 64 standard I/O registers  
  - 160 extended I/O registers  
  - 2 KB internal SRAM  

---

## Roadmap

- **Core functionality**
  - Fetch-decode-execute cycle  
  - Basic instruction set (ADD, SUB, MOV, LDI, etc.)  
  - ProgramMemory + DataMemory integration  

- **Data memory model**
  - Proper mapping of register file and SREG  
  - Stack operations (PUSH/POP)  
  - Indirect addressing with X, Y, Z registers  
  - Load/store to SRAM and I/O  

- **Peripherals & interrupts**
  - Interrupt Unit and vectors  
  - Timers, GPIO, UART emulation  
  - Context saving/restoring  

- **Bootloader & self-programming**
  - Simulate Flash self-programming via SPM instructions  
  - Partition memory into bootloader / application sections  

---

## Contributing

Contributions are welcome! You can:

- Open issues for bugs or feature requests  
- Submit pull requests for new instructions, peripherals, optimizations, or tests  
- Suggest architectural improvements  

---

## References

- [ATmega328P Datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega328P-Data-Sheet-DS40002061B.pdf)
- [AVR Instruction Set Manual](https://ww1.microchip.com/downloads/en/DeviceDoc/AVR-InstructionSet-Manual-DS40002198.pdf)  

---

