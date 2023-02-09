#ifndef EMULATOR_HPP
#define EMULATOR_HPP

#include <string>
#include <cinttypes>
#include <vector>
#include <map>
#include <iostream>
#include <bitset>

#define MEMORY_SIZE 64 * 1024
#define SP_START 0xFEF6 / 2 - 1
#define PR_START 0xFEF6 / 2
#define SP_END 0x0008
#define PR_END 0xFEFF

#define IVT_RESET 0x0000
#define IVT_ERROR 0x0001
#define IVT_TIMER 0x0002
#define IVT_TERMINAL 0x0003
#define IVT_END 0x0007

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

    void stack_push(WORD);
    WORD stack_pop();
    void update_register(BYTE up, REGISTER_PTR reg);

    std::vector<std::string> split(char del, std::string agg);
    void execute_instruction(Instruction* to_execute);

    BYTE memory[MEMORY_SIZE];
    WORD r0, r1, r2, r3, r4, r5, sp, pc, psw;

    static std::map<BYTE, Instruction_type> instruction_codes;
    static std::map<BYTE, std::string> register_codes;
    static std::map<BYTE, std::string> addressing_codes;

    std::map<BYTE, REGISTER_PTR> r;
    
    bool finished;
    std::string input_file;
};

#endif