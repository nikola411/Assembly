#include "codes.hpp"

using namespace ProcessorUtil;

using AssemblyUtil::instruction_ptr;
using AssemblyUtil::Instruction;

using ParserUtil::eInstructionIdentifier;

#define DO_NOTHING nullptr

typedef std::vector<std::pair<instruction_ptr, std::vector<codePopulation_ptr>>> InstructionManipulationStructure;

std::map<eInstructionIdentifier, std::map<eOperandType, std::map<eAddressingType, InstructionManipulationStructure>>>
CodesMap::InstructionCodesMap =
{
};

void CodesMap::AddInstructionPair(InstructionManipulationStructure& instructions, int code, std::vector<std::pair<eCodePopulationMethod, eValueToUse>> methods)
{
    auto codePopulationVector = std::vector<codePopulation_ptr>();

    if (methods.size() == 0)
        codePopulationVector.push_back(DO_NOTHING);

    for (auto method: methods)
    {
        codePopulationVector.push_back(std::make_shared<CodePopulation>(method.first, method.second));
    }

    auto instructionPtr = std::make_shared<Instruction>((uint32_t) code);

    instructions.push_back(std::pair<instruction_ptr, std::vector<codePopulation_ptr>>(instructionPtr, codePopulationVector));
}

std::vector<std::pair<AssemblyUtil::instruction_ptr, std::vector<ProcessorUtil::codePopulation_ptr>>>&
CodesMap::GetMapEntry(ParserUtil::eInstructionIdentifier identifier, ProcessorUtil::eOperandType operand, ProcessorUtil::eAddressingType addressing)
{
    InstructionCodesMap[identifier];
    InstructionCodesMap[identifier][operand];

    return InstructionCodesMap[identifier][operand][addressing];
}

 
void CodesMap::AddMapEntry(ParserUtil::eInstructionIdentifier identifier, ProcessorUtil::eOperandType operand, ProcessorUtil::eAddressingType addressing, InstructionManipulationStructure& entry)
{
    InstructionCodesMap[identifier][operand][addressing] = entry;
}

