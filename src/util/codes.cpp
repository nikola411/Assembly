#include "codes.hpp"

using namespace ProcessorUtil;

using AssemblyUtil::instruction_ptr;
using AssemblyUtil::Instruction;
using ParserUtil::eInstructionIdentifier;

#define DO_NOTHING {}
#define PUSH_AND_RESET_R0\
    AddInstructionPair(entry, 0x81E00000, DO_NOTHING); \
    AddInstructionPair(entry, 0x51000000, DO_NOTHING); \

#define POP_R0\
    AddInstructionPair(entry, 0x930E0004, DO_NOTHING); \

typedef std::vector<std::pair<instruction_ptr, std::vector<codePopulation_ptr>>> InstructionManipulationStructure;

std::map<eInstructionIdentifier, std::map<eOperandType, std::map<eAddressingType, InstructionManipulationStructure>>>
    CodesMap::InstructionCodesMap = {};
eInstructionIdentifier CodesMap::currentIdentifier = (eInstructionIdentifier)0;
eOperandType CodesMap::currentOperandType = eOperandType::NONE_TYPE;
eAddressingType CodesMap::currentAddressingType = eAddressingType::NONE_ADDR;

void CodesMap::AddInstructionPair(InstructionManipulationStructure& instructions, int code, std::vector<std::pair<eCodePopulationMethod, eValueToUse>> methods)
{
    auto codePopulationVector = std::vector<codePopulation_ptr>();

    if (methods.size() == 0)
        codePopulationVector.push_back(nullptr);

    for (auto method: methods)
    {
        codePopulationVector.push_back(std::make_shared<CodePopulation>(method.first, method.second));
    }

    auto instructionPtr = std::make_shared<Instruction>((uint32_t) code);
    instructions.push_back(std::pair<instruction_ptr, std::vector<codePopulation_ptr>>(instructionPtr, codePopulationVector));
}

std::vector<std::pair<AssemblyUtil::instruction_ptr, std::vector<codePopulation_ptr>>>&
CodesMap::GetMapEntry(ParserUtil::eInstructionIdentifier identifier, eOperandType operand, eAddressingType addressing)
{
    currentIdentifier = identifier;
    currentOperandType = operand;
    currentAddressingType = addressing;

    InstructionCodesMap[identifier];
    InstructionCodesMap[identifier][operand];

    return InstructionCodesMap[identifier][operand][addressing];
}

void CodesMap::AddMapEntry(InstructionManipulationStructure& entry)
{
    InstructionCodesMap[currentIdentifier][currentOperandType][currentAddressingType] = entry;
}

