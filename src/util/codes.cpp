#include "codes.hpp"

using namespace ProcessorUtil;

using AssemblyUtil::instruction_ptr;
using AssemblyUtil::Instruction;

#define DO_NOTHING {}
#define PUSH_AND_RESET_R0\
    AddInstructionPair(entry, 0x81E00000, DO_NOTHING); \
    AddInstructionPair(entry, 0x51000000, DO_NOTHING); \

#define POP_R0\
    AddInstructionPair(entry, 0x930E0004, DO_NOTHING); \

std::map<eInstructionIdentifier, std::map<eOperandType, std::map<eAddressingType, std::vector<instructionPopulationPair>>>>
    CodesMap::InstructionCodesMap = {};

eInstructionIdentifier CodesMap::currentIdentifier = (eInstructionIdentifier)0;
eOperandType CodesMap::currentOperandType = eOperandType::NONE_TYPE;
eAddressingType CodesMap::currentAddressingType = eAddressingType::ADDR_NONE;

void CodesMap::AddInstructionPair(std::vector<instructionPopulationPair>& instructions, int code, std::vector<std::pair<InstructionMethodPtr, eValueToUse>> methods)
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

std::vector<instructionPopulationPair>&
CodesMap::GetMapEntry(ParserUtil::eInstructionIdentifier identifier, eOperandType operand, eAddressingType addressing)
{
    currentIdentifier = identifier;
    currentOperandType = operand;
    currentAddressingType = addressing;

    InstructionCodesMap[identifier];
    InstructionCodesMap[identifier][operand];

    return InstructionCodesMap[identifier][operand][addressing];
}

void CodesMap::AddMapEntry(std::vector<instructionPopulationPair>& entry)
{
    InstructionCodesMap[currentIdentifier][currentOperandType][currentAddressingType] = entry;
}