void CodesMap::PopulateMap()
{
    auto& entry = GetMapEntry(eInstructionIdentifier::LD, eOperandType::REGISTER, eAddressingType::DIRECT);
    AddInstructionPair(entry, 0x91000000,
    {
        { eCodePopulationMethod::SET_REG_A, eValueToUse::FIRST_OPERAND },
        { eCodePopulationMethod::SET_REG_B, eValueToUse::SECOND_OPERAND }
    });
    AddMapEntry(eInstructionIdentifier::LD, eOperandType::REGISTER, eAddressingType::DIRECT, entry);

    entry = GetMapEntry(eInstructionIdentifier::LD, eOperandType::REGISTER, eAddressingType::MEMORY);
    AddInstructionPair(entry, 0x93000000,
    {
        { eCodePopulationMethod::SET_REG_A, eValueToUse::SECOND_OPERAND },
        { eCodePopulationMethod::SET_REG_B, eValueToUse::FIRST_OPERAND }
    });
    AddMapEntry(eInstructionIdentifier::LD, eOperandType::REGISTER, eAddressingType::MEMORY, entry);

    entry = GetMapEntry(eInstructionIdentifier::LD, eOperandType::REGISTER, eAddressingType::MEMORY_OFFSET);
    AddInstructionPair(entry, 0x91000000,
    {
        { eCodePopulationMethod::SET_REG_A, eValueToUse::FIRST_OPERAND },
        { eCodePopulationMethod::SET_REG_B, eValueToUse::FIRST_OPERAND },
        { eCodePopulationMethod::SET_PAYLOAD, eValueToUse::FIRST_OFFSET }
    });
    AddInstructionPair(entry, 0x93000000,
    {
        { eCodePopulationMethod::SET_REG_A, eValueToUse::SECOND_OPERAND },
        { eCodePopulationMethod::SET_REG_B, eValueToUse::FIRST_OPERAND },
    });
    AddMapEntry(eInstructionIdentifier::LD, eOperandType::REGISTER, eAddressingType::MEMORY_OFFSET, entry);

    entry = GetMapEntry(eInstructionIdentifier::LD, eOperandType::IMMEDIATE, eAddressingType::MEMORY);
    AddInstructionPair(entry, 0x81E00000, {});
    AddInstructionPair(entry, 0x51000000, {});
    AddInstructionPair(entry, 0x91000000, {{ eCodePopulationMethod::SET_PAYLOAD, eValueToUse::FIRST_OPERAND }});
    AddInstructionPair(entry, 0x93000000, {{ eCodePopulationMethod::SET_REG_A, eValueToUse::SECOND_OPERAND }});
    AddInstructionPair(entry, 0x930E0004, {});
    AddMapEntry(eInstructionIdentifier::LD, eOperandType::IMMEDIATE, eAddressingType::MEMORY, entry);

    entry = GetMapEntry(eInstructionIdentifier::ST, eOperandType::REGISTER, eAddressingType::DIRECT);
    AddInstructionPair(entry, 0x91000000,
    {
        { eCodePopulationMethod::SET_REG_A, eValueToUse::FIRST_OPERAND },
        { eCodePopulationMethod::SET_REG_B, eValueToUse::SECOND_OPERAND }
    });
    AddMapEntry(eInstructionIdentifier::ST, eOperandType::REGISTER, eAddressingType::DIRECT, entry);

    entry = GetMapEntry(eInstructionIdentifier::ST, eOperandType::REGISTER, eAddressingType::MEMORY);
    AddInstructionPair(entry, 0x81000000,
    {
        { eCodePopulationMethod::SET_REG_A, eValueToUse::SECOND_OPERAND },
        { eCodePopulationMethod::SET_REG_B, eValueToUse::FIRST_OPERAND }
    });
    AddMapEntry(eInstructionIdentifier::ST, eOperandType::REGISTER, eAddressingType::MEMORY);

    //todo: dodaj da se cuvaju argumenti od GetMapEntry da AddMapEntry ne bi morao da ih ponavlja :)

    entry = GetMapEntry(eInstructionIdentifier::XCHG, eOperandType::REGISTER, eAddressingType::DIRECT);
    AddInstructionPair(entry, 0x40000000,
    {
        { eCodePopulationMethod::SET_REG_B, eValueToUse::FIRST_OPERAND },
        { eCodePopulationMethod::SET_REG_C, eValueToUse::SECOND_OPERAND }
    });
    AddMapEntry(eInstructionIdentifier::XCHG, eOperandType::REGISTER, eAddressingType::DIRECT, entry);
    
    entry = GetMapEntry(eInstructionIdentifier::ADD, eOperandType::REGISTER, eAddressingType::DIRECT);
    AddInstructionPair(entry, 0x50000000,
    {
        { eCodePopulationMethod::SET_REG_A, eValueToUse::SECOND_OPERAND },
        { eCodePopulationMethod::SET_REG_B, eValueToUse::SECOND_OPERAND },
        { eCodePopulationMethod::SET_REG_C, eValueToUse::FIRST_OPERAND }
    });
    AddMapEntry(eInstructionIdentifier::ADD, eOperandType::REGISTER, eAddressingType::DIRECT, entry);

    entry = GetMapEntry(eInstructionIdentifier::SUB, eOperandType::REGISTER, eAddressingType::DIRECT);
    AddInstructionPair(entry, 0x51000000,
    {
        { eCodePopulationMethod::SET_REG_A, eValueToUse::SECOND_OPERAND },
        { eCodePopulationMethod::SET_REG_B, eValueToUse::SECOND_OPERAND },
        { eCodePopulationMethod::SET_REG_C, eValueToUse::FIRST_OPERAND }
    });
    AddMapEntry(eInstructionIdentifier::SUB, eOperandType::REGISTER, eAddressingType::DIRECT, entry);

    entry = GetMapEntry(eInstructionIdentifier::MUL, eOperandType::REGISTER, eAddressingType::DIRECT);
    AddInstructionPair(entry, 0x52000000,
    {
        { eCodePopulationMethod::SET_REG_A, eValueToUse::SECOND_OPERAND },
        { eCodePopulationMethod::SET_REG_B, eValueToUse::SECOND_OPERAND },
        { eCodePopulationMethod::SET_REG_C, eValueToUse::FIRST_OPERAND }
    });
    AddMapEntry(eInstructionIdentifier::MUL, eOperandType::REGISTER, eAddressingType::DIRECT, entry);

    entry = GetMapEntry(eInstructionIdentifier::DIV, eOperandType::REGISTER, eAddressingType::DIRECT);
    AddInstructionPair(entry, 0x53000000,
    {
        { eCodePopulationMethod::SET_REG_A, eValueToUse::SECOND_OPERAND },
        { eCodePopulationMethod::SET_REG_B, eValueToUse::SECOND_OPERAND },
        { eCodePopulationMethod::SET_REG_C, eValueToUse::FIRST_OPERAND }
    });
    AddMapEntry(eInstructionIdentifier::DIV, eOperandType::REGISTER, eAddressingType::DIRECT, entry);

    entry = GetMapEntry(eInstructionIdentifier::NOT, eOperandType::REGISTER, eAddressingType::DIRECT);
    AddInstructionPair(entry, 0x60000000,
    {
        { eCodePopulationMethod::SET_REG_A, eValueToUse::FIRST_OPERAND },
        { eCodePopulationMethod::SET_REG_B, eValueToUse::FIRST_OPERAND },
    });
    AddMapEntry(eInstructionIdentifier::NOT, eOperandType::REGISTER, eAddressingType::DIRECT, entry);

    entry = GetMapEntry(eInstructionIdentifier::AND, eOperandType::REGISTER, eAddressingType::DIRECT);
    AddInstructionPair(entry, 0x61000000,
    {
        { eCodePopulationMethod::SET_REG_A, eValueToUse::SECOND_OPERAND },
        { eCodePopulationMethod::SET_REG_B, eValueToUse::SECOND_OPERAND },
        { eCodePopulationMethod::SET_REG_C, eValueToUse::FIRST_OPERAND }
    });
    AddMapEntry(eInstructionIdentifier::AND, eOperandType::REGISTER, eAddressingType::DIRECT, entry);

    entry = GetMapEntry(eInstructionIdentifier::OR, eOperandType::REGISTER, eAddressingType::DIRECT);
    AddInstructionPair(entry, 0x62000000,
    {
        { eCodePopulationMethod::SET_REG_A, eValueToUse::SECOND_OPERAND },
        { eCodePopulationMethod::SET_REG_B, eValueToUse::SECOND_OPERAND },
        { eCodePopulationMethod::SET_REG_C, eValueToUse::FIRST_OPERAND }
    });
    AddMapEntry(eInstructionIdentifier::OR, eOperandType::REGISTER, eAddressingType::DIRECT, entry);

    entry = GetMapEntry(eInstructionIdentifier::XOR, eOperandType::REGISTER, eAddressingType::DIRECT);
    AddInstructionPair(entry, 0x63000000,
    {
        { eCodePopulationMethod::SET_REG_A, eValueToUse::SECOND_OPERAND },
        { eCodePopulationMethod::SET_REG_B, eValueToUse::SECOND_OPERAND },
        { eCodePopulationMethod::SET_REG_C, eValueToUse::FIRST_OPERAND }
    });
    AddMapEntry(eInstructionIdentifier::XOR, eOperandType::REGISTER, eAddressingType::DIRECT, entry);

    entry = GetMapEntry(eInstructionIdentifier::SHL, eOperandType::REGISTER, eAddressingType::DIRECT);
    AddInstructionPair(entry, 0x70000000,
    {
        { eCodePopulationMethod::SET_REG_A, eValueToUse::SECOND_OPERAND },
        { eCodePopulationMethod::SET_REG_B, eValueToUse::SECOND_OPERAND },
        { eCodePopulationMethod::SET_REG_C, eValueToUse::FIRST_OPERAND }
    });
    AddMapEntry(eInstructionIdentifier::SHL, eOperandType::REGISTER, eAddressingType::DIRECT, entry);

    entry = GetMapEntry(eInstructionIdentifier::SHR, eOperandType::REGISTER, eAddressingType::DIRECT);
    AddInstructionPair(entry, 0x71000000,
    {
        { eCodePopulationMethod::SET_REG_A, eValueToUse::SECOND_OPERAND },
        { eCodePopulationMethod::SET_REG_B, eValueToUse::SECOND_OPERAND },
        { eCodePopulationMethod::SET_REG_C, eValueToUse::FIRST_OPERAND }
    });
    AddMapEntry(eInstructionIdentifier::SHR, eOperandType::REGISTER, eAddressingType::DIRECT, entry);

}

std::vector<std::pair<instruction_ptr, std::vector<codePopulation_ptr>>>
CodesMap::GetInstructionCodes(eInstructionIdentifier identifier, eOperandType operand, eAddressingType addressing)
{
    return InstructionCodesMap[identifier][operand][addressing];
}
