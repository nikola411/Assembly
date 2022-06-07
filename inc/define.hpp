#ifndef _DEFINE_HPP
#define _DEFINE_HPP

#include <string>
#include <vector>
#include <map>

// ENUMS USED IN PARSING
enum Directive_type
{
    GLOBAL, EXTERN, SECTION, WORD, SKIP, END, LABEL
}; // level a directives only

enum Instruction_type
{
    PUSH, POP, XCHG,
    ADD, SUB, DIV, MUL,
    CMP, NOT, AND, OR, XOR, TEST, SHL, SHR,
    LDR, STR,
    IRET, HALT, RET, CALL, INT
};

enum Jump_type
{
    JMP, JEQ, JNE, JGT
};

enum Label_type
{
    SYMBOL, LITERAL, REGISTER
};

// LITERAL_OFFSET AND SYMBOL_OFFSET ARE MEMORY TYPE ADDRESSINGS BUT WITH DIFFERENT OFFSETS

enum Addressing_type
{
    ABSOLUTE, MEMORY, PC_RELATIVE, LITERAL_OFFSET, SYMBOL_OFFSET
};

// IDEA IS THAT WE MIX LABEL TYPE WITH ADDRESSING TYPE AND THAT WAY GENERATE ALL COMIBNATIONS THAT WE CAN HAVE
// ALSO, WE CAN MAKE DATA_ADDRESSING_TYPE AND JUMP_ADDRESSING_TYPE TO HAVE DIFFERENCES BETWEEN JUMPS AND DATA

// STRUCTS USED FOR ASSEMBLER
struct Forward_reference
{
    int value; // address in code that we need to change when we arrive to the label
};

struct Symbol_table_entry
{
    std::string label;
    int size;
    int offset;
    std::string type;
    std::string binding;
    
    std::string section;
    bool defined;
    std::vector<int> fref;
};

enum Section_type
{
    TEXT, DATA, RODATA, BSS, REL
};

// std::map<Directive_type, std::string> directive_map =
// {
//     {Directive_type::GLOBAL , ".global"},
//     {Directive_type::EXTERN , ".extern"},
//     {Directive_type::WORD , ".word"},
//     {Directive_type::SECTION , ".section"},
//     {Directive_type::SKIP , ".skip"},
//     {Directive_type::END , ".end"}
// };

enum Relocation_type
{
    R_ABSOLUTE, R_PC_RELATIVE
};

struct Relocation_entry
{
    int offset;
    std::string section;
    int ord_number;
    Relocation_type type;
};

struct Section_entry
{
    int offset;
    std::vector<char> data;
};

#endif