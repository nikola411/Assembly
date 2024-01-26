#include "Assembly.hpp"
#include "Instruction.hpp"
#include "InstructionUtil.hpp"
#include "AssemblyUtil.hpp"
#include "Util.hpp"

#include <iostream>
#include <iomanip>
#include <memory>
#include <bitset>
#include <fstream>

#define FIRST_PASS false
#define SECOND_PASS true
#define INSTRUCTION_SIZE 4
#define LOCAL true
#define DEFINED true
#define UNDEFIED false
#define DEFAULT_SECTION "default"
#define PAYLOAD_MASK 0x0FFF

#define CallInstructionMethod \
    (instruction.*function->method)
#define CallAssemblyMethod(pass, line) \
    (this->*m_handles[pass][line->type][line->instruction])(line)

#define NO_ACTION &Assembly::DoNothing

using namespace AssemblyUtil;
using namespace ParserUtil;

Assembly::Assembly() :
    m_end(FIRST_PASS), m_locationCounter(0)
{
    InstructionUtil::CodesMap::PopulateMap();
    InitializeHandleMap();

    m_currentSection = std::make_shared<SectionEntry>();
    m_currentSection->name = DEFAULT_SECTION;

    m_symbolAndLiteralPool = {};
}

void Assembly::SetInstruction(eInstructionIdentifier instruction, eInstructionType type)
{
    m_currentLine = std::make_shared<ProgramLine>();
    m_currentLine->instruction = instruction;
    m_currentLine->type = type;
}

void Assembly::SetOperand(std::string value, eOperandType type)
{
    m_currentLine->operands.push_back(ParserOperand(value, type));
}

void Assembly::SetOperand(ParserOperand& operand)
{
    m_currentLine->operands.push_back(operand);
}

void Assembly::SetMultipleOperands(std::vector<ParserOperand> operands)
{
    for (auto op : operands)
    {
        m_currentLine->operands.push_back(op);
    }
}

void Assembly::FinishInstruction()
{
    if (m_end == SECOND_PASS)
        return;

    CallAssemblyMethod(m_end, m_currentLine);

    m_program.push_back(m_currentLine);
}

void Assembly::ContinueParsing()
{
    m_locationCounter = 0;

    if (m_end == FIRST_PASS)
        throw AssemblyUtil::AssemblyException("Assembly code did not finish with .end directive.");

    for (auto line : m_program)
    {
        if (line->type == eInstructionType::LABEL || line->type == eInstructionType::DIRECTIVE)
        {
            CallAssemblyMethod(m_end, line);
            continue;
        }

        auto operands = line->operands;
        auto identifier = line->instruction;
        auto addressingType = line->addressingType;
        auto operandType = GetOperandType(operands, line);

        auto manipulationData = InstructionUtil::CodesMap::GetInstructionCodes(identifier, operandType, addressingType);

        for (auto entry : manipulationData)
        {
            auto instruction = entry.first;
            for (auto function : entry.second)
            {
                if (function != nullptr)
                {
                    auto data = GetDataValue(operands, function->operand);
                    CallInstructionMethod(data);
                }
            }

            WriteDataToSection(m_currentSection, instruction.GetInstructionVector(), m_locationCounter);
            m_locationCounter += INSTRUCTION_SIZE;
        }
    }

    auto last = m_sections.front();

    for (auto section : m_sections) // update sections offsets
    {
        if (section == m_sections.front())
        {
            section->offset = 0;
            continue;
        }

        section->offset = last->offset + last->sectionData.size();
        last = section;
    }
}

