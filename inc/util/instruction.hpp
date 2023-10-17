#ifndef _INSTRUCTION_HPP
#define _INSTRUCTION_HPP

#include <vector>
#include <memory>
#include <stdint.h>
#include <unordered_map>

#define EMPTY_BYTE 0xFF
#define BYTE_SIZE 8

typedef uint8_t BYTE;

namespace AssemblyUtil
{
    class Instruction
    {
    public:
        Instruction();
        Instruction(uint32_t instructionCode);

        void SetInstructionCode(uint16_t code);
        void SetInstructionRegisterA(uint16_t rA);
        void SetInstructionRegisterB(uint16_t rB);
        void SetInstructionRegisterC(uint16_t rC);
        void SetInstructionPayload(uint16_t payload);

        void CallMethod(int num, uint16_t value);

        std::vector<BYTE> GetInstructionVector() const;
        uint32_t GetInstructionUInt32() const;
    private:
        std::vector<BYTE> m_instructionData;
    };

    typedef std::shared_ptr<Instruction> instruction_ptr;
};

#endif