#include <iostream>
#include "cpu/cpu.hpp"
#include "display/display.hpp"

int main(int, char **)
{
    std::cout << "Hello, from Chipper-8!\n";

    chipper8::CPU cpu;
    cpu.init();

    chipper8::Display display;
    display.init();
}