void CodesMap::PopulateMap()
{
    { // program control methods population
        auto& entry = GetMapEntry(eInstructionIdentifier::HALT, eOperandType::NONE_TYPE, eAddressingType::NONE_ADDR);
        AddInstructionPair(entry, 0x00000000, DO_NOTHING);
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::INT, eOperandType::NONE_TYPE, eAddressingType::NONE_ADDR);
        AddInstructionPair(entry, 0x10000000, DO_NOTHING);
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::IRET, eOperandType::NONE_TYPE, eAddressingType::NONE_ADDR);
        AddInstructionPair(entry, 0x93FE0004, DO_NOTHING);
        AddInstructionPair(entry, 0x970E0004, DO_NOTHING); // pop status
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::RET, eOperandType::NONE_TYPE, eAddressingType::NONE_ADDR);
        AddInstructionPair(entry, 0x93FE0004, DO_NOTHING);
        AddMapEntry(entry);
    }

    { // LD map population
        auto& entry = GetMapEntry(eInstructionIdentifier::LD, eOperandType::REGISTER, eAddressingType::DIRECT);
        AddInstructionPair(entry, 0x91000000,
        {
            { eCodePopulationMethod::SET_REG_A, eValueToUse::FIRST_OPERAND },
            { eCodePopulationMethod::SET_REG_B, eValueToUse::SECOND_OPERAND }
        });
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::LD, eOperandType::REGISTER, eAddressingType::MEMORY);
        AddInstructionPair(entry, 0x93000000,
        {
            { eCodePopulationMethod::SET_REG_A, eValueToUse::SECOND_OPERAND },
            { eCodePopulationMethod::SET_REG_B, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry(entry);

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
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::LD, eOperandType::IMMEDIATE, eAddressingType::MEMORY);
        PUSH_AND_RESET_R0
        AddInstructionPair(entry, 0x91000000, {{ eCodePopulationMethod::SET_PAYLOAD, eValueToUse::FIRST_OPERAND }});
        AddInstructionPair(entry, 0x93000000, {{ eCodePopulationMethod::SET_REG_A, eValueToUse::SECOND_OPERAND }});
        POP_R0
        AddMapEntry(entry);
    }

    { // ST Map population
        auto& entry = GetMapEntry(eInstructionIdentifier::ST, eOperandType::REGISTER, eAddressingType::DIRECT);
        AddInstructionPair(entry, 0x91000000,
        {
            { eCodePopulationMethod::SET_REG_A, eValueToUse::FIRST_OPERAND },
            { eCodePopulationMethod::SET_REG_B, eValueToUse::SECOND_OPERAND }
        });
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::ST, eOperandType::REGISTER, eAddressingType::MEMORY);
        AddInstructionPair(entry, 0x81000000,
        {
            { eCodePopulationMethod::SET_REG_A, eValueToUse::SECOND_OPERAND },
            { eCodePopulationMethod::SET_REG_B, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::ST, eOperandType::REGISTER, eAddressingType::MEMORY_OFFSET);
        AddInstructionPair(entry, 0x81000000,
        {
            { eCodePopulationMethod::SET_REG_A, eValueToUse::SECOND_OPERAND },
            { eCodePopulationMethod::SET_REG_C, eValueToUse::FIRST_OPERAND },
            { eCodePopulationMethod::SET_PAYLOAD, eValueToUse::SECOND_OFFSET }
        });
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::ST, eOperandType::IMMEDIATE, eAddressingType::MEMORY);
        PUSH_AND_RESET_R0
        AddInstructionPair(entry, 0x91000000, {{ eCodePopulationMethod::SET_PAYLOAD, eValueToUse::SECOND_OPERAND }});
        AddInstructionPair(entry, 0x81000000, {{ eCodePopulationMethod::SET_REG_B, eValueToUse::FIRST_OPERAND }});
        POP_R0
        AddMapEntry(entry);
    }

    { // PUSH and POP map population
        auto& entry = GetMapEntry(eInstructionIdentifier::PUSH, eOperandType::REGISTER, eAddressingType::DIRECT);
        AddInstructionPair(entry, 0x81E00FFC, {{ eCodePopulationMethod::SET_REG_C, eValueToUse::FIRST_OPERAND }});
        AddMapEntry(entry);
        
        entry = GetMapEntry(eInstructionIdentifier::POP, eOperandType::REGISTER, eAddressingType::DIRECT);
        AddInstructionPair(entry, 0x930E0004, {{ eCodePopulationMethod::SET_REG_A, eValueToUse::FIRST_OPERAND }});
        AddMapEntry(entry);
    }

    { // Branch methods map population
        auto& entry = GetMapEntry(eInstructionIdentifier::CALL, eOperandType::IMMEDIATE, eAddressingType::DIRECT);
        AddInstructionPair(entry, 0x81E0F004, DO_NOTHING); // push pc
        PUSH_AND_RESET_R0
        AddInstructionPair(entry, 0x30000000, {{ eCodePopulationMethod::SET_PAYLOAD, eValueToUse::FIRST_OPERAND }});
        POP_R0
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::JMP, eOperandType::IMMEDIATE, eAddressingType::DIRECT);
        PUSH_AND_RESET_R0
        AddInstructionPair(entry, 0x30000000, {{ eCodePopulationMethod::SET_PAYLOAD, eValueToUse::FIRST_OPERAND }});
        POP_R0
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::BEQ, eOperandType::IMMEDIATE, eAddressingType::DIRECT);
        PUSH_AND_RESET_R0
        AddInstructionPair(entry, 0x31000000,
        {
            { eCodePopulationMethod::SET_REG_B, eValueToUse::FIRST_OPERAND },
            { eCodePopulationMethod::SET_REG_C, eValueToUse::SECOND_OPERAND },
            { eCodePopulationMethod::SET_PAYLOAD, eValueToUse::THIRD_OPERAND }
        });
        POP_R0
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::BNE, eOperandType::IMMEDIATE, eAddressingType::DIRECT);
        PUSH_AND_RESET_R0
        AddInstructionPair(entry, 0x32000000,
        {
            { eCodePopulationMethod::SET_REG_B, eValueToUse::FIRST_OPERAND },
            { eCodePopulationMethod::SET_REG_C, eValueToUse::SECOND_OPERAND },
            { eCodePopulationMethod::SET_PAYLOAD, eValueToUse::THIRD_OPERAND }
        });
        POP_R0
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::BGT, eOperandType::IMMEDIATE, eAddressingType::DIRECT);
        PUSH_AND_RESET_R0
        AddInstructionPair(entry, 0x33000000,
        {
            { eCodePopulationMethod::SET_REG_B, eValueToUse::FIRST_OPERAND },
            { eCodePopulationMethod::SET_REG_C, eValueToUse::SECOND_OPERAND },
            { eCodePopulationMethod::SET_PAYLOAD, eValueToUse::THIRD_OPERAND }
        });
        POP_R0
        AddMapEntry(entry);
    }

    { // XCHG and arithmetic methods map population
        auto& entry = GetMapEntry(eInstructionIdentifier::XCHG, eOperandType::REGISTER, eAddressingType::DIRECT);
        AddInstructionPair(entry, 0x40000000,
        {
            { eCodePopulationMethod::SET_REG_B, eValueToUse::FIRST_OPERAND },
            { eCodePopulationMethod::SET_REG_C, eValueToUse::SECOND_OPERAND }
        });
        AddMapEntry(entry);
        
        entry = GetMapEntry(eInstructionIdentifier::ADD, eOperandType::REGISTER, eAddressingType::DIRECT);
        AddInstructionPair(entry, 0x50000000,
        {
            { eCodePopulationMethod::SET_REG_A, eValueToUse::SECOND_OPERAND },
            { eCodePopulationMethod::SET_REG_B, eValueToUse::SECOND_OPERAND },
            { eCodePopulationMethod::SET_REG_C, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::SUB, eOperandType::REGISTER, eAddressingType::DIRECT);
        AddInstructionPair(entry, 0x51000000,
        {
            { eCodePopulationMethod::SET_REG_A, eValueToUse::SECOND_OPERAND },
            { eCodePopulationMethod::SET_REG_B, eValueToUse::SECOND_OPERAND },
            { eCodePopulationMethod::SET_REG_C, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::MUL, eOperandType::REGISTER, eAddressingType::DIRECT);
        AddInstructionPair(entry, 0x52000000,
        {
            { eCodePopulationMethod::SET_REG_A, eValueToUse::SECOND_OPERAND },
            { eCodePopulationMethod::SET_REG_B, eValueToUse::SECOND_OPERAND },
            { eCodePopulationMethod::SET_REG_C, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::DIV, eOperandType::REGISTER, eAddressingType::DIRECT);
        AddInstructionPair(entry, 0x53000000,
        {
            { eCodePopulationMethod::SET_REG_A, eValueToUse::SECOND_OPERAND },
            { eCodePopulationMethod::SET_REG_B, eValueToUse::SECOND_OPERAND },
            { eCodePopulationMethod::SET_REG_C, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry(entry);
    }

    { // Logical methods map population
        auto& entry = GetMapEntry(eInstructionIdentifier::NOT, eOperandType::REGISTER, eAddressingType::DIRECT);
        AddInstructionPair(entry, 0x60000000,
        {
            { eCodePopulationMethod::SET_REG_A, eValueToUse::FIRST_OPERAND },
            { eCodePopulationMethod::SET_REG_B, eValueToUse::FIRST_OPERAND },
        });
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::AND, eOperandType::REGISTER, eAddressingType::DIRECT);
        AddInstructionPair(entry, 0x61000000,
        {
            { eCodePopulationMethod::SET_REG_A, eValueToUse::SECOND_OPERAND },
            { eCodePopulationMethod::SET_REG_B, eValueToUse::SECOND_OPERAND },
            { eCodePopulationMethod::SET_REG_C, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::OR, eOperandType::REGISTER, eAddressingType::DIRECT);
        AddInstructionPair(entry, 0x62000000,
        {
            { eCodePopulationMethod::SET_REG_A, eValueToUse::SECOND_OPERAND },
            { eCodePopulationMethod::SET_REG_B, eValueToUse::SECOND_OPERAND },
            { eCodePopulationMethod::SET_REG_C, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::XOR, eOperandType::REGISTER, eAddressingType::DIRECT);
        AddInstructionPair(entry, 0x63000000,
        {
            { eCodePopulationMethod::SET_REG_A, eValueToUse::SECOND_OPERAND },
            { eCodePopulationMethod::SET_REG_B, eValueToUse::SECOND_OPERAND },
            { eCodePopulationMethod::SET_REG_C, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry(entry);
    }

    { // SHL SHR methods map population
        auto& entry = GetMapEntry(eInstructionIdentifier::SHL, eOperandType::REGISTER, eAddressingType::DIRECT);
        AddInstructionPair(entry, 0x70000000,
        {
            { eCodePopulationMethod::SET_REG_A, eValueToUse::SECOND_OPERAND },
            { eCodePopulationMethod::SET_REG_B, eValueToUse::SECOND_OPERAND },
            { eCodePopulationMethod::SET_REG_C, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::SHR, eOperandType::REGISTER, eAddressingType::DIRECT);
        AddInstructionPair(entry, 0x71000000,
        {
            { eCodePopulationMethod::SET_REG_A, eValueToUse::SECOND_OPERAND },
            { eCodePopulationMethod::SET_REG_B, eValueToUse::SECOND_OPERAND },
            { eCodePopulationMethod::SET_REG_C, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry(entry);
    }

    { // CSRRD and CSRWR
        auto& entry = GetMapEntry(eInstructionIdentifier::CSRRD, eOperandType::REGISTER, eAddressingType::DIRECT); // gpr <= csr
        AddInstructionPair(entry, 0x90000000,
        {
            { eCodePopulationMethod::SET_REG_A, eValueToUse::SECOND_OPERAND },
            { eCodePopulationMethod::SET_REG_B, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::CSRWR, eOperandType::REGISTER, eAddressingType::DIRECT); // csr <= gpr
        AddInstructionPair(entry, 0x95000000,
        {
            { eCodePopulationMethod::SET_REG_A, eValueToUse::SECOND_OPERAND },
            { eCodePopulationMethod::SET_REG_B, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry(entry);
    }
}

std::vector<std::pair<instruction_ptr, std::vector<codePopulation_ptr>>>
CodesMap::GetInstructionCodes(eInstructionIdentifier identifier, eOperandType operand, eAddressingType addressing)
{
    return InstructionCodesMap[identifier][operand][addressing];
}
