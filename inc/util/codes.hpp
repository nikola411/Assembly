#ifndef _CODES_HPP
#define _CODES_HPP

#include "util.hpp"
#include "instruction.hpp"

#include <map>

namespace ProcessorUtil
{
    class CodesMap
    {
    public:
        static void PopulateMap();
        static std::vector<std::pair<AssemblyUtil::instruction_ptr, std::vector<codePopulation_ptr>>> GetInstructionCodes(ParserUtil::eInstructionIdentifier identifier, eOperandType operand, eAddressingType addressing);
    private:
        static void AddInstructionPair(std::vector<std::pair<AssemblyUtil::instruction_ptr, std::vector<codePopulation_ptr>>>& instructions, int code, std::vector<std::pair<eCodePopulationMethod, eValueToUse>> methods);
        static std::vector<std::pair<AssemblyUtil::instruction_ptr, std::vector<codePopulation_ptr>>>& GetMapEntry(ParserUtil::eInstructionIdentifier identifier, eOperandType operand, eAddressingType addressing);
        static void AddMapEntry(std::vector<std::pair<AssemblyUtil::instruction_ptr, std::vector<codePopulation_ptr>>>& entry);

        static ParserUtil::eInstructionIdentifier currentIdentifier;
        static eOperandType currentOperandType;
        static eAddressingType currentAddressingType;

        static std::map<ParserUtil::eInstructionIdentifier, std::map<eOperandType, std::map<eAddressingType, std::vector<std::pair<AssemblyUtil::instruction_ptr, std::vector<codePopulation_ptr>>>>>> InstructionCodesMap;
    };
}

#endif