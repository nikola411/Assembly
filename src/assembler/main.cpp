#include <iostream>
#include <string>

#include "driver.hpp"
#include "assembly.hpp"
#include "util.hpp"
#include "instruction.hpp"
#include "codes.hpp"

#include <bitset>
#include <memory>
#include <vector>

using std::string;

int main(int argc, char* argv[])
{
    //PARSE ARGUMENTS
    std::cout << "Parsing arguments.\n";
    if (argc > 5 || argc < 4)
    {
        std::cout << "FATAL! Wrong number of arguments! \n";
        exit(-1);
    }

    string flag = string(argv[1]);
    string output_file = string(argv[2]);
    string input_file = string(argv[3]);

    bool debug = false;
    if (argc > 4)
    {
        debug = bool(argv[4]);
    }

    std::cout << "Debug enabled: " << debug << "\n";
    if (flag != "-o")
    {
        std::cout << "FATAL! No output flag specified! \n";
    }
    std::cout << "Starting to read the input file.\n";
    // FINISH PARSING THE ARGUMENTS
    // START ASSEMBLY
    auto assembly = new Assembly();
    Driver driver(debug, assembly);
    driver.parse(input_file);

    assembly->PrintProgram();
    assembly->ContinueParsing();

    CodesMap::PopulateMap();
    auto entry = CodesMap::GetInstructionCodes(ParserUtil::eInstructionIdentifier::LD, ProcessorUtil::eOperandType::IMMEDIATE, ProcessorUtil::eAddressingType::MEMORY);
    entry = CodesMap::GetInstructionCodes(ParserUtil::eInstructionIdentifier::LD, ProcessorUtil::eOperandType::REGISTER, ProcessorUtil::eAddressingType::DIRECT);
    // entry = CodesMap::GetInstructionCodes(ParserUtil::eInstructionIdentifier::ADD, ProcessorUtil::eOperandType::REGISTER, ProcessorUtil::eAddressingType::DIRECT);
    delete assembly;

    return 0;
}