#include "EmulatorUtil.hpp"

#include <string>
#include <iomanip>
#include <fstream>

void EmulatorUtil::Dump(BYTE* memory)
{
            // memory dump
    std::string line("");
    std::ofstream file("memory.dump", std::ios_base::openmode::_S_out);
    file << "First 1024 locations \n";
    file << "   Memory   |    Stack   \n";
            // "00 00 00 00 | 00 00 00 00"
    file << std::hex;
    for (int i = 0; i < 1024; )
    {
        for (int j = 0; j < 4; j++)
        {
            file << std::setw(2) << std::setfill('0') << (unsigned short) memory[ProgramStart + i + j] << " ";
        }
        file << "| ";
        for (int j = 0; j < 4; j++)
        {
            file << std::setw(2) << std::setfill('0') << (unsigned short) memory[StackStart - i - j] << " ";
        }
        file << "\n";
        i += 4;
    }
}