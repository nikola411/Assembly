#include "emulator.hpp"

#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "ERROR: Wrong number of arguments.\n";
        return -1;
    }

    auto emulator = std::make_shared<Emulator>(argv[1]);
    emulator->ReadInputProgram();
    emulator->StartEmulating();

    return 0;
}