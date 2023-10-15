#include "assembly.hpp"
#include "instruction.hpp"
#include "codes.hpp"

#include <iostream>
#include <memory>

#define LOCAL true
#define GLOBAL false
#define DEFAULT_SECTION "default"

static const int INSTRUCTION_SIZE = 4;

using namespace AssemblyUtil;

Assembly::Assembly():
    m_end(false), m_locationCounter(0)
{
    ProcessorUtil::CodesMap::PopulateMap();

    m_currentSection = std::make_shared<SectionEntry>();
    m_currentSection->name = DEFAULT_SECTION;
}

void Assembly::SetInstruction(ParserUtil::eInstructionIdentifier instruction, ParserUtil::eInstructionType type)
{
    m_currentLine = std::make_shared<ProgramLine>();
    m_currentLine->instruction = instruction;
    m_currentLine->type = type;
}

void Assembly::SetOperand(std::string value, ParserUtil::eOperandType type)
{
    m_currentLine->operands.push_back(ParserUtil::ParserOperand(value, type));
}

void Assembly::SetOperand(ParserUtil::ParserOperand& operand)
{
    m_currentLine->operands.push_back(operand);
}

void Assembly::SetMultipleOperands(std::vector<ParserUtil::ParserOperand> operands)
{
    for (auto op: operands)
    {
        m_currentLine->operands.push_back(op);
    }
}

void Assembly::FinishInstruction()
{
    switch (m_currentLine->type)
    {
        case ParserUtil::eInstructionType::DIRECTIVE:
        {
            if (m_currentLine->instruction == ParserUtil::eInstructionIdentifier::EXTERN)
            {
                for (auto operand: m_currentLine->operands)
                {
                    auto entry = std::make_shared<SymbolTableEntry>(operand.value, m_currentSection->name, m_locationCounter, false);
                    m_symbolTable.push_back(entry);
                }
            }
            else if (m_currentLine->instruction == ParserUtil::eInstructionIdentifier::SECTION)
            {
                auto sectionName = m_currentLine->operands.front().value;

                if (m_currentSection->name != sectionName)
                {
                    m_currentSection->locationCounter = m_locationCounter;
                    m_sections.push_back(m_currentSection);

                    m_locationCounter = 0;
                    m_currentSection = std::make_shared<SectionEntry>();
                    m_currentSection->name = sectionName;
                    
                    m_symbolTable.push_back(std::make_shared<SymbolTableEntry>(sectionName, sectionName));
                }
                else
                {
                    throw AssemblyException("Cannot declare same section two times in a row");
                }
            }

            break;
        }
        case ParserUtil::eInstructionType::LABEL:
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

            break;
        }
        case ParserUtil::eInstructionType::BRANCH:
        case ParserUtil::eInstructionType::DATA:
        case ParserUtil::eInstructionType::MEMORY:
        case ParserUtil::eInstructionType::PROCESSOR:
        case ParserUtil::eInstructionType::SPECIAL:
        case ParserUtil::eInstructionType::STACK:
            m_locationCounter += INSTRUCTION_SIZE;
            break;
    }

    m_program.push_back(m_currentLine);
}

void Assembly::ContinueParsing()
{
    std::cout << "SYMBOL TABLE \n";
    for (auto sym: m_symbolTable)
    {
        std::cout << sym->label << " " << sym->offset << " " << sym->section << "\n";
    }

    for (auto line: m_program)
    {
        m_currentLine = line;
        auto addressingType = GetProcessorAddressingType(m_currentLine->addressingType);
        auto variantOperandNumber = VariantOperandNumber(m_currentLine);
        auto operandType = variantOperandNumber == 0 ? GetProcessorOperandType(ParserUtil::eOperandType::GPR) :
                        CanOperandHaveOffset(m_currentLine) ? GetProcessorOperandType(m_currentLine->operands[variantOperandNumber].type) :
                            GetProcessorOperandType(ParserUtil::eOperandType::SYM);
    }
}

void Assembly::PrintProgram()
{

}

void Assembly::HandleDirective()
{
}

void Assembly::HandleBranchInstruction()
{
}

void Assembly::HandleDataInstruction()
{
}

void Assembly::HandleMemoryInstruction()
{
}

void Assembly::HandleProcessorInstruction()
{
}

void Assembly::HandleSpecialInstruction()
{
}

void Assembly::HandleStackInstruction()
{
}

ProcessorUtil::eAddressingType Assembly::GetProcessorAddressingType(ParserUtil::eAddressingType addressingType) const
{
    return (ProcessorUtil::eAddressingType)addressingType;
}

int Assembly::VariantOperandNumber(AssemblyUtil::line_ptr line) const
{
    if (line->instruction == ParserUtil::eInstructionIdentifier::LD ||
            line->instruction == ParserUtil::eInstructionIdentifier::JMP ||
            line->instruction == ParserUtil::eInstructionIdentifier::CALL)
        return 1;
    if (line->instruction == ParserUtil::eInstructionIdentifier::ST)
        return 2;
    if (line->instruction == ParserUtil::eInstructionIdentifier::BEQ ||
            line->instruction == ParserUtil::eInstructionIdentifier::BNE ||
            line->instruction == ParserUtil::eInstructionIdentifier::BGT)
        return 3;

    return 0;
}

bool Assembly::CanOperandHaveOffset(AssemblyUtil::line_ptr line) const
{
    if (line->instruction == ParserUtil::eInstructionIdentifier::LD || line->instruction == ParserUtil::eInstructionIdentifier::ST)
        return true;
    return false;
}

ProcessorUtil::eOperandType Assembly::GetProcessorOperandType(ParserUtil::eOperandType operandType) const
{
    if (operandType == ParserUtil::eOperandType::CSR || operandType == ParserUtil::eOperandType::GPR)
        return ProcessorUtil::eOperandType::REGISTER;
    if (operandType == ParserUtil::eOperandType::SYM || operandType == ParserUtil::eOperandType::LTR)
        return ProcessorUtil::eOperandType::IMMEDIATE;
    
    throw AssemblyUtil::AssemblyException("Wrong argument passed for conversion on line: " + __LINE__);
}
