#ifndef EMULATOR_HPP
#define EMULATOR_HPP

#include <string>
#include <cinttypes>
#include <vector>
#include <map>

#define MEMORY_SIZE 64 * 1024
#define SP_START 0xFEF6 / 2 - 1
#define PR_START 0xFEF6 / 2
#define SP_END 0x0008
#define PR_END 0xFEFF

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned short* REGISTER_PTR;

struct Instruction
{
    BYTE op_code;
    BYTE registers;
    BYTE addressing;
    BYTE load1;
    BYTE load2;
};

enum Instruction_type
{
    PUSH, POP, XCHG,
    ADD, SUB, DIV, MUL,
    CMP, NOT, AND, OR, XOR, TEST, SHL, SHR,
    LDR, STR,
    IRET, HALT, RET, INT
};

enum Jump_type
{
    JMP, JEQ, JNE, JGT, CALL
};

enum Label_type
{
    SYMBOL, LITERAL, REGISTER
};

enum Instruction_read_state
{
    CODE = 1, REGISTERS = 2, ADDRESSING = 3, PAYLOAD1 = 4, PAYLOAD2 = 5, EXECUTION = 6
};

class Emulator
{
public:
    Emulator(std::string input_file);
    void read_input_and_load();
    void start_emulating();
    ~Emulator();


    std::vector<std::string> split(char del, std::string agg);
    void execute_instruction(Instruction* to_execute);

    WORD read_register_value(std::string reg) const;
    void write_register_value(WORD value, std::string reg);

    BYTE read_memory_byte(int offset) const;
    void write_memory_byte(int offset, BYTE byte);

    BYTE memory[MEMORY_SIZE];
    WORD r0, r1, r2, r3, r4, r5, sp, pc, psw;

    static std::map<BYTE, Instruction_type> instruction_codes;
    static std::map<BYTE, Jump_type> jump_codes;
    static std::map<BYTE, std::string> register_codes;
    static std::map<BYTE, std::string> addressing_codes;

    std::map<std::string, REGISTER_PTR> registers;
    std::map<BYTE, REGISTER_PTR> r;
    
    std::string input_file;
};

#endif