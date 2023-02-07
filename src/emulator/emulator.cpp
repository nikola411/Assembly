#include "emulator.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <iomanip>
#include <sstream>

std::map<BYTE, Instruction_type> Emulator::instruction_codes
{
    { 0x30, IRET },
    { 0x00, HALT },
    { 0x40, RET },
    { 0x10, INT },
    { 0x60, XCHG },
    { 0x70, ADD },
    { 0x71, SUB },
    { 0x72, MUL },
    { 0x73, DIV },
    { 0x74, CMP },
    { 0x80, NOT },
    { 0x81, AND },
    { 0x82, OR },
    { 0x83, XOR },
    { 0x84, TEST },
    { 0x90, SHL },
    { 0x91, SHR },
    { 0xB0, STR },
    { 0xA0, LDR },
    { 0xB0, PUSH },
    { 0xA0, POP }
};

std::map<BYTE, Jump_type> Emulator::jump_codes
{
    { 0x50, JMP},
    { 0x51, JEQ},
    { 0x52, JNE},
    { 0x53, JGT},
    { 0x30, CALL}
};

std::map<BYTE, std::string> Emulator::register_codes
{
    {0x00, "r0"},
    {0x01, "r1"},
    {0x02, "r2"},
    {0x03, "r3"},
    {0x04, "r4"},
    {0x05, "r5"},
    {0x06, "r6"},
    {0x06, "sp"},
    {0x07, "r7"},
    {0x07, "pc"},
    {0x08, "psw"}
};

std::map<BYTE, std::string> Emulator::addressing_codes
{
    {0x00, "IMMEDIATE"},
    {0x01, "REGISTER_DIRECT"},
    {0x05, "REGISTER_ADD"},
    {0x02, "REGISTER_INDIRECT"},
    {0x03, "REGISTER_OFFSET"},
    {0x04, "MEMORY"}
};

Emulator::Emulator(std::string input_file) : input_file(input_file)
{
    pc = PR_START;

    registers["r0"] = &r0;
    registers["r1"] = &r1;
    registers["r2"] = &r2;
    registers["r3"] = &r3;
    registers["r4"] = &r4;
    registers["r5"] = &r5;
    registers["pc"] = &pc;
    registers["sp"] = &sp;
    registers["psw"] = &psw;

    r[0] = &r0;
    r[1] = &r1;
    r[2] = &r2;
    r[3] = &r3;
    r[4] = &r4;
    r[5] = &r5;
    r[6] = &sp;
    r[7] = &pc;
    r[8] = &psw;
}

void Emulator::start_emulating()
{
    int state = CODE;
    int i = 0;
    while (i++ < 10)
    {
        BYTE byte = memory[pc];
        ++pc;
        Instruction* current_instruction = new Instruction();
        std::cout << std::hex << (int)byte << " ";
        switch (state)
        {
            case CODE:
            {
                if (instruction_codes.find(byte) == instruction_codes.end() && jump_codes.find(byte) == jump_codes.end())
                {
                    // std::cout << "ERROR! Operation does not exist! \n";
                    // exit(1);
                }

                //current_instruction->op_code = jump_codes.find(byte) == jump_codes.end() ? instruction_codes[byte] : jump_codes[byte];
                current_instruction->op_code = byte;
                state = REGISTERS;
                std::cout << "CODE \n";
                break;
            }
            case REGISTERS:
            {
                if (register_codes.find(byte >> 4) == register_codes.end() && register_codes.find(byte & 0x0F) == register_codes.end())
                {
                    std::cout << "ERROR! Wrong register! ";
                    // exit(1);
                }

                current_instruction->registers = byte;
                state = ADDRESSING;
                std::cout << "REGISTERS \n";
                break;
            }
            case ADDRESSING:
            {
                if (addressing_codes.find(byte >> 4) == addressing_codes.end())
                {
                    // std::cout << "ERROR! Wrong register! ";
                    // exit(1);
                }

                current_instruction->addressing = byte;

                if (addressing_codes[byte] == "REGISTER_DIRECT" || addressing_codes[byte] == "REGISTER_INDIRECT")
                {
                    state = EXECUTION;
                }
                else
                {
                    state = PAYLOAD1;
                }
                std::cout << "ADDRESSING \n";
                break;
            }
            case PAYLOAD1:
            {
                current_instruction->load1 = byte;
                state = PAYLOAD2;
                std::cout << "PAYLOAD1 \n";
                break;
            }
            case PAYLOAD2:
            {
                current_instruction->load2 = byte;
                state = EXECUTION;
                std::cout << "PAYLOAD2 \n";
                break;
            }
            case EXECUTION:
            {
                execute_instruction(current_instruction);
                state = CODE;
                std::cout << "EXECUTION \n";
                break;
            }
            default: break;
        }

        delete current_instruction;
    }
}

