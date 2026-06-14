#include <iostream>
#include "cpu.hpp"
#include <chrono>

namespace chipper8
{
    CPU::CPU()
    {
        running = true;
    }

    void CPU::cycle() {
        auto start_time = cycle_clock::now();
        auto last_time_dst = start_time;
        auto last_time_cpu = start_time;
        std::chrono::microseconds cpu_cycle_rate = std::__1::chrono::microseconds(1000000 / cpu_clock_rate); // Convert clock rate to microseconds

        while (running) {
            auto now = cycle_clock::now();
            if (now - last_time_dst >= dst_clock_rate) {
                dst_count++;
                last_time_dst = cycle_clock::now();
            }
            if (now - last_time_cpu >= cpu_cycle_rate) {
                cycle_count++;
                last_time_cpu = cycle_clock::now();
            }
            // std::cout << cycle_count / 60 << "cycle, " << dst_count / 60 << "display and sound." <<std::endl;
            std::cout << dst_count / cycle_count <<std::endl;
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
