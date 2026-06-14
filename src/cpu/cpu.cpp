#include <iostream>
#include "cpu.hpp"

namespace chipper8
{
    CPU::CPU()
    {
    }

    uint16_t CPU::fetch()
    {
        /*
        instructions are big endian so backwards. So I get the second half and increment PC.
        Then get first half and increment PC.
        */
        uint8_t second_half = memory[PC++];
        uint8_t first_half = memory[PC++];
        /*
        shift second half 8 bits to the right then OR with first half
        this effectively combines them first_half_second_half
        */
        uint16_t instruction = second_half << 8 | first_half;
        return instruction;
    }

    void CPU::decode(uint16_t instructions)
    {
    }
}