void Assembly::PrintProgram(std::string outFile)
{
    std::fstream file;
    file.open(outFile, std::ios_base::openmode::_S_out);

#define SPACE " "
#define DETAILED false

    file << "SYMBOL TABLE:" << "\n";
    if (DETAILED)
    {
        file << ";Index "
            << "Label "
            << "Section "
            << "Offset "
            << "Local "
            << "Defined "
            << "Value \n";
    }

    int j = 0;
    for (auto entry : m_symbolTable)
    {
        file
            << ++j << ": "
            << entry->label << " "
            << entry->section << " "
            << std::hex << entry->offset << " "
            << entry->local << " "
            << entry->defined << " "
            << entry->poolEntry << "\n";
    }
    // file << "\n" << "sections" << "\n";
    file << "\nSECTIONS:\n";
    for (auto section : m_sections)
    {
        file << "<" << section->name << "> " << section->locationCounter
            << " " << section->offset << " " << section->valueMap.size() << ": \n";

        if (section->sectionData.size() == 0)
            continue;

        auto i = 0;
        auto lines = 0;

        for (int i = 0; i < section->sectionData.size(); i += 4)
        {
            file << std::hex << std::setw(8) << std::setfill('0') << lines << ": ";
            for (int j = i; j < i + 4; ++j)
            {
                file << std::setw(2) << std::setfill('0') << (short)section->sectionData[j] << SPACE;
            }
            lines += 4;
            file << "\n";
        }

    }

    file << "\nRELOCATIONS:\n";
    if (DETAILED)
    {
        file << ";Index | Label | Section | Offset | Type \n";
    }

    int i = 0;
    for (auto relocation : m_relcationTable)
    {
        file << ++i << ": " << relocation->label << SPACE << relocation->section << SPACE
            << relocation->offset << SPACE << relocation->type << " \n";
    }

    file.close();
}

int Assembly::GetVariantOperandNumber(AssemblyUtil::line_ptr line) const
{
    switch (line->instruction)
    {
    case eInstructionIdentifier::LD:
    case eInstructionIdentifier::JMP:
    case eInstructionIdentifier::CALL:
        return 1;
    case eInstructionIdentifier::ST:
        return 2;
    case eInstructionIdentifier::BEQ:
    case eInstructionIdentifier::BNE:
    case eInstructionIdentifier::BGT:
        return 3;
    default:
        return 0;
    }
}

bool Assembly::CanOperandHaveOffset(AssemblyUtil::line_ptr line) const
{
    if (line->instruction == eInstructionIdentifier::LD || line->instruction == eInstructionIdentifier::ST)
        return true;

    return false;
}

uint16_t Assembly::GetDataValue(std::vector<ParserUtil::ParserOperand>& operands, InstructionUtil::eValueToUse operandNumber)
{
    auto hasOffset = operandNumber > 2;
    auto index = hasOffset ? operandNumber - 3 : operandNumber; // -3 to normalize (we are looking for offset, not operand but still need operand index)
    auto type = hasOffset ? operands[index].offsetType : operands[index].type;
    auto value = hasOffset ? operands[(int)index].offset : operands[(int)index].value;
    auto addressingType = operands[index].addressingType;

    if (type == eOperandType::SYM)
        return GetSymbolValue(value, addressingType);

    if (type == eOperandType::LTR)
    {
        auto poolIndex = GetLiteralMapEntry(value);
        return m_currentSection->sectionData.size() + poolIndex - m_locationCounter;
    }

    auto operand = operands[(int)index];
    return GetRegisterValue(operand.value, operand.type);
}

uint16_t Assembly::GetSymbolValue(std::string& name, eAddressingType addressingType)
{
    auto entry = FindSymbol(m_symbolTable, name);

    if (entry == nullptr || !entry->local)
    {
        CreateRelocationEntry(name, m_currentSection->name, m_locationCounter + 2, eRelocationType::REL_EXTERN);
        return 0x000;
    }

    if (entry->section != m_currentSection->name)
    {
        CreateRelocationEntry(name, m_currentSection->name, m_locationCounter + 2, eRelocationType::REL_LOCAL);
        return 0x000;
    }

    uint16_t entryIndex = GetLiteralMapEntry(entry->offset);

    if (entry->poolEntry == 0xFFFFFFFF)
        entry->poolEntry = entryIndex;

    CreateRelocationEntry(name, m_currentSection->name, m_locationCounter + 2, eRelocationType::REL_RELATIVE);

    uint32_t offset = m_currentSection->sectionData.size() + (entryIndex * INSTRUCTION_SIZE) - m_locationCounter;
    return offset;
}

