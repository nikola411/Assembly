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

namespace AssemblyUtil
{
    struct SymbolTableEntry
    {
        std::string label;
        std::string section;
        int offset;
        bool local;
        bool defined;
        int index;

        SymbolTableEntry(std::string label = "", std::string section = "", int offset = 0, bool local = true, bool defined = false, int index = 0)
            :label(label), section(section), offset(offset), local(local), defined(defined), index(index)
        {
        }
    };

    struct RelocationTableEntry
    {
        std::string label;
        std::string section;
        ParserUtil::eRelocationType type;
        int offset;
        int index;
    };

    struct SectionEntry
    {
        std::string name;
        std::vector<BYTE> sectionData;
        int locationCounter;
        int offset;
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
        AssemblyException(std::string msg): m_msg(msg) {}
        std::string what(){return m_msg;}
    private:
        std::string m_msg;
    };

    typedef std::shared_ptr<SymbolTableEntry> symbol_ptr;
    typedef std::shared_ptr<RelocationTableEntry> relocation_ptr;
    typedef std::shared_ptr<ProgramLine> line_ptr;
    typedef std::shared_ptr<SectionEntry> section_ptr;

    symbol_ptr FindSymbol(std::vector<symbol_ptr>& table, std::string& name);
    symbol_ptr CreateSymbolEntry(std::string label = "", std::string section = "", int offset = 0, bool local = true, bool defined = false, int index = 0);
    bool InsertSymbol(std::vector<symbol_ptr>& table, symbol_ptr symbol);
    bool UpdateSymbolOffset(std::vector<symbol_ptr>& table, symbol_ptr symbol, int offset);
    bool UpdateSymbolSection(std::vector<symbol_ptr>& table, symbol_ptr symbol, std::string section);
    bool UpdateSymbolLocality(std::vector<symbol_ptr>& table, symbol_ptr symbol, bool isLocal);
    bool WriteDataToSection(section_ptr sections, const std::vector<BYTE>& data, int offset);
    bool WriteDataToSection(AssemblyUtil::section_ptr section, uint32_t data, int offset, int bytesToWrite);
    bool WriteDataToSection(std::vector<AssemblyUtil::section_ptr>& sectionTable, std::string& sectionName, uint32_t data, int offset, int bytesStart, int bytesToWrite);
    bool AllocateSectionData(section_ptr section, int size);
    relocation_ptr FindRelocation(std::vector<relocation_ptr>& relocations, std::string& name);
    std::vector<BYTE> ReadDataFromSection(std::vector<section_ptr>& sections, std::string& name, int offsetStart, int offsetEnd);
}

namespace LinkerUtil
{
    static const std::string OUT_FLAG("-o");
    static const std::string HEX_FLAG("-hex");
    static const std::string RELOCATABLE_FLAG("-relocatable");
    static const std::string LINKER_FLAG("./linker");
    static std::string PLACE_FLAG("-place=");

    class LinkerException: std::exception
    {
    public:
        LinkerException(std::string msg):msg(msg){}
        std::string what(){return msg;}
    private:
        std::string msg;
    };

    struct LinkerArguments
    {
        bool hex;
        bool relocatable;
        std::vector<std::string> place;
        std::string outFile;
        std::vector<std::string> inFiles;
    };

    enum ArgumentParsingState
    {
        DO_NOTHING = 0x00,
        READ_OUTPUT = 0x01
    };

    enum eInputFileState
    {
        SYMBOL_TABLE,
        SECTIONS,
        RELOCATIONS
    };

    struct SectionMergeData
    {
        int index;
        std::string name;
        std::vector<std::pair<int, AssemblyUtil::section_ptr>> sections;

        void AddSection(int file, AssemblyUtil::section_ptr section)
        {
            sections.push_back({ file, section });
        }
    };

    void UpdateSymbolsOffset(std::vector<AssemblyUtil::symbol_ptr>& table, std::string& sectionName, int offset);
    void UpdateRelocationsOffset(std::vector<AssemblyUtil::relocation_ptr>& table, std::string& sectionName, int offset);

    typedef std::vector<AssemblyUtil::symbol_ptr> SymbolTable;
    typedef std::vector<AssemblyUtil::section_ptr> SectionTable;
    typedef std::vector<AssemblyUtil::relocation_ptr> RelocationTable;
    typedef std::shared_ptr<SectionMergeData> SectionMergeDataPtr;
}

int Contains(std::string& base, char c);
bool StartsWith(std::string& base, std::string& start);
std::vector<std::string> Split(std::string& toSplit, char c);

#endif