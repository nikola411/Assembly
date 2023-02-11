#ifndef EMULATOR_HPP
#define EMULATOR_HPP

#include <string>
#include <cinttypes>
#include <vector>
#include <map>
#include <iostream>
#include <bitset>

#define MEMORY_SIZE 64 * 1024
#define PR_END 0xFEF6 / 2 - 1
#define SP_END 0xFEF6 / 2
#define PR_START 0x0010
#define SP_START 0xFEFF

#define IVT_RESET 0x0000 * 2
#define IVT_ERROR 0x0001 * 2
#define IVT_TIMER 0x0002 * 2
#define IVT_TERMINAL 0x0003 * 2
#define IVT_END 0x0007 * 2

#define TERM_OUT 0xFF00 
#define TERM_IN 0xFF02
#define TIM_CFG 0xFF10

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned short* REGISTER_PTR;

class Terminal;
struct Instruction;
enum Instruction_type : short;
enum Label_type : short;
enum Instruction_read_state : short;

class Emulator
{
public:
    Emulator(std::string input_file);
    void read_input_and_load();
    void start_emulating();
    ~Emulator();

    void finish_emulation();

    void handle_ldr(Instruction* instr);
    void handle_str(Instruction* instr);
    void handle_call(Instruction* instr);

    void fill_z_flag(WORD);
    void fill_n_flag(WORD);
    void fill_c_flag(bool);

    void stack_push(WORD);
    WORD stack_pop();
    void update_register(BYTE up, REGISTER_PTR reg);
    static void write_mem_reg(BYTE byte, int offset);
    static BYTE read_mem_reg(int offset);

    std::vector<std::string> split(char del, std::string agg);
    void execute_instruction(Instruction* to_execute);

    static BYTE memory[MEMORY_SIZE];
    WORD r0, r1, r2, r3, r4, r5, sp, pc, psw;

    static std::map<BYTE, Instruction_type> instruction_codes;
    static std::map<BYTE, std::string> register_codes;
    static std::map<BYTE, std::string> addressing_codes;

    std::map<BYTE, REGISTER_PTR> r;
    
    bool finished;
    std::string input_file;
    Terminal* term;
};

#endif