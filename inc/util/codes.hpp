#ifndef _CODES_HPP
#define _CODES_HPP

#include "util.hpp"
#include "instruction.hpp"

#include <map>

using AssemblyUtil::instruction_ptr;
using AssemblyUtil::Instruction;
using ParserUtil::eAddressingType;
using ParserUtil::eInstructionIdentifier;
using ParserUtil::eOperandType;

namespace ProcessorUtil
{
    enum eValueToUse
    {
        FIRST_OPERAND = 0x00, SECOND_OPERAND = 0x01, THIRD_OPERAND = 0x02,
        FIRST_OFFSET = 0x03, SECOND_OFFSET = 0x04, THIRD_OFFSET = 0x05
    };

    typedef void (AssemblyUtil::Instruction::* InstructionMethodPtr)(uint16_t);

    struct CodePopulation
    {
        InstructionMethodPtr method;
        eValueToUse operand;

        CodePopulation(InstructionMethodPtr method, eValueToUse operand):
            method(method), operand(operand) {}
    };

    typedef std::shared_ptr<CodePopulation> codePopulation_ptr;
    typedef std::pair<Instruction, std::vector<codePopulation_ptr>> instructionPopulationPair;
    
    class CodesMap
    {
    public:
        static void PopulateMap();
        static int GetInstructionCount(eInstructionIdentifier identifier, eOperandType operand, eAddressingType addressing);
        static std::vector<instructionPopulationPair> GetInstructionCodes(eInstructionIdentifier identifier, eOperandType operand, eAddressingType addressing);
    private:
        static void AddInstructionPair(int code, std::vector<std::pair<InstructionMethodPtr, eValueToUse>> methods);
        static void AddInstructionList(std::vector<instructionPopulationPair>& instructions, std::vector<std::pair<int, std::vector<std::pair<InstructionMethodPtr, eValueToUse>>>> pairsVector);
        static void SetMapEntry(eInstructionIdentifier identifier, eOperandType operand, eAddressingType addressing);
        static void AddMapEntry();

        static eInstructionIdentifier currentIdentifier;
        static eOperandType currentOperandType;
        static eAddressingType currentAddressingType;

        static std::vector<instructionPopulationPair> currentEntry;

        static std::map<eInstructionIdentifier, std::map<eOperandType, std::map<eAddressingType, std::vector<instructionPopulationPair>>>> InstructionCodesMap;
    };
}

#endif