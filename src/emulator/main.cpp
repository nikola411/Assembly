#include "Emulator.hpp"

#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "ERROR: Wrong number of arguments.\n";
        return -1;
    }

    auto emulator = std::make_shared<Emulator>(argv[1], true);
    EmulatorResult emulatorResult = emulator->ReadInputProgram();
    emulatorResult = emulator->StartEmulating();

    if (emulatorResult == EMU_FINISH)
    {
        std::cout << "SUCCESS! \n";
        emulator->Print();
    }
    else
    {
        std::cerr << "Emulator failed with result: " << emulatorResult << " \n";   
    }

    return 0;
}