#include "Instruction.hpp"

#define UNIT_SIZE 4

#define code_offset 6 * UNIT_SIZE
#define rA_offset 5 * UNIT_SIZE
#define rB_offset 4 * UNIT_SIZE
#define rC_offset 3 * UNIT_SIZE
#define pd_offset 0

#define char_mask 0xF

#define cd_mask 0xFF << code_offset
#define rA_mask char_mask << rA_offset
#define rB_mask char_mask << rB_offset
#define rC_mask char_mask << rC_offset
#define pd_mask 0xFFF << pd_offset

using namespace AssemblyUtil;

Instruction::Instruction()
{
    m_instructionData = { EMPTY_BYTE, EMPTY_BYTE, EMPTY_BYTE, EMPTY_BYTE };
    m_intInstructionData = 0xFFFFFFFF;
}

Instruction::Instruction(uint32_t instructionCode)
{
    m_instructionData = { EMPTY_BYTE, EMPTY_BYTE, EMPTY_BYTE, EMPTY_BYTE };
    m_intInstructionData = instructionCode;
}

void Instruction::SetInstructionCode(uint16_t code)
{
    m_intInstructionData = 0;
    m_intInstructionData |= (uint32_t)(((code & 0xFF) << code_offset) & cd_mask);
}

void Instruction::SetInstructionRegisterA(uint16_t rA)
{
    m_intInstructionData |= (uint32_t)(((rA & 0x0F) << rA_offset));
}

void Instruction::SetInstructionRegisterB(uint16_t rB)
{
    m_intInstructionData |= (uint32_t)(((rB & 0x0F) << rB_offset) & rB_mask);
}

void Instruction::SetInstructionRegisterC(uint16_t rC)
{
    m_intInstructionData |= (uint32_t)(((rC & 0x0F) << rC_offset) & rC_mask);
}

void Instruction::SetInstructionPayload(uint16_t payload)
{
    m_intInstructionData |= (uint32_t)(((payload & 0xFFF) << pd_offset) & pd_mask);
}

void AssemblyUtil::Instruction::DoNothing(uint16_t payload)
{
    return;
}

std::vector<BYTE> Instruction::GetInstructionVector()
{
    for(auto i = 0; i < 4; ++i)
    {
        m_instructionData[i] = (BYTE)(m_intInstructionData >> (3 - i) * BYTE_SIZE);
    }

    return m_instructionData;
}

uint32_t Instruction::GetInstructionUInt32() const
{
    return m_intInstructionData;
}
