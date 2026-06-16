#pragma once

#include <array>
#include <cstdint>
#include <stack>
#include <chrono>

using cycle_clock = std::chrono::steady_clock;

namespace chipper8
{
    enum Register : uint8_t
    {
        V0,
        V1,
        V2,
        V3,
        V4,
        V5,
        V6,
        V7,
        V8,
        V9,
        VA,
        VB,
        VC,
        VD,
        VE,
        VF
    };

    class CPU
    {
    public:
        CPU();
        void cycle();
        uint16_t fetch();
        void decode(uint16_t instruction);

    private:
        // Chip-8 Components
        std::array<uint8_t, 4096> memory{};
        std::array<uint8_t, 16> registers{};
        std::uint16_t PC{0x200};
        std::uint16_t I{};
        std::stack<uint16_t> stack{};
        std::uint8_t DT{};
        std::uint8_t ST{};

        // Interpreter Variables
        // - Additional helpers
        bool running{};
        double dst_count{};
        double cycle_count{};
        // - Cycle Counting
        cycle_clock::time_point start;
        cycle_clock::time_point end;
        double total_ms{};
        // -- The Chip-8 doesn't have a fixed rate, but we make the clock rate adjustable as different games need
        // -- different times. Delay and sound run at a separate 60hz, which we compare separately.
        int cpu_clock_rate{500};
        const std::chrono::microseconds dst_clock_rate = std::chrono::microseconds(16666); // 1 Million divided by 60.
    };
}
