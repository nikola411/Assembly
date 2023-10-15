#ifndef _CODES_HPP
#define _CODES_HPP

#include "util.hpp"
#include "instruction.hpp"

#include <map>

class CodesMap
{
public:
    static void PopulateMap();
    static std::vector<std::pair<AssemblyUtil::instruction_ptr, std::vector<ProcessorUtil::codePopulation_ptr>>> GetInstructionCodes(ParserUtil::eInstructionIdentifier identifier, ProcessorUtil::eOperandType operand, ProcessorUtil::eAddressingType addressing);
private:
    static void AddInstructionPair(std::vector<std::pair<AssemblyUtil::instruction_ptr, std::vector<ProcessorUtil::codePopulation_ptr>>>& instructions, int code, std::vector<std::pair<ProcessorUtil::eCodePopulationMethod, ProcessorUtil::eValueToUse>> methods);
    static std::vector<std::pair<AssemblyUtil::instruction_ptr, std::vector<ProcessorUtil::codePopulation_ptr>>>& GetMapEntry(ParserUtil::eInstructionIdentifier identifier, ProcessorUtil::eOperandType operand, ProcessorUtil::eAddressingType addressing);
    static void AddMapEntry(ParserUtil::eInstructionIdentifier identifier, ProcessorUtil::eOperandType operand, ProcessorUtil::eAddressingType addressing, std::vector<std::pair<AssemblyUtil::instruction_ptr, std::vector<ProcessorUtil::codePopulation_ptr>>>& entry);
    static std::map<ParserUtil::eInstructionIdentifier, std::map<ProcessorUtil::eOperandType, std::map<ProcessorUtil::eAddressingType, std::vector<std::pair<AssemblyUtil::instruction_ptr, std::vector<ProcessorUtil::codePopulation_ptr>>>>>> InstructionCodesMap;
};

#endif