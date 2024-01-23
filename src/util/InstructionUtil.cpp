#include "InstructionUtil.hpp"

using namespace InstructionUtil;

using AssemblyUtil::instruction_ptr;
using AssemblyUtil::Instruction;

#define DO_NOTHING {}

std::map<eInstructionIdentifier, std::map<eOperandType, std::map<eAddressingType, std::vector<instructionPopulationPair>>>>
    CodesMap::InstructionCodesMap = {};

eInstructionIdentifier CodesMap::currentIdentifier = (eInstructionIdentifier)0;
eOperandType CodesMap::currentOperandType = eOperandType::NONE_TYPE;
eAddressingType CodesMap::currentAddressingType = eAddressingType::ADDR_NONE;
std::vector<instructionPopulationPair> CodesMap::currentEntry = {};

void CodesMap::AddInstructionPair(int code, std::vector<std::pair<InstructionMethodPtr, eValueToUse>> methods)
{
    auto codePopulationVector = std::vector<codePopulation_ptr>();

    if (methods.size() == 0)
        codePopulationVector.push_back(nullptr);

    for (auto method: methods)
    {
        codePopulationVector.push_back(std::make_shared<CodePopulation>(method.first, method.second));
    }

    Instruction instruction((uint32_t) code);
    currentEntry.push_back(std::pair<Instruction, std::vector<codePopulation_ptr>>(instruction, codePopulationVector));
}

void CodesMap::SetMapEntry(ParserUtil::eInstructionIdentifier identifier, eOperandType operand, eAddressingType addressing)
{
    currentIdentifier = identifier;
    currentOperandType = operand;
    currentAddressingType = addressing;

    InstructionCodesMap[identifier];
    InstructionCodesMap[identifier][operand];

    currentEntry = InstructionCodesMap[identifier][operand][addressing];
}

void CodesMap::AddMapEntry()
{
    InstructionCodesMap[currentIdentifier][currentOperandType][currentAddressingType] = currentEntry;
}

static constexpr uint8_t StatusRegister = 0x00;
static constexpr uint8_t HandlerRegister = 0x01;
static constexpr uint8_t CauseRegister = 0x02;
static constexpr uint8_t SPRegister = 0x0E;
static constexpr uint8_t PCRegister = 0x0F;

#define SetRegisterA(instr, reg) \
    instr | (reg << 5 * 4 & 0x0F)
#define SetRegisterB(instr, reg) \
    instr | (reg << 4 * 4 & 0x0F)
#define SetRegisterC(instr, reg) \
    instr | (reg << 3 * 4 & 0x0F)