uint16_t Assembly::GetRegisterValue(std::string& name, eOperandType type)
{
    if (type == eOperandType::CSR)
        return (uint16_t)CSRStringToEnum(name);
    else
        return (uint16_t)GPRStringToEnum(name);

    return 0;
}

uint16_t Assembly::GetLiteralMapEntry(std::string& value)
{
    uint32_t literalValue = GetLiteralValue(value);
    return GetLiteralMapEntry(literalValue);
}

uint16_t Assembly::GetLiteralMapEntry(uint32_t value)
{
    auto& valueMap = m_currentSection->valueMap;
    uint16_t entry = 0;

    for (entry = 0; entry < valueMap.size(); ++entry)
    {
        if (valueMap[entry] == value)
            break;
    }

    if (entry == valueMap.size())
        valueMap.push_back(value);

    return entry;
}

uint32_t Assembly::GetLiteralValue(std::string& value)
{
    std::size_t pos{};
    int base(16);
    uint32_t data = std::stoul(value, &pos, base);
    return data;
}

ParserUtil::eOperandType Assembly::GetOperandType(std::vector<ParserUtil::ParserOperand>& operands, AssemblyUtil::line_ptr line)
{
    auto variantOperandNumber = GetVariantOperandNumber(line);
    auto operandType = variantOperandNumber != 0 ? operands[variantOperandNumber - 1].type :
        operands.size() == 0 ? eOperandType::NONE_TYPE :
        eOperandType::GPR;

    return operandType;
}

bool Assembly::InsertSection(AssemblyUtil::section_ptr section)
{
    bool isInserted = false;
    for (auto elem : m_sections)
    {
        if (elem->name == section->name)
        {
            isInserted = true;
            break;
        }
    }

    if (!isInserted)
        m_sections.push_back(section);

    return isInserted;
}

bool Assembly::CreateRelocationEntry(std::string name, std::string section, int offset, ParserUtil::eRelocationType type)
{
    auto entry = std::make_shared<AssemblyUtil::RelocationTableEntry>();
    entry->label = name;
    entry->section = section;
    entry->type = type;
    entry->offset = offset;

    m_relcationTable.push_back(entry);

    return true;
}

void Assembly::SectionFirstPass(AssemblyUtil::line_ptr line)
{
    auto sectionName = m_currentLine->operands.front().value;

    if (m_currentSection->name == sectionName)
        throw AssemblyException("Cannot declare same section two times in a row");

    // update and save current section
    m_currentSection->locationCounter = m_locationCounter;
    auto bytesToWrite = m_currentSection->sectionData.size() - m_locationCounter;
    AllocateSectionData(m_currentSection, bytesToWrite);
    m_sections.push_back(m_currentSection);

    // init section
    m_locationCounter = 0;
    m_currentSection = std::make_shared<SectionEntry>();
    m_currentSection->name = sectionName;

    m_symbolTable.push_back(std::make_shared<SymbolTableEntry>(sectionName, sectionName, 0, LOCAL, DEFINED));
}

void Assembly::EndFirstPass(AssemblyUtil::line_ptr line)
{
    if (m_currentSection->sectionData.size() < m_locationCounter)
        AllocateSectionData(m_currentSection, m_locationCounter - m_currentSection->sectionData.size());

    InsertSection(m_currentSection);

    m_end = SECOND_PASS;
}

void Assembly::ExternFirstPass(AssemblyUtil::line_ptr line)
{
    for (auto operand : m_currentLine->operands)
    {
        auto entry = std::make_shared<SymbolTableEntry>(operand.value, m_currentSection->name, m_locationCounter, false);
        m_symbolTable.push_back(entry);
    }
}

