#include <iostream>
#include "cpu/cpu.hpp"
#include "display/display.hpp"

int main(int, char **)
{
    std::cout << "Hello from Chipper-8 :)" << std::endl;

    chipper8::CPU cpu;
    chipper8::Display display;

    // display.init();

    cpu.cycle();
}