void Emulator::read_input_and_load()
{
    std::ifstream current_file(input_file, std::ios_base::in);
    std::string line;
    std::stringstream ss;
    unsigned int value; 
    BYTE start;

    while (std::getline(current_file, line))
    {
        std::vector<std::string> string_bytes = split(' ', line);
        start = (BYTE)atoi(string_bytes[0].substr(0, 4).c_str());

        for (int i = 1; i < string_bytes.size(); i++)
        {
            ss.clear();
            ss << std::hex << string_bytes[i];
            ss >> value;

            if (PR_START + start + i - 1 > PR_END)
            {
                // ERROR
            }

            memory[PR_START + start + i - 1] = (BYTE)value;
        }
    }
}

Emulator::~Emulator()
{

}

void Emulator::execute_instruction(Instruction* to_execute)
{
    switch (to_execute->op_code)
    {
        case ADD:
        {
            BYTE r_byte = to_execute->registers;
            *r[r_byte >> 4] = *r[r_byte >> 4] + *r[r_byte & 0x0F]; 
            break;
        }
        case SUB:
        {
            BYTE r_byte = to_execute->registers;
            *r[r_byte >> 4] = *r[r_byte >> 4] - *r[r_byte & 0x0F]; 
            break;
        }
        case MUL:
        {
            BYTE r_byte = to_execute->registers;
            *r[r_byte >> 4] = *r[r_byte >> 4] * *r[r_byte & 0x0F]; 
            break;
        }
        case DIV:
        {
            BYTE r_byte = to_execute->registers;
            *r[r_byte >> 4] = *r[r_byte >> 4] / *r[r_byte & 0x0F]; 
            break;
        }
        case NOT:
        {
            BYTE r_byte = to_execute->registers & 0x0F;
            *r[r_byte] = !*r[r_byte];
        }
        case AND:
        {
            BYTE r_byte = to_execute->registers;
            *r[r_byte >> 4] = *r[r_byte >> 4] & *r[r_byte & 0x0F]; 
            break;
        }
        case OR:
        {
            BYTE r_byte = to_execute->registers;
            *r[r_byte >> 4] = *r[r_byte >> 4] | *r[r_byte & 0x0F]; 
            break;
        }
        case XOR:
        {
            BYTE r_byte = to_execute->registers;
            *r[r_byte >> 4] = *r[r_byte >> 4] ^ *r[r_byte & 0x0F]; 
            break;
        }
        
        

        default: break;
    }
}

WORD Emulator::read_register_value(std::string reg) const
{
    auto ptr = registers.find(reg);
    return *ptr->second;
}

void Emulator::write_register_value(WORD value, std::string reg)
{
    *registers[reg] = value;
}

BYTE Emulator::read_memory_byte(int offset) const
{
    if (offset > MEMORY_SIZE)
    {
        // ERROR , reading out of memory
    }

    return memory[offset];
}

void Emulator::write_memory_byte(int offset, BYTE byte)
{
    if (offset > MEMORY_SIZE)
    {
        // ERROR , reading out of memory
    }

    memory[offset] = byte;
}

std::vector<std::string> Emulator::split(char del, std::string agg)
{
    int last_del = 0;
    std::string ins = "";
    std::vector<std::string> ret;
    
    for (int i = 0; i < agg.size(); i++)
    {
        if (agg[i] != del)
        {
            if (last_del)
            {
                last_del = 0;
                ins = "";
            }
            
            ins += agg[i];
        }
        else
        {
            if (!last_del)
            {
                ret.emplace_back(ins);
                last_del = 1;
            }
        }
    }

    if (ins != "" && !last_del)
    {
        ret.emplace_back(ins);
    }
    
    return ret;
}