void Assembly::LabelFirstPass(AssemblyUtil::line_ptr line)
{
    auto name = m_currentLine->operands.front().value;
    name = name.substr(0, name.size() - 1);

    auto entry = FindSymbol(m_symbolTable, name);
    if (entry == nullptr)
    {
        entry = std::make_shared<SymbolTableEntry>(name, m_currentSection->name, m_locationCounter, LOCAL, DEFINED);
        m_symbolTable.push_back(entry);
    }
    else
    {
        UpdateSymbolOffset(m_symbolTable, entry, m_locationCounter);
        UpdateSymbolSection(m_symbolTable, entry, m_currentSection->name);
    }
}

void Assembly::SectionSecondPass(AssemblyUtil::line_ptr line)
{
    m_currentSection->locationCounter = m_locationCounter;
    WriteDataTableToSection(m_currentSection);
    m_currentSection = nullptr; // m_sections has a reference to the current section so this just resets current section
    auto sectionName = line->operands.front().value;

    for (auto section : m_sections)
    {
        if (section->name == sectionName)
        {
            m_currentSection = section;
            break;
        }
    }

    if (m_currentSection == nullptr)
        throw AssemblyException("Section " + sectionName + " is not found in sections vector in second pass.");

    m_currentSection->locationCounter = 0;
    m_locationCounter = 0;
}

void Assembly::EndSecondPass(AssemblyUtil::line_ptr line)
{
    m_currentSection->locationCounter = m_locationCounter;
    WriteDataTableToSection(m_currentSection);
}

void Assembly::SkipFirstPass(AssemblyUtil::line_ptr line)
{
    auto value = line->operands.front().value;
    auto bytes = GetLiteralValue(value);
    AllocateSectionData(m_currentSection, bytes);
    // for (auto i = 0; i < bytes; ++i)
    //     m_currentSection->sectionData.push_back(0x00);
    m_locationCounter += bytes;
}

void Assembly::WordFirstPass(AssemblyUtil::line_ptr line)
{
    for (const auto& operand : line->operands)
    {
        WriteDataToSection(m_currentSection, 0x0, m_locationCounter, INSTRUCTION_SIZE);
        m_locationCounter += INSTRUCTION_SIZE;
    }
}

void Assembly::SkipSecondPass(AssemblyUtil::line_ptr line)
{
    auto bytes = GetLiteralValue(line->operands.front().value);
    m_locationCounter += bytes;
}

void Assembly::WordSecondPass(AssemblyUtil::line_ptr line)
{
    auto memoryData = 0x0000;
    for (auto operand : line->operands)
    {
        if (operand.type == eOperandType::LTR)
        {
            auto value = GetLiteralValue(operand.value);
            memoryData |= value;
        }
        else
        // if (operand.type == eOperandType::SYM)
        {
            auto value = GetSymbolValue(operand.value, eAddressingType::ADDR_DIRECT);
            memoryData |= value;
        }

        WriteDataToSection(m_currentSection, memoryData, m_locationCounter, 4);
        memoryData = 0x0000;
        m_locationCounter += 4;
    }
}

void Assembly::LabelSecondPass(AssemblyUtil::line_ptr line)
{
    auto value = line->operands[0].value;
    value = value.substr(0, value.size() - 1);

    auto entry = FindSymbol(m_symbolTable, value);
    if (entry != nullptr)
    {
        entry->offset = m_locationCounter;
        return;
    }

    throw AssemblyException("Label not found in first pass but found in second");
}

void Assembly::GlobalSecondPass(AssemblyUtil::line_ptr line)
{
    for (auto operand : line->operands)
        for (auto symbol : m_symbolTable)
            if (symbol->label == operand.value)
                symbol->local = false;
}

void Assembly::OtherTypeFirstPass(AssemblyUtil::line_ptr line)
{
    auto addressingType = eAddressingType::ADDR_DIRECT;
    if (line->instruction == eInstructionIdentifier::HALT)
        addressingType = eAddressingType::ADDR_NONE;
    auto operands = line->operands;
    auto operandType = GetOperandType(operands, line);

    auto instructionCount = InstructionUtil::CodesMap::GetInstructionCount(line->instruction, operandType, addressingType);

    line->addressingType = addressingType;

    for (int i = 0; i < instructionCount; ++i)
        WriteDataToSection(m_currentSection, 0x0000, m_locationCounter, INSTRUCTION_SIZE);

    m_locationCounter += instructionCount * INSTRUCTION_SIZE; // updating location counter based on number of processor instructions that one asm instruction takes

}

