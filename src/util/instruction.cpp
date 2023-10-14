#include "instruction.hpp"

using namespace AssemblyUtil;

Instruction::Instruction()
{
    instructionData = {EMPTY_BYTE, EMPTY_BYTE, EMPTY_BYTE, EMPTY_BYTE};
}

void Instruction::SetInstructionCode(uint8_t code)
{
    instructionData[0] = code;
}

void Instruction::SetInstructionRegisterA(uint8_t rA)
{
    instructionData[1] = 0x0F & instructionData[1];
    instructionData[1] |= rA << 4;
}

void Instruction::SetInstructionRegisterB(uint8_t rB)
{
    instructionData[1] &= 0xF0;
    instructionData[1] |= (rB & 0x0F);
}

void Instruction::SetInstructionRegisterC(uint8_t rC)
{
    instructionData[2] = 0x0F & instructionData[2];
    instructionData[2] |= rC << 4;
}

void Instruction::SetInstructionPayload(uint16_t payload)
{
    payload &= 0x0FFF;
    instructionData[2] &= 0xF0;
    instructionData[2] |= payload >> 8;
    instructionData[3] = (uint8_t)(payload & 0x0FF);
}

std::vector<BYTE> AssemblyUtil::Instruction::GetInstructionVector() const
{
    return instructionData;
}

uint32_t AssemblyUtil::Instruction::GetInstructionUInt32() const
{
    uint32_t instruction = 0;

    for (uint8_t i = 0; i < 4; i++)
    {
        instruction |= instructionData[i] << (4 - i) * BYTE_SIZE;
    }
    
    return instruction;
}
