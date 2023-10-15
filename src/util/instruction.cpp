#include "instruction.hpp"

using namespace AssemblyUtil;

Instruction::Instruction()
{
    m_instructionData = {EMPTY_BYTE, EMPTY_BYTE, EMPTY_BYTE, EMPTY_BYTE};
}

AssemblyUtil::Instruction::Instruction(uint32_t instructionCode)
{
    m_instructionData = {EMPTY_BYTE, EMPTY_BYTE, EMPTY_BYTE, EMPTY_BYTE};

    for (uint8_t i = 0; i < 4; ++i)
    {
        m_instructionData[i] = instructionCode >> (3 - i ) * BYTE_SIZE;
    }
}

void Instruction::SetInstructionCode(uint16_t code)
{
    m_instructionData[0] = (uint8_t)code;
}

void Instruction::SetInstructionRegisterA(uint16_t rA)
{
    m_instructionData[1] = (uint8_t)(0x0F & m_instructionData[1]);
    m_instructionData[1] |= (uint8_t)(rA << 4);
}

void Instruction::SetInstructionRegisterB(uint16_t rB)
{
    m_instructionData[1] &= (uint8_t)0xF0;
    m_instructionData[1] |= (uint8_t)(rB & 0x0F);
}

void Instruction::SetInstructionRegisterC(uint16_t rC)
{
    m_instructionData[2] = (uint8_t)(0x0F & m_instructionData[2]);
    m_instructionData[2] |= (uint8_t)(rC << 4);
}

void Instruction::SetInstructionPayload(uint16_t payload)
{
    payload &= 0x0FFF;
    m_instructionData[2] &= (uint8_t)0xF0;
    m_instructionData[2] |= (uint8_t)(payload >> 8);
    m_instructionData[3] = (uint8_t)(payload & 0x0FF);
}

std::vector<BYTE> AssemblyUtil::Instruction::GetInstructionVector() const
{
    return m_instructionData;
}

uint32_t AssemblyUtil::Instruction::GetInstructionUInt32() const
{
    uint32_t instruction = 0;

    for (uint8_t i = 0; i < 4; i++)
    {
        instruction |= m_instructionData[i] << (3 - i) * BYTE_SIZE;
    }
    
    return instruction;
}
