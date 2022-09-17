
#include <string>
#include <iostream>

#include "emulator.hpp"



int main(int argc, char* argv[])
{
    if (argc == 0)
    {

    }

    Emulator emu("output.hex");
    
    emu.read_input_and_load();

    emu.start_emulating();
 
    return 0;
}