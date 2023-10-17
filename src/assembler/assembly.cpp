#include "assembly.hpp"
#include "instruction.hpp"
#include "codes.hpp"
#include "util.hpp"

#include <iostream>
#include <memory>
#include <bitset>

#define INSTRUCTION_SIZE 4
#define LOCAL true
#define GLOBAL false
#define DEFAULT_SECTION "default"
#define CallInstructionMethod \
    (*instruction.*function->method)

using namespace AssemblyUtil;
using namespace ParserUtil;

Assembly::Assembly():
    m_end(false), m_locationCounter(0)
{
    ProcessorUtil::CodesMap::PopulateMap();

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
    if (m_end)
        return;

    switch (m_currentLine->type)
    {
        case eInstructionType::DIRECTIVE:
        {
            if (m_currentLine->instruction == eInstructionIdentifier::EXTERN)
            {
                for (auto operand: m_currentLine->operands)
                {
                    auto entry = std::make_shared<SymbolTableEntry>(operand.value, m_currentSection->name, m_locationCounter, false);
                    m_symbolTable.push_back(entry);
                }
            }
            else if (m_currentLine->instruction == eInstructionIdentifier::SECTION)
            {
                auto sectionName = m_currentLine->operands.front().value;

                if (m_currentSection->name == sectionName)
                    throw AssemblyException("Cannot declare same section two times in a row");
                
                m_currentSection->locationCounter = m_locationCounter;

                auto bytesToWrite = m_currentSection->sectionData.size() - m_locationCounter;
                for (auto i = 0; i < m_locationCounter; ++i)
                    m_currentSection->sectionData.push_back(0x00);
    
                m_sections.push_back(m_currentSection);

                m_locationCounter = 0;
                m_currentSection = std::make_shared<SectionEntry>();
                m_currentSection->name = sectionName;
                
                m_symbolTable.push_back(std::make_shared<SymbolTableEntry>(sectionName, sectionName)); 
            }
            else if (m_currentLine->instruction == eInstructionIdentifier::END)
            {
                if (m_currentSection->sectionData.size() < m_locationCounter)
                    for (auto i = 0; i < m_locationCounter; ++i)
                        m_currentSection->sectionData.push_back(0x00);

                m_sections.push_back(m_currentSection);
                m_end = true;
            }

            break;
        }
        case eInstructionType::LABEL:
        {
            auto name = m_currentLine->operands.front().value;
            name = name.substr(0, name.size() - 1);

            auto entry = FindSymbol(m_symbolTable, name);
            if (entry == nullptr)
            {
                entry = std::make_shared<SymbolTableEntry>(name, m_currentSection->name, m_locationCounter, LOCAL);
                m_symbolTable.push_back(entry);

                break;
            }
            
            UpdateSymbolOffset(m_symbolTable, entry, m_locationCounter);
            UpdateSymbolSection(m_symbolTable, entry, m_currentSection->name);
            
            break;
        }
        case eInstructionType::MEMORY:
            if (m_currentLine->instruction == eInstructionIdentifier::LD)
                m_currentLine->addressingType = m_currentLine->operands.front().addressingType;
            if (m_currentLine->instruction == eInstructionIdentifier::ST)
                m_currentLine->addressingType = m_currentLine->operands.back().addressingType;
            break;
        case eInstructionType::BRANCH:
        case eInstructionType::PROCESSOR:
        case eInstructionType::DATA:
        case eInstructionType::SPECIAL:
        case eInstructionType::STACK:
            m_currentLine->addressingType = eAddressingType::ADDR_DIRECT;
            break;
    }

    m_locationCounter += INSTRUCTION_SIZE;
    m_program.push_back(m_currentLine);
}

void Assembly::ContinueParsing()
{
    m_locationCounter = 0;

    if (!m_end)
        throw AssemblyUtil::AssemblyException("Assembly code did not finish with .end directive.");

    for (auto line: m_program)
    {
        if (line->type == eInstructionType::LABEL)
            continue;

        if (line->type == eInstructionType::DIRECTIVE)
        {
            HandleDirective(line);
            continue;
        }

        auto operands = line->operands;
        auto identifier = line->instruction;
        auto addressingType = line->addressingType;
        auto variantOperandNumber = GetVariantOperandNumber(line);       
        auto operandType =  variantOperandNumber != 0 ? operands[variantOperandNumber - 1].type :
                            operands.size()      == 0 ? eOperandType::NONE_TYPE : 
                            eOperandType::GPR;
       
        auto manipulationData = ProcessorUtil::CodesMap::GetInstructionCodes(identifier, operandType, addressingType);

        if (operandType == eOperandType::SYM)
        {

        }

        for (auto entry: manipulationData)
        {
            auto instruction = entry.first;
            for (auto function: entry.second)
            {
                auto data = GetDataValue(operands, function->operand);
                CallInstructionMethod(data);
            }

            WriteDataToSection(m_currentSection, instruction->GetInstructionVector(), m_locationCounter);
            m_locationCounter += 4;
        }
    }
}

void Assembly::PrintProgram()
{

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
    auto index = hasOffset ? operandNumber - 3 : operandNumber;
    auto type = hasOffset ? operands[index].offsetType : operands[index].type;
    auto value = hasOffset ? operands[(int)index].offset : operands[(int)index].value;

    if (type == eOperandType::SYM)
        return GetSymbolValue(value);

    if (type == eOperandType::LTR)
        return GetLiteralValue(value);    

    auto operand = operands[(int)index];
    return GetRegisterValue(operand.value, operand.type);
}

uint16_t Assembly::GetSymbolValue(std::string& name)
{
    auto entry = FindSymbol(m_symbolTable, name);
    std::vector<BYTE> dataToConvert = {0x00, 0x00};

    if (entry == nullptr)
    {
        // relocation
    }
    else
    {
        dataToConvert = ReadDataFromSection(m_sections, entry->section, entry->offset, 2);
    }

    uint16_t data = (dataToConvert[0] & 0x0F) << 8 | (dataToConvert[1]);
    return data;
}

uint16_t Assembly::GetRegisterValue(std::string& name, eOperandType type)
{
    if (type == eOperandType::CSR)
        return CSRStringToEnum(name);
    else
        return GPRStringToEnum(name);
    
    return 0;
}

uint16_t Assembly::GetLiteralValue(std::string& value)
{
    uint16_t data = (uint16_t)std::bitset<16>(value).to_ulong();
    data &= 0x0FFF;
    
    return data;
}

void Assembly::HandleDirective(line_ptr line)
{
    if (line->instruction == eInstructionIdentifier::SECTION)
    {
        m_sections.push_back(m_currentSection);
        m_currentSection = nullptr;

        for (auto section: m_sections)
            if (section->name == line->operands[0].value)
                m_currentSection = section;

        if (m_currentSection == nullptr)
            throw AssemblyException("Section " + line->operands[0].value + "is not found in sections vector in second pass.");
    }
}
