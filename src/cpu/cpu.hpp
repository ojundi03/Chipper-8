#pragma once

#include <array>
#include <cstdint>
#include <stack>

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
        uint16_t fetch();
        void decode(uint16_t instruction);

    private:
        std::array<uint8_t, 4096> memory{};
        std::array<uint8_t, 16> registers{};
        std::uint16_t PC{0x200};
        std::uint16_t I{};
        std::stack<uint16_t> stack{};
    };
}
