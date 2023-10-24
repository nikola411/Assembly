#ifndef _UTIL_HPP
#define _UTIL_HPP

#include <stdint.h>
#include <string>
#include <unordered_map>
#include <map>
#include <memory>
#include <vector>

#include "instruction.hpp"

#define EMPTY_BYTE 0x00

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
        REL_GLOBAL_OFFSET,
        REL_
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

namespace AssemblyUtil
{
    struct SymbolTableEntry
    {
        std::string label;
        std::string section;
        int offset;
        bool local;

        SymbolTableEntry(std::string label = "", std::string section = "", int offset = 0, bool local = true)
            :label(label), section(section), offset(offset), local(local)
        {
        }
    };

    struct RelocationTableEntry
    {
        std::string label;
        std::string section;
        ParserUtil::eRelocationType type; // promeni ovo u enum
        int offset; // promeni ovo u byte
    };

    struct SectionEntry
    {
        std::string name;
        std::vector<BYTE> sectionData;
        int locationCounter;
    };

    struct ProgramLine
    {
        ParserUtil::eInstructionIdentifier instruction;
        std::vector<ParserUtil::ParserOperand> operands;
        int numberOfArguments;
        ParserUtil::eInstructionType type;
        ParserUtil::eAddressingType addressingType;
    };

    class AssemblyException: public std::exception
    {
    public:
        AssemblyException(std::string msg)
        {
            m_msg = msg;
        }

        const char* what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override
        {
            return m_msg.c_str();
        }

    private:
        std::string m_msg;
    };

    typedef std::shared_ptr<SymbolTableEntry> symbol_ptr;
    typedef std::shared_ptr<RelocationTableEntry> relocation_ptr;
    typedef std::shared_ptr<ProgramLine> line_ptr;
    typedef std::shared_ptr<SectionEntry> section_ptr;

    symbol_ptr FindSymbol(std::vector<symbol_ptr>& table, std::string& name);
    bool InsertSymbol(std::vector<symbol_ptr>& table, symbol_ptr symbol);
    bool UpdateSymbolOffset(std::vector<symbol_ptr>& table, symbol_ptr symbol, int offset);
    bool UpdateSymbolSection(std::vector<symbol_ptr>& table, symbol_ptr symbol, std::string section);
    bool UpdateSymbolLocality(std::vector<symbol_ptr>& table, symbol_ptr symbol, bool isLocal);
    bool WriteDataToSection(section_ptr sections, const std::vector<BYTE>& data, int offset);
    bool WriteDataToSection(AssemblyUtil::section_ptr section, uint32_t data, int offset, int bytesToWrite);
    bool AllocateSectionData(section_ptr section, int size);
    
    std::vector<BYTE> ReadDataFromSection(std::vector<section_ptr>& sections, std::string& name, int offsetStart, int offsetEnd);
}

#endif