
#include <string>
#include <iostream>

#include "emulator.hpp"
#include "terminal.hpp"
#include "emulator_error.hpp"

int main(int argc, char* argv[])
{
    if (argc == 0)
    {
        std::cout << "Wrong number of arguments. Input file not specified.";
        return 0;
    }

    std::string input = argv[1];

    try
    {
        Emulator emu(input);
        emu.read_input_and_load();
        emu.start_emulating();
    }
    catch (EmulatorError e)
    {
        std::cout << e.to_string();
    }
    
    return 0;
}