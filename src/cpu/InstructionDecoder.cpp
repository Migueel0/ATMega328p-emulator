#include "InstructionDecoder.hpp"

Instruction InstructionDecoder::decode(uint16_t rawOpcode) {
    Instruction inst;
    inst.opcode = rawOpcode;

    /* 
    Associate a mnemonic and an execution function based on the opcode to the complete instruction set summary.
    */ 
    return inst;
}
