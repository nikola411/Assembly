#ifndef _UTIL_HPP
#define _UTIL_HPP

#include <stdint.h>
#include <unordered_map>
#include <string>
#include <memory>
#include <vector>

#define EMPTY_BYTE 0xFF

typedef uint8_t BYTE;

enum eInstructionIdentifier
{
    GLOBAL = 0, EXTERN, WORD, SECTION, SKIP,
    HALT, INT, IRET, RET,
    JMP, CALL,
    PUSH, POP, NOT
};

enum eInstructionType
{
    DIRECTIVE,
    ZERO_OPERAND_INSTRUCTION,
    ONE_OPERAND_INSTRUCTION,
    TWO_OPERAND_INSTRUCTION,
    BRANCH_INSTRUCTION // 3 operands
};

enum eOperandType
{
    GPR = 0,
    CSR,
    SYM,
    LTR
};

enum eAddressingType
{
    DIRECT = 0,
    MEMORY
};

enum eRelocationType
{

};

struct Instruction
{
    std::vector<BYTE> instructionData;

    Instruction()
    {
        instructionData = {EMPTY_BYTE, EMPTY_BYTE, EMPTY_BYTE, EMPTY_BYTE};
    }
};

struct SymbolTableEntry
{
    std::string label;
    std::string section;
    int offset;
    bool local;

    SymbolTableEntry(std::string label = "", std::string section = "", int offset = 0, bool local = false)
        :label(label), section(section), offset(offset), local(local)
    {

    }
};

struct RelocationTableEntry
{
    std::string label;
    std::string section;
    eRelocationType type; // promeni ovo u enum
    BYTE value; // promeni ovo u byte
    BYTE addend; // promeni ovo u int
};

struct ProgramLine
{
public:
    eInstructionIdentifier instruction;
    std::vector<std::pair<std::string, eOperandType>> operands;
    int numberOfArguments;
    eInstructionType type;
};

typedef std::shared_ptr<SymbolTableEntry> symbol_ptr;
typedef std::shared_ptr<RelocationTableEntry> relocation_ptr;
typedef std::shared_ptr<Instruction> instruction_ptr;
typedef std::shared_ptr<ProgramLine> line_ptr;

#endif