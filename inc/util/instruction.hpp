#ifndef _INSTRUCTION_HPP
#define _INSTRUCTION_HPP

#include <vector>
#include <memory>
#include <stdint.h>

#define EMPTY_BYTE 0xFF
#define BYTE_SIZE 8

typedef uint8_t BYTE;

namespace AssemblyUtil
{
    class Instruction
    {
    public:
        Instruction();

        void SetInstructionCode(uint8_t code);
        void SetInstructionRegisterA(uint8_t rA);
        void SetInstructionRegisterB(uint8_t rB);
        void SetInstructionRegisterC(uint8_t rC);
        void SetInstructionPayload(uint16_t payload);

        std::vector<BYTE> GetInstructionVector() const;
        uint32_t GetInstructionUInt32() const;
    private:
        std::vector<BYTE> instructionData;

    };

    typedef std::shared_ptr<Instruction> instruction_ptr;
};

#endif