void CodesMap::PopulateMap()
{
    { // program control methods population
        SetMapEntry(eInstructionIdentifier::HALT, eOperandType::NONE_TYPE, eAddressingType::ADDR_NONE);
        AddInstructionPair(0x00000000, DO_NOTHING);
        AddMapEntry();

        SetMapEntry(eInstructionIdentifier::INT, eOperandType::NONE_TYPE, eAddressingType::ADDR_NONE);
        AddInstructionPair(0x10000000, DO_NOTHING);
        AddMapEntry();

        SetMapEntry(eInstructionIdentifier::IRET, eOperandType::NONE_TYPE, eAddressingType::ADDR_NONE);
        //AddInstructionPair(0x93FE0004, DO_NOTHING);
        AddInstructionPair(0x93FE0004, DO_NOTHING);
        AddInstructionPair(0x970E0004, DO_NOTHING); // pop status
        AddMapEntry();

        // fix for dumb project spec
        // SetMapEntry(eInstructionIdentifier::IRET, eOperandType::NONE_TYPE, eAddressingType::ADDR_NONE);
        // AddInstructionPair(0x970E0004, DO_NOTHING); // pop status
        // AddInstructionPair(0x93FE0004, DO_NOTHING);
        // AddMapEntry();

        SetMapEntry(eInstructionIdentifier::RET, eOperandType::NONE_TYPE, eAddressingType::ADDR_NONE);
        AddInstructionPair(0x93FE0004, DO_NOTHING);
        AddMapEntry();
    }

    { // LD map population
        /*
            ld operand, %gpr ; gpr <= operand
        */
        SetMapEntry(eInstructionIdentifier::LD, eOperandType::GPR, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(0x91000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterB, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry();

        SetMapEntry(eInstructionIdentifier::LD, eOperandType::GPR, eAddressingType::ADDR_MEMORY);
        AddInstructionPair(0x92000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterB, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry();

        SetMapEntry(eInstructionIdentifier::LD, eOperandType::GPR, eAddressingType::ADDR_MEMORY_OFFSET);
        AddInstructionPair(0x92000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterB, eValueToUse::FIRST_OPERAND },
            { &Instruction::SetInstructionPayload, eValueToUse::FIRST_OFFSET }
        });
        AddMapEntry();

        SetMapEntry(eInstructionIdentifier::LD, eOperandType::SYM, eAddressingType::ADDR_MEMORY);
        AddInstructionPair(0x92000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionPayload, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry();

        SetMapEntry(eInstructionIdentifier::LD, eOperandType::LTR, eAddressingType::ADDR_MEMORY);
        AddInstructionPair(0x92000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionPayload, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry();

        SetMapEntry(eInstructionIdentifier::LD, eOperandType::SYM, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(0x91000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionPayload, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry();

        SetMapEntry(eInstructionIdentifier::LD, eOperandType::LTR, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(0x91000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionPayload, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry();
    }

    { // ST Map population
        /*
            st %gpr, operand ; operand <= gpr
        */
        SetMapEntry(eInstructionIdentifier::ST, eOperandType::GPR, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(0x91000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterB, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry();

        SetMapEntry(eInstructionIdentifier::ST, eOperandType::GPR, eAddressingType::ADDR_MEMORY);
        AddInstructionPair(0x80000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterC, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry();

        SetMapEntry(eInstructionIdentifier::ST, eOperandType::GPR, eAddressingType::ADDR_MEMORY_OFFSET);
        AddInstructionPair(0x80000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterC, eValueToUse::FIRST_OPERAND },
            { &Instruction::SetInstructionPayload, eValueToUse::SECOND_OFFSET }
        });
        AddMapEntry();

        SetMapEntry(eInstructionIdentifier::ST, eOperandType::SYM, eAddressingType::ADDR_MEMORY);
        AddInstructionPair(0x80000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::FIRST_OPERAND },
            { &Instruction::SetInstructionPayload, eValueToUse::SECOND_OPERAND },
        });
        AddMapEntry();

        SetMapEntry(eInstructionIdentifier::ST, eOperandType::LTR, eAddressingType::ADDR_MEMORY);
        AddInstructionPair(0x80000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::FIRST_OPERAND },
            { &Instruction::SetInstructionPayload, eValueToUse::SECOND_OPERAND },
        });
        AddMapEntry();
    }

    { // PUSH and POP map population
        SetMapEntry(eInstructionIdentifier::PUSH, eOperandType::GPR, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(0x81E00FFC, {{ &Instruction::SetInstructionRegisterC, eValueToUse::FIRST_OPERAND }});
        AddMapEntry();
        
        SetMapEntry(eInstructionIdentifier::POP, eOperandType::GPR, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(0x930E0004, {{ &Instruction::SetInstructionRegisterA, eValueToUse::FIRST_OPERAND }});
        AddMapEntry();
    }

    { // Branch methods map population
        /*
            push pc; pc <= operand; 
        */
        SetMapEntry(eInstructionIdentifier::CALL, eOperandType::SYM, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(0x20000000,
        {
            { &Instruction::SetInstructionPayload, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry();

        SetMapEntry(eInstructionIdentifier::CALL, eOperandType::LTR, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(0x20000000,
        {
            { &Instruction::SetInstructionPayload, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry();
        /*
            pc <= operand;
        */
        SetMapEntry(eInstructionIdentifier::JMP, eOperandType::SYM, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(0x30000000, {{ &Instruction::SetInstructionPayload, eValueToUse::FIRST_OPERAND }});
        AddMapEntry();

        SetMapEntry(eInstructionIdentifier::JMP, eOperandType::LTR, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(0x30000000, {{ &Instruction::SetInstructionPayload, eValueToUse::FIRST_OPERAND }});
        AddMapEntry();
        /*
            if (gpr1 == gpr2) pc <= operand;
        */
        SetMapEntry(eInstructionIdentifier::BEQ, eOperandType::SYM, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(0x31000000,
        {
            { &Instruction::SetInstructionRegisterB, eValueToUse::FIRST_OPERAND },
            { &Instruction::SetInstructionRegisterC, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionPayload,   eValueToUse::THIRD_OPERAND }
        });
        AddMapEntry();

        SetMapEntry(eInstructionIdentifier::BEQ, eOperandType::LTR, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(0x31000000,
        {
            { &Instruction::SetInstructionRegisterB, eValueToUse::FIRST_OPERAND },
            { &Instruction::SetInstructionRegisterC, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionPayload,   eValueToUse::THIRD_OPERAND }
        });
        AddMapEntry();
        /*
            if (gpr1 != gpr2) pc <= operand; 
        */
        SetMapEntry(eInstructionIdentifier::BNE, eOperandType::SYM, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(0x32000000,
        {
            { &Instruction::SetInstructionRegisterB, eValueToUse::FIRST_OPERAND },
            { &Instruction::SetInstructionRegisterC, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionPayload,   eValueToUse::THIRD_OPERAND }
        });
        AddMapEntry();

        SetMapEntry(eInstructionIdentifier::BNE, eOperandType::LTR, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(0x32000000,
        {
            { &Instruction::SetInstructionRegisterB, eValueToUse::FIRST_OPERAND },
            { &Instruction::SetInstructionRegisterC, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionPayload,   eValueToUse::THIRD_OPERAND }
        });
        AddMapEntry();
        /*
            if (gpr1 signed > gpr2) pc <= operand; 
        */
        SetMapEntry(eInstructionIdentifier::BGT, eOperandType::SYM, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(0x33000000,
        {
            { &Instruction::SetInstructionRegisterB, eValueToUse::FIRST_OPERAND },
            { &Instruction::SetInstructionRegisterC, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionPayload,   eValueToUse::THIRD_OPERAND }
        });
        AddMapEntry();

        SetMapEntry(eInstructionIdentifier::BGT, eOperandType::LTR, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(0x33000000,
        {
            { &Instruction::SetInstructionRegisterB, eValueToUse::FIRST_OPERAND },
            { &Instruction::SetInstructionRegisterC, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionPayload,   eValueToUse::THIRD_OPERAND }
        });
        AddMapEntry();
    }

    { // XCHG and arithmetic methods map population
        SetMapEntry(eInstructionIdentifier::XCHG, eOperandType::GPR, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(0x40000000,
        {
            { &Instruction::SetInstructionRegisterB, eValueToUse::FIRST_OPERAND },
            { &Instruction::SetInstructionRegisterC, eValueToUse::SECOND_OPERAND }
        });
        AddMapEntry();
        
        SetMapEntry(eInstructionIdentifier::ADD, eOperandType::GPR, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(0x50000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterB, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterC, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry();

        SetMapEntry(eInstructionIdentifier::SUB, eOperandType::GPR, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(0x51000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterB, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterC, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry();

        SetMapEntry(eInstructionIdentifier::MUL, eOperandType::GPR, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(0x52000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterB, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterC, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry();

        SetMapEntry(eInstructionIdentifier::DIV, eOperandType::GPR, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(0x53000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterB, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterC, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry();
    }

    { // Logical methods map population
        SetMapEntry(eInstructionIdentifier::NOT, eOperandType::GPR, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(0x60000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::FIRST_OPERAND },
            { &Instruction::SetInstructionRegisterB, eValueToUse::FIRST_OPERAND },
        });
        AddMapEntry();

        SetMapEntry(eInstructionIdentifier::AND, eOperandType::GPR, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(0x61000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterB, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterC, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry();

        SetMapEntry(eInstructionIdentifier::OR, eOperandType::GPR, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(0x62000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterB, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterC, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry();

        SetMapEntry(eInstructionIdentifier::XOR, eOperandType::GPR, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(0x63000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterB, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterC, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry();
    }

    { // SHL SHR methods map population
        SetMapEntry(eInstructionIdentifier::SHL, eOperandType::GPR, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(0x70000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterB, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterC, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry();

        SetMapEntry(eInstructionIdentifier::SHR, eOperandType::GPR, eAddressingType::ADDR_DIRECT);
        AddInstructionPair(0x71000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterB, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterC, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry();
    }

    { // CSRRD and CSRWR
        SetMapEntry(eInstructionIdentifier::CSRRD, eOperandType::GPR, eAddressingType::ADDR_DIRECT); // gpr <= csr
        AddInstructionPair(0x90000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterB, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry();

        SetMapEntry(eInstructionIdentifier::CSRWR, eOperandType::GPR, eAddressingType::ADDR_DIRECT); // csr <= gpr
        AddInstructionPair(0x95000000,
        {
            { &Instruction::SetInstructionRegisterA, eValueToUse::SECOND_OPERAND },
            { &Instruction::SetInstructionRegisterB, eValueToUse::FIRST_OPERAND }
        });
        AddMapEntry();
    }
}

int InstructionUtil::CodesMap::GetInstructionCount(eInstructionIdentifier identifier, eOperandType operand, eAddressingType addressing)
{
    auto instructionVector = InstructionCodesMap[identifier][operand][addressing];
    return instructionVector.size();
}

std::vector<instructionPopulationPair>
CodesMap::GetInstructionCodes(eInstructionIdentifier identifier, eOperandType operand, eAddressingType addressing)
{
    return InstructionCodesMap[identifier][operand][addressing];
}