void CodesMap::PopulateMap()
{
    { // program control methods population
        auto entry = GetMapEntry(eInstructionIdentifier::HALT, eOperandType::NONE_TYPE, eAddressingType::ADDR_NONE);
        AddInstructionPair(entry, 0x00000000, DO_NOTHING);
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::INT, eOperandType::NONE_TYPE, eAddressingType::ADDR_NONE);
        AddInstructionPair(entry, 0x10000000, DO_NOTHING);
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::IRET, eOperandType::NONE_TYPE, eAddressingType::ADDR_NONE);
        AddInstructionPair(entry, 0x93FE0004, DO_NOTHING);
        AddInstructionPair(entry, 0x970E0004, DO_NOTHING); // pop status
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::RET, eOperandType::NONE_TYPE, eAddressingType::ADDR_NONE);
        AddInstructionPair(entry, 0x93FE0004, DO_NOTHING);
        AddMapEntry(entry);
    }

    { // LD map population
        auto entry = GetMapEntry(eInstructionIdentifier::LD, eOperandType::GPR, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(entry, 0x91000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::FIRST_OPERAND },
            { &Instruction::SetInstructionRegisterB, eValueToUse::SECOND_OPERAND }
        });
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::LD, eOperandType::GPR, eAddressingType::ADDR_MEMORY);
        AddInstructionPair(entry, 0x93000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterB, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::LD, eOperandType::GPR, eAddressingType::ADDR_MEMORY_OFFSET);
        AddInstructionPair(entry, 0x91000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::FIRST_OPERAND },
            { &Instruction::SetInstructionRegisterB, eValueToUse::FIRST_OPERAND },
            { &Instruction::SetInstructionPayload, eValueToUse::FIRST_OFFSET }
        });
        AddInstructionPair(entry, 0x93000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterB, eValueToUse::FIRST_OPERAND },
        });
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::LD, eOperandType::SYM, eAddressingType::ADDR_MEMORY);
        PUSH_AND_RESET_R0
        AddInstructionPair(entry, 0x91000000, {{ &Instruction::SetInstructionPayload, eValueToUse::FIRST_OPERAND }});
        AddInstructionPair(entry, 0x93000000, {{ &Instruction::SetInstructionRegisterA, eValueToUse::SECOND_OPERAND }});
        POP_R0
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::LD, eOperandType::LTR, eAddressingType::ADDR_MEMORY);
        PUSH_AND_RESET_R0
        AddInstructionPair(entry, 0x91000000, {{ &Instruction::SetInstructionPayload, eValueToUse::FIRST_OPERAND }});
        AddInstructionPair(entry, 0x93000000, {{ &Instruction::SetInstructionRegisterA, eValueToUse::SECOND_OPERAND }});
        POP_R0
        AddMapEntry(entry);
    }

    { // ST Map population
        auto entry = GetMapEntry(eInstructionIdentifier::ST, eOperandType::GPR, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(entry, 0x91000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::FIRST_OPERAND },
            { &Instruction::SetInstructionRegisterB, eValueToUse::SECOND_OPERAND }
        });
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::ST, eOperandType::GPR, eAddressingType::ADDR_MEMORY);
        AddInstructionPair(entry, 0x81000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterB, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::ST, eOperandType::GPR, eAddressingType::ADDR_MEMORY_OFFSET);
        AddInstructionPair(entry, 0x81000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterC, eValueToUse::FIRST_OPERAND },
            { &Instruction::SetInstructionPayload, eValueToUse::SECOND_OFFSET }
        });
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::ST, eOperandType::SYM, eAddressingType::ADDR_MEMORY);
        PUSH_AND_RESET_R0
        AddInstructionPair(entry, 0x91000000, {{ &Instruction::SetInstructionPayload, eValueToUse::SECOND_OPERAND }});
        AddInstructionPair(entry, 0x81000000, {{ &Instruction::SetInstructionRegisterB, eValueToUse::FIRST_OPERAND }});
        POP_R0
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::ST, eOperandType::LTR, eAddressingType::ADDR_MEMORY);
        PUSH_AND_RESET_R0
        AddInstructionPair(entry, 0x91000000, {{ &Instruction::SetInstructionPayload, eValueToUse::SECOND_OPERAND }});
        AddInstructionPair(entry, 0x81000000, {{ &Instruction::SetInstructionRegisterB, eValueToUse::FIRST_OPERAND }});
        POP_R0
        AddMapEntry(entry);
    }

    { // PUSH and POP map population
        auto entry = GetMapEntry(eInstructionIdentifier::PUSH, eOperandType::GPR, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(entry, 0x81E00FFC, {{ &Instruction::SetInstructionRegisterC, eValueToUse::FIRST_OPERAND }});
        AddMapEntry(entry);
        
        entry = GetMapEntry(eInstructionIdentifier::POP, eOperandType::GPR, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(entry, 0x930E0004, {{ &Instruction::SetInstructionRegisterA, eValueToUse::FIRST_OPERAND }});
        AddMapEntry(entry);
    }

    { // Branch methods map population
        auto entry = GetMapEntry(eInstructionIdentifier::CALL, eOperandType::SYM, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(entry, 0x81E0F004, DO_NOTHING); // push pc
        PUSH_AND_RESET_R0
        AddInstructionPair(entry, 0x30000000, {{ &Instruction::SetInstructionPayload, eValueToUse::FIRST_OPERAND }});
        POP_R0
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::CALL, eOperandType::LTR, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(entry, 0x81E0F004, DO_NOTHING); // push pc
        PUSH_AND_RESET_R0
        AddInstructionPair(entry, 0x30000000, {{ &Instruction::SetInstructionPayload, eValueToUse::FIRST_OPERAND }});
        POP_R0
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::JMP, eOperandType::SYM, eAddressingType::ADDR_DIRECT);
        PUSH_AND_RESET_R0
        AddInstructionPair(entry, 0x30000000, {{ &Instruction::SetInstructionPayload, eValueToUse::FIRST_OPERAND }});
        POP_R0
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::JMP, eOperandType::LTR, eAddressingType::ADDR_DIRECT);
        PUSH_AND_RESET_R0
        AddInstructionPair(entry, 0x30000000, {{ &Instruction::SetInstructionPayload, eValueToUse::FIRST_OPERAND }});
        POP_R0
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::BEQ, eOperandType::SYM, eAddressingType::ADDR_DIRECT);
        PUSH_AND_RESET_R0
        AddInstructionPair(entry, 0x31000000,
        {
            { &Instruction::SetInstructionRegisterB, eValueToUse::FIRST_OPERAND },
            { &Instruction::SetInstructionRegisterC, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionPayload, eValueToUse::THIRD_OPERAND }
        });
        POP_R0
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::BEQ, eOperandType::LTR, eAddressingType::ADDR_DIRECT);
        PUSH_AND_RESET_R0
        AddInstructionPair(entry, 0x31000000,
        {
            { &Instruction::SetInstructionRegisterB, eValueToUse::FIRST_OPERAND },
            { &Instruction::SetInstructionRegisterC, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionPayload, eValueToUse::THIRD_OPERAND }
        });
        POP_R0
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::BNE, eOperandType::SYM, eAddressingType::ADDR_DIRECT);
        PUSH_AND_RESET_R0
        AddInstructionPair(entry, 0x32000000,
        {
            { &Instruction::SetInstructionRegisterB, eValueToUse::FIRST_OPERAND },
            { &Instruction::SetInstructionRegisterC, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionPayload, eValueToUse::THIRD_OPERAND }
        });
        POP_R0
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::BNE, eOperandType::LTR, eAddressingType::ADDR_DIRECT);
        PUSH_AND_RESET_R0
        AddInstructionPair(entry, 0x32000000,
        {
            { &Instruction::SetInstructionRegisterB, eValueToUse::FIRST_OPERAND },
            { &Instruction::SetInstructionRegisterC, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionPayload, eValueToUse::THIRD_OPERAND }
        });
        POP_R0
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::BGT, eOperandType::SYM, eAddressingType::ADDR_DIRECT);
        PUSH_AND_RESET_R0
        AddInstructionPair(entry, 0x33000000,
        {
            { &Instruction::SetInstructionRegisterB, eValueToUse::FIRST_OPERAND },
            { &Instruction::SetInstructionRegisterC, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionPayload, eValueToUse::THIRD_OPERAND }
        });
        POP_R0
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::BGT, eOperandType::LTR, eAddressingType::ADDR_DIRECT);
        PUSH_AND_RESET_R0
        AddInstructionPair(entry, 0x33000000,
        {
            { &Instruction::SetInstructionRegisterB, eValueToUse::FIRST_OPERAND },
            { &Instruction::SetInstructionRegisterC, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionPayload, eValueToUse::THIRD_OPERAND }
        });
        POP_R0
        AddMapEntry(entry);
    }

    { // XCHG and arithmetic methods map population
        auto entry = GetMapEntry(eInstructionIdentifier::XCHG, eOperandType::GPR, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(entry, 0x40000000,
        {
            { &Instruction::SetInstructionRegisterB, eValueToUse::FIRST_OPERAND },
            { &Instruction::SetInstructionRegisterC, eValueToUse::SECOND_OPERAND }
        });
        AddMapEntry(entry);
        
        entry = GetMapEntry(eInstructionIdentifier::ADD, eOperandType::GPR, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(entry, 0x50000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterB, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterC, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::SUB, eOperandType::GPR, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(entry, 0x51000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterB, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterC, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::MUL, eOperandType::GPR, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(entry, 0x52000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterB, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterC, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::DIV, eOperandType::GPR, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(entry, 0x53000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterB, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterC, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry(entry);
    }

    { // Logical methods map population
        auto entry = GetMapEntry(eInstructionIdentifier::NOT, eOperandType::GPR, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(entry, 0x60000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::FIRST_OPERAND },
            { &Instruction::SetInstructionRegisterB, eValueToUse::FIRST_OPERAND },
        });
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::AND, eOperandType::GPR, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(entry, 0x61000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterB, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterC, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::OR, eOperandType::GPR, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(entry, 0x62000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterB, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterC, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::XOR, eOperandType::GPR, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(entry, 0x63000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterB, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterC, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry(entry);
    }

    { // SHL SHR methods map population
        auto entry = GetMapEntry(eInstructionIdentifier::SHL, eOperandType::GPR, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(entry, 0x70000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterB, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterC, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::SHR, eOperandType::GPR, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(entry, 0x71000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterB, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterC, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry(entry);
    }

    { // CSRRD and CSRWR
        auto entry = GetMapEntry(eInstructionIdentifier::CSRRD, eOperandType::GPR, eAddressingType::ADDR_DIRECT); // gpr <= csr
        AddInstructionPair(entry, 0x90000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterB, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry(entry);

        entry = GetMapEntry(eInstructionIdentifier::CSRWR, eOperandType::GPR, eAddressingType::ADDR_DIRECT); // csr <= gpr
        AddInstructionPair(entry, 0x95000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterB, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry(entry);
    }
}

std::vector<std::pair<instruction_ptr, std::vector<codePopulation_ptr>>>
CodesMap::GetInstructionCodes(eInstructionIdentifier identifier, eOperandType operand, eAddressingType addressing)
{
    return InstructionCodesMap[identifier][operand][addressing];
}
