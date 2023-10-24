#include "assembly.hpp"
#include "instruction.hpp"
#include "codes.hpp"
#include "util.hpp"

#include <iostream>
#include <iomanip>
#include <memory>
#include <bitset>

#define FIRST_PASS false
#define SECOND_PASS true
#define INSTRUCTION_SIZE 4
#define LOCAL true
#define GLOBAL false
#define DEFAULT_SECTION "default"
#define PAYLOAD_MASK 0x0FFF

#define CallInstructionMethod \
    (instruction.*function->method)
#define CallAssemblyMethod(pass, line) \
    (this->*m_handles[pass][line->type][line->instruction])(line)

#define NO_ACTION &Assembly::DoNothing

using namespace AssemblyUtil;
using namespace ParserUtil;

Assembly::Assembly():
    m_end(FIRST_PASS), m_locationCounter(0)
{
    ProcessorUtil::CodesMap::PopulateMap();
    InitializeHandleMap();

    m_currentSection = std::make_shared<SectionEntry>();
    m_currentSection->name = DEFAULT_SECTION;
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
    for (auto op: operands)
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

    for (auto line: m_program)
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
       
        auto manipulationData = ProcessorUtil::CodesMap::GetInstructionCodes(identifier, operandType, addressingType);

        for (auto entry: manipulationData)
        {
            auto instruction = entry.first;
            for (auto function: entry.second)
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
}

void Assembly::PrintProgram()
{
    std::cout << "symbol_table" << "\n";
    for (auto entry: m_symbolTable)
    {
        std::cout << entry->label << " " << entry->section << " " << std::hex << entry->offset << " " << entry->local <<    "\n";
    }
    std::cout << "\n" << "sections" << "\n";
    for (auto section: m_sections)
    {
        std::cout << section->name << " " << section->locationCounter << "\n";
        auto i = 0;
        for (auto byte: section->sectionData)
        {
            std::cout << std::hex << std::setw(2) << std::setfill('0') <<(short)byte << " ";
            if (++i == 4)
            {
                i = 0;
                std::cout << "\n";
            }
        }
    }
    std::cout << "\nrelocations \n";
    for (auto relocation: m_relcationTable)
    {
        std::cout << relocation->label << " " << relocation->section << " "
                    << relocation->type << " " << relocation->offset << " \n";
    }
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

uint16_t Assembly::GetDataValue(std::vector<ParserUtil::ParserOperand>& operands, ProcessorUtil::eValueToUse operandNumber)
{
    auto hasOffset = operandNumber > 2;
    auto index = hasOffset ? operandNumber - 3           : operandNumber; // -3 to normalize (we are looking for offset, not operand but still need operand index)
    auto type =  hasOffset ? operands[index].offsetType  : operands[index].type;
    auto value = hasOffset ? operands[(int)index].offset : operands[(int)index].value;
    auto addressingType = operands[index].addressingType;

    if (type == eOperandType::SYM)
        return GetSymbolValue(value, addressingType);

    if (type == eOperandType::LTR)
        return GetLiteralValue(value);    

    auto operand = operands[(int)index];
    return GetRegisterValue(operand.value, operand.type);
}

uint16_t Assembly::GetSymbolValue(std::string& name, eAddressingType addressingType)
{
    auto entry = FindSymbol(m_symbolTable, name);
    std::vector<BYTE> dataToConvert = { 0x00, 0x00 };

    if (entry == nullptr)
    {
        CreateRelocationEntry(name, m_currentSection->name, m_locationCounter + 3);
        return 0x000;
    }
    
    if (addressingType == eAddressingType::ADDR_DIRECT)
        return (uint16_t)entry->offset;

    dataToConvert = ReadDataFromSection(m_sections, entry->section, entry->offset, 2);
    uint16_t data = (dataToConvert[0] & 0x0F) << 8 | (dataToConvert[1]);

    return data;
}

uint16_t Assembly::GetRegisterValue(std::string& name, eOperandType type)
{
    if (type == eOperandType::CSR)
        return (uint16_t)CSRStringToEnum(name);
    else
        return (uint16_t)GPRStringToEnum(name);
    
    return 0;
}

uint16_t Assembly::GetLiteralValue(std::string& value)
{
    std::size_t pos{};
    int base(16);

    uint16_t data = std::stoi(value, &pos, base);
    data &= PAYLOAD_MASK;
    
    return data;
}

ParserUtil::eOperandType Assembly::GetOperandType(std::vector<ParserUtil::ParserOperand>& operands, AssemblyUtil::line_ptr line)
{
    auto variantOperandNumber = GetVariantOperandNumber(line);       
    auto operandType =  variantOperandNumber != 0 ? operands[variantOperandNumber - 1].type :
                        operands.size()      == 0 ? eOperandType::NONE_TYPE : 
                        eOperandType::GPR;

    return operandType;
}

bool Assembly::InsertSection(AssemblyUtil::section_ptr section)
{
    bool isInserted = false;
    for (auto elem: m_sections)
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

bool Assembly::CreateRelocationEntry(std::string name, std::string section, int offset)
{
    auto entry = std::make_shared<AssemblyUtil::RelocationTableEntry>();
    entry->label = name;
    entry->section = section;
    entry->type = eRelocationType::REL_GLOBAL_OFFSET;
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
    
    m_symbolTable.push_back(std::make_shared<SymbolTableEntry>(sectionName, sectionName)); 
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
    for (auto operand: m_currentLine->operands)
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
        entry = std::make_shared<SymbolTableEntry>(name, m_currentSection->name, m_locationCounter, LOCAL);
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
    m_currentSection = nullptr; // m_sections has a reference to the current section so this just resets current section

    auto sectionName = line->operands.front().value;

    for (auto section: m_sections)
        if (section->name == sectionName)
            m_currentSection = section;

    if (m_currentSection == nullptr)
        throw AssemblyException("Section " + sectionName + " is not found in sections vector in second pass.");
}

void Assembly::EndSecondPass(AssemblyUtil::line_ptr line)
{
    m_currentSection->locationCounter = m_locationCounter;
}

void Assembly::SkipSecondPass(AssemblyUtil::line_ptr line)
{
    auto value = line->operands.front().value;
    auto bytes = GetLiteralValue(value);
    for (auto i = 0; i < bytes; ++i)
        m_currentSection->sectionData.push_back(0x00);
    m_locationCounter += bytes;
}

void Assembly::WordSecondPass(AssemblyUtil::line_ptr line)
{
    auto memoryData = 0x0000;
    for (auto operand: line->operands)
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

void Assembly::OtherTypeFirstPass(AssemblyUtil::line_ptr line)
{
    auto addressingType = eAddressingType::ADDR_DIRECT;
    auto operands = line->operands;
    auto operandType =  GetOperandType(operands, line);

    auto instructionCount = ProcessorUtil::CodesMap::GetInstructionCount(line->instruction, operandType, addressingType);

    m_locationCounter += instructionCount * INSTRUCTION_SIZE; // updating location counter based on number of processor instructions that one asm instruction takes
    line->addressingType = addressingType;
}

void Assembly::MemoryTypeFirstPass(AssemblyUtil::line_ptr line)
{
    if (line->instruction == eInstructionIdentifier::LD)
        line->addressingType = line->operands.front().addressingType;
    if (line->instruction == eInstructionIdentifier::ST)
        line->addressingType = line->operands.back().addressingType;
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
        { eInstructionIdentifier::SKIP,    NO_ACTION },
        { eInstructionIdentifier::EXTERN,  &Assembly::ExternFirstPass },
        { eInstructionIdentifier::WORD,    NO_ACTION }
    };

    m_handles[SECOND_PASS][eInstructionType::DIRECTIVE] =
    {
        { eInstructionIdentifier::SECTION, &Assembly::SectionSecondPass },
        { eInstructionIdentifier::END,     &Assembly::EndSecondPass },
        { eInstructionIdentifier::SKIP,    &Assembly::SkipSecondPass },
        { eInstructionIdentifier::EXTERN,  NO_ACTION },
        { eInstructionIdentifier::WORD,    &Assembly::WordSecondPass }
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

    m_handles[FIRST_PASS][eInstructionType::PROCESSOR] =
    {
        { eInstructionIdentifier::HALT, &Assembly::OtherTypeFirstPass },
        { eInstructionIdentifier::INT,  &Assembly::OtherTypeFirstPass },
        { eInstructionIdentifier::IRET, &Assembly::OtherTypeFirstPass },
        { eInstructionIdentifier::RET,  &Assembly::OtherTypeFirstPass },
    };

    m_handles[FIRST_PASS][eInstructionType::STACK] =
    {
        { eInstructionIdentifier::PUSH, &Assembly::OtherTypeFirstPass },
        { eInstructionIdentifier::POP,  &Assembly::OtherTypeFirstPass },
    };

    m_handles[FIRST_PASS][eInstructionType::SPECIAL] =
    {
        { eInstructionIdentifier::CSRRD, &Assembly::OtherTypeFirstPass },
        { eInstructionIdentifier::CSRWR, &Assembly::OtherTypeFirstPass },
    };

    m_handles[FIRST_PASS][eInstructionType::BRANCH] =
    {
        { eInstructionIdentifier::JMP,  &Assembly::OtherTypeFirstPass },
        { eInstructionIdentifier::CALL, &Assembly::OtherTypeFirstPass },
        { eInstructionIdentifier::BEQ,  &Assembly::OtherTypeFirstPass },
        { eInstructionIdentifier::BNE,  &Assembly::OtherTypeFirstPass },
        { eInstructionIdentifier::BGT,  &Assembly::OtherTypeFirstPass },
    };
}
