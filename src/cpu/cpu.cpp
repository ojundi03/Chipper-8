#include <iostream>
#include "cpu.hpp"
#include <chrono>

namespace chipper8
{
    CPU::CPU()
    {
        running = true;

        // kept out of hpp and discarding after load as it's only ever used once to load into memory
        static constexpr std::array<uint8_t, 80> FONT{
            0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
            0x20, 0x60, 0x20, 0x20, 0x70, // 1
            0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
            0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
            0x90, 0x90, 0xF0, 0x10, 0x10, // 4
            0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
            0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
            0xF0, 0x10, 0x20, 0x40, 0x40, // 7
            0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
            0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
            0xF0, 0x90, 0xF0, 0x90, 0x90, // A
            0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
            0xF0, 0x80, 0x80, 0x80, 0xF0, // C
            0xE0, 0x90, 0x90, 0x90, 0xE0, // D
            0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
            0xF0, 0x80, 0xF0, 0x80, 0x80  // F
        };

        // Load font into memory
        std::copy(FONT.begin(), FONT.end(), memory.begin());
    }

    void CPU::cycle()
    {
        auto start_time = cycle_clock::now();
        auto last_time_dst = start_time;
        auto last_time_cpu = start_time;
        std::chrono::microseconds cpu_cycle_rate = std::chrono::microseconds(1000000 / cpu_clock_rate); // Convert clock rate to microseconds

        while (running)
        {
            auto now = cycle_clock::now();
            if (now - last_time_dst >= dst_clock_rate)
            {
                dst_count++;
                last_time_dst = cycle_clock::now();
            }
            if (now - last_time_cpu >= cpu_cycle_rate)
            {
                cycle_count++;
                last_time_cpu = cycle_clock::now();
            }
            // std::cout << cycle_count / 60 << "cycle, " << dst_count / 60 << "display and sound." <<std::endl;
            std::cout << dst_count / cycle_count << std::endl;
        }
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