void Assembly::MemoryTypeFirstPass(AssemblyUtil::line_ptr line)
{
    if (line->instruction == eInstructionIdentifier::LD)
        line->addressingType = line->operands.front().addressingType;
    if (line->instruction == eInstructionIdentifier::ST)
    {
        line->addressingType = line->operands.back().addressingType;
        if (line->addressingType == eAddressingType::ADDR_DIRECT)
            throw AssemblyException("Wrong addressing for the ST instruction:");
    }

    WriteDataToSection(m_currentSection, 0x0000, m_locationCounter, INSTRUCTION_SIZE);
    m_locationCounter += INSTRUCTION_SIZE;
}

void Assembly::DoNothing(AssemblyUtil::line_ptr line)
{
    // Empty handler
}

void Assembly::InitializeHandleMap()
{
    m_handles[FIRST_PASS][eInstructionType::DIRECTIVE] =
    {
        { eInstructionIdentifier::SECTION, &Assembly::SectionFirstPass },
        { eInstructionIdentifier::END,     &Assembly::EndFirstPass },
        { eInstructionIdentifier::SKIP,    &Assembly::SkipFirstPass },
        { eInstructionIdentifier::EXTERN,  &Assembly::ExternFirstPass },
        { eInstructionIdentifier::WORD,    &Assembly::WordFirstPass },
        { eInstructionIdentifier::GLOBAL,  NO_ACTION }
    };

    m_handles[SECOND_PASS][eInstructionType::DIRECTIVE] =
    {
        { eInstructionIdentifier::SECTION, &Assembly::SectionSecondPass },
        { eInstructionIdentifier::END,     &Assembly::EndSecondPass },
        { eInstructionIdentifier::SKIP,    &Assembly::SkipSecondPass },
        { eInstructionIdentifier::EXTERN,  NO_ACTION },
        { eInstructionIdentifier::WORD,    &Assembly::WordSecondPass },
        { eInstructionIdentifier::GLOBAL,  &Assembly::GlobalSecondPass }
    };

    m_handles[FIRST_PASS][eInstructionType::LABEL] =
    {
        { eInstructionIdentifier::LBL, &Assembly::LabelFirstPass }
    };

    m_handles[SECOND_PASS][eInstructionType::LABEL] =
    {
        { eInstructionIdentifier::LBL, &Assembly::LabelSecondPass }
    };

    m_handles[FIRST_PASS][eInstructionType::MEMORY] =
    {
        { eInstructionIdentifier::LD, &Assembly::MemoryTypeFirstPass },
        { eInstructionIdentifier::ST, &Assembly::MemoryTypeFirstPass }
    };

    m_handles[SECOND_PASS][eInstructionType::MEMORY] =
    {
        { eInstructionIdentifier::LD, NO_ACTION },
        { eInstructionIdentifier::ST, NO_ACTION }
    };

    m_handles[FIRST_PASS][eInstructionType::DATA] =
    {
        { eInstructionIdentifier::XCHG, &Assembly::OtherTypeFirstPass },
        { eInstructionIdentifier::ADD,  &Assembly::OtherTypeFirstPass },
        { eInstructionIdentifier::SUB,  &Assembly::OtherTypeFirstPass },
        { eInstructionIdentifier::MUL,  &Assembly::OtherTypeFirstPass },
        { eInstructionIdentifier::DIV,  &Assembly::OtherTypeFirstPass },
        { eInstructionIdentifier::NOT,  &Assembly::OtherTypeFirstPass },
        { eInstructionIdentifier::AND,  &Assembly::OtherTypeFirstPass },
        { eInstructionIdentifier::OR,   &Assembly::OtherTypeFirstPass },
        { eInstructionIdentifier::XOR,  &Assembly::OtherTypeFirstPass },
        { eInstructionIdentifier::SHL,  &Assembly::OtherTypeFirstPass },
        { eInstructionIdentifier::SHR,  &Assembly::OtherTypeFirstPass },
    };

    m_handles[SECOND_PASS][eInstructionType::DATA] =
    {
        { eInstructionIdentifier::XCHG, NO_ACTION },
        { eInstructionIdentifier::ADD,  NO_ACTION },
        { eInstructionIdentifier::SUB,  NO_ACTION },
        { eInstructionIdentifier::MUL,  NO_ACTION },
        { eInstructionIdentifier::DIV,  NO_ACTION },
        { eInstructionIdentifier::NOT,  NO_ACTION },
        { eInstructionIdentifier::AND,  NO_ACTION },
        { eInstructionIdentifier::OR,   NO_ACTION },
        { eInstructionIdentifier::XOR,  NO_ACTION },
        { eInstructionIdentifier::SHL,  NO_ACTION },
        { eInstructionIdentifier::SHR,  NO_ACTION },
    };

    m_handles[FIRST_PASS][eInstructionType::PROCESSOR] =
    {
        { eInstructionIdentifier::HALT, &Assembly::OtherTypeFirstPass },
        { eInstructionIdentifier::INT,  &Assembly::OtherTypeFirstPass },
        { eInstructionIdentifier::IRET, &Assembly::OtherTypeFirstPass },
        { eInstructionIdentifier::RET,  &Assembly::OtherTypeFirstPass },
    };

    m_handles[SECOND_PASS][eInstructionType::PROCESSOR] =
    {
        { eInstructionIdentifier::HALT, NO_ACTION },
        { eInstructionIdentifier::INT,  NO_ACTION },
        { eInstructionIdentifier::IRET, NO_ACTION },
        { eInstructionIdentifier::RET,  NO_ACTION },
    };

    m_handles[FIRST_PASS][eInstructionType::STACK] =
    {
        { eInstructionIdentifier::PUSH, &Assembly::OtherTypeFirstPass },
        { eInstructionIdentifier::POP,  &Assembly::OtherTypeFirstPass },
    };

    m_handles[SECOND_PASS][eInstructionType::STACK] =
    {
        { eInstructionIdentifier::PUSH, NO_ACTION },
        { eInstructionIdentifier::POP,  NO_ACTION },
    };

    m_handles[FIRST_PASS][eInstructionType::SPECIAL] =
    {
        { eInstructionIdentifier::CSRRD, &Assembly::OtherTypeFirstPass },
        { eInstructionIdentifier::CSRWR, &Assembly::OtherTypeFirstPass },
    };

    m_handles[SECOND_PASS][eInstructionType::SPECIAL] =
    {
        { eInstructionIdentifier::CSRRD, NO_ACTION },
        { eInstructionIdentifier::CSRWR, NO_ACTION },
    };

    m_handles[FIRST_PASS][eInstructionType::BRANCH] =
    {
        { eInstructionIdentifier::JMP,  &Assembly::OtherTypeFirstPass },
        { eInstructionIdentifier::CALL, &Assembly::OtherTypeFirstPass },
        { eInstructionIdentifier::BEQ,  &Assembly::OtherTypeFirstPass },
        { eInstructionIdentifier::BNE,  &Assembly::OtherTypeFirstPass },
        { eInstructionIdentifier::BGT,  &Assembly::OtherTypeFirstPass },
    };

    m_handles[SECOND_PASS][eInstructionType::BRANCH] =
    {
        { eInstructionIdentifier::JMP,  NO_ACTION },
        { eInstructionIdentifier::CALL, NO_ACTION },
        { eInstructionIdentifier::BEQ,  NO_ACTION },
        { eInstructionIdentifier::BNE,  NO_ACTION },
        { eInstructionIdentifier::BGT,  NO_ACTION },
    };
}
