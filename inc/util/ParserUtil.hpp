#pragma once

#include <string>

typedef uint8_t BYTE;

namespace ParserUtil
{
    enum eInstructionIdentifier
    {
        // directive
        GLOBAL, EXTERN, WORD, SECTION, SKIP, LBL, END,
        // branch
        JMP, CALL,
        BEQ, BNE, BGT,
        // processor
        HALT, INT, IRET, RET,
        // stack
        PUSH, POP,
        // data
        XCHG, ADD, SUB, MUL, DIV, NOT, AND, OR, XOR, SHL, SHR,
        // memory
        LD, ST,
        // special
        CSRRD, CSRWR
    };

    enum eInstructionType
    {
        DIRECTIVE = 0x00,
        BRANCH = 0x01,
        PROCESSOR = 0x02,
        STACK = 0x03,
        DATA = 0x04,
        MEMORY = 0x05,
        SPECIAL = 0x06,
        LABEL = 0x07
    };

    enum eOperandType
    {
        NONE_TYPE = 0,
        GPR,
        CSR,
        SYM,
        LTR
    };

    enum eAddressingType
    {
        ADDR_NONE = 0,
        ADDR_DIRECT,
        ADDR_MEMORY,
        ADDR_MEMORY_OFFSET        
    };

    enum eRelocationType
    {
        REL_LOCAL,
        REL_EXTERN
    };

    enum eGPR
    {
        R0 = 0x00,  R1 = 0x01,  R2 = 0x02,  R3 = 0x03,
        R4 = 0x04,  R5 = 0x05,  R6 = 0x06,  R7 = 0x07,
        R8 = 0x08,  R9 = 0x09,  R10 = 0x0A, R11 = 0x0B,
        R12 = 0x0C, R13 = 0x0D, R14 = 0x0E, R15 = 0x0F
    };

    enum eCSR
    {
        STATUS = 0x00, HANDLER = 0x01, CAUSE = 0x02
    };

    struct ParserOperand
    {
        std::string value;
        eOperandType type;

        std::string offset;
        eOperandType offsetType;

        eAddressingType addressingType;

        ParserOperand(){}

        ParserOperand(std::string value, eOperandType type):
            value(value), type(type)
        {}

        ParserOperand(std::string value, eOperandType type, std::string offset, eOperandType offsetType, eAddressingType addressingType):
            value(value), type(type), offset(offset), offsetType(offsetType), addressingType(addressingType)
        {}
    };

    struct ConditionalJumpOperands
    {
        std::string gpr1;
        std::string gpr2;
        ParserOperand operand;
    };

    eGPR GPRStringToEnum(std::string reg);
    eCSR CSRStringToEnum(std::string csr);
}