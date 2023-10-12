#include "assembly.hpp"

#include <iostream>
#include <memory>

Assembly::Assembly()
{
}

void Assembly::SetInstruction(eInstructionIdentifier instruction, eInstructionType type)
{
    m_currentLine = std::make_shared<ProgramLine>();
    m_currentLine->instruction = instruction;
    m_currentLine->type = type;
}

void Assembly::SetOperand(std::string value, eOperandType type)
{
    m_currentLine->operands.push_back({ value, type });
}

void Assembly::SetMultipleOperands(std::vector<std::pair<std::string, eOperandType>> operands)
{
    for (auto op: operands)
    {
        m_currentLine->operands.push_back({ op.first, op.second });
    }
}

void Assembly::SetAddressingType(eAddressingType addrType)
{

}

void Assembly::FinishInstruction()
{
    for (auto op: m_currentLine->operands)
    {
        std::cout << op.first << " " << op.second << "\n";
    }
}

void Assembly::ParseLine()
{
    switch(m_currentLine->operands.size())
    {
        case 0:
        {
            break;
        }
        case 1:
        {
            break;
        }
        case 2:
        {
            break;
        }
        case 3:
        {
            break;
        } 
    }

    m_program.push_back(m_currentLine);
}
