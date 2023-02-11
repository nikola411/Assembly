#include "emulator.hpp"
#include "emulator_error.hpp"
#include "terminal.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <iomanip>
#include <sstream>
#include <chrono>

const unsigned short Z_FLAG = 0x01 << 0;
const unsigned short O_FLAG = 0x01 << 1;
const unsigned short C_FLAG = 0x01 << 2;
const unsigned short N_FLAG = 0x01 << 3;
const unsigned short TIM_FLAG = 0x01 << 13;
const unsigned short TER_FLAG = 0x01 << 14;
const unsigned short I_FLAG = 0x01 << 15;

BYTE Emulator::memory[MEMORY_SIZE] = {0};

enum Instruction_type : short
{
    PUSH, POP, XCHG, ADD,  SUB,
    DIV,  MUL, CMP,  NOT,  AND,
    OR,   XOR, TEST, SHL,  SHR,
    LDR,  STR, IRET, HALT, RET,
    INT,  JMP, JEQ,  JNE,  JGT, 
    CALL
};

enum Label_type : short
{
    SYMBOL,
    LITERAL,
    REGISTER
};

enum Instruction_read_state : short
{
    CODE = 1,
    REGISTERS = 2,
    ADDRESSING = 3,
    PAYLOAD1 = 4,
    PAYLOAD2 = 5,
    EXECUTION = 6
};

struct Instruction
{
    BYTE op_code;
    BYTE registers;
    BYTE addressing;
    BYTE load1;
    BYTE load2;
};

std::map<BYTE, Instruction_type> Emulator::instruction_codes
{
    {0x20, IRET}, {0x00, HALT}, {0x40, RET}, {0x10, INT}, {0x60, XCHG},
    {0x70, ADD},  {0x71, SUB},  {0x72, MUL}, {0x73, DIV}, {0x74, CMP},
    {0x80, NOT},  {0x81, AND},  {0x82, OR},  {0x83, XOR}, {0x84, TEST},
    {0x90, SHL},  {0x91, SHR},  {0xB0, STR}, {0xA0, LDR}, {0x50, JMP},
    {0x51, JEQ},  {0x52, JNE},  {0x53, JGT}, {0x30, CALL}
};

std::map<BYTE, std::string> Emulator::register_codes
{
    {0x00, "r0"}, {0x01, "r1"}, {0x02, "r2"},
    {0x03, "r3"}, {0x04, "r4"}, {0x05, "r5"},
    {0x06, "r6"}, {0x06, "sp"}, {0x07, "r7"},
    {0x07, "pc"}, {0x08, "psw"}, {0x0F, "EMPTY_REGISTER_FIELD"}
};

std::map<BYTE, std::string> Emulator::addressing_codes{
    {0x00, "IMMEDIATE"},
    {0x01, "REGISTER_DIRECT"},
    {0x05, "REGISTER_DIRECT_OFFSET"},
    {0x02, "REGISTER_INDIRECT"},
    {0x03, "REGISTER_INDIRECT_OFFSET"},
    {0x04, "MEMORY"}};

enum Addressing_type : short
{
    ADR_IMM = 0x00,
    ADR_REG_DIR = 0x01,
    ADR_REG_DIR_OFF = 0x05,
    ADR_REG_IND = 0x02,
    ADR_REG_IND_OFF = 0x03,
    ADR_MEM = 0x04
};

Emulator::Emulator(std::string input_file) : input_file(input_file), finished(false), term()
{
    pc = PR_START;
    psw = 0;
    r0 = 0;
    r1 = 0;
    r2 = 0;
    r3 = 0;
    r4 = 0;
    r5 = 0;
    sp = SP_START - 1;

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
    this->finished = false;

    Instruction *current_instruction = new Instruction();
    // term->start();

    while (pc <= PR_END)
    {
        if (this->finished == true)
            break;

        BYTE byte = memory[pc];
        pc += state != EXECUTION;
        i += state != EXECUTION;

        switch (state)
        {
        case CODE:
        {
            if (instruction_codes.find(byte) == instruction_codes.end())
            {
                throw EmulatorError("Wrong operation code.", i, byte);
            }

            if (instruction_codes[byte] == HALT)
            {
                this->finished = true;
                break;
            }

            current_instruction->op_code = byte;
            state = REGISTERS;

            break;
        }
        case REGISTERS:
        {
            if (register_codes.find(byte >> 4) == register_codes.end() && register_codes.find(byte & 0x0F) == register_codes.end())
            {
                throw EmulatorError("Register address value out of bounds [0-7].", i, byte);
            }

            current_instruction->registers = byte;
            state = ADDRESSING;

            break;
        }
        case ADDRESSING:
        {
            if (addressing_codes.find(byte >> 4) == addressing_codes.end())
            {
                throw EmulatorError("Wrong addressing for given instruction.", i, byte);
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

            break;
        }
        case PAYLOAD1:
        {
            current_instruction->load1 = byte;
            state = PAYLOAD2;

            break;
        }
        case PAYLOAD2:
        {
            current_instruction->load2 = byte;
            state = EXECUTION;

            break;
        }
        case EXECUTION:
        {
            execute_instruction(current_instruction);
            delete current_instruction;
            current_instruction = new Instruction();
            state = CODE;

            break;
        }
        default:
            break;
        }
    }
    finish_emulation();
    // write final output
}

void Emulator::read_input_and_load()
{
    std::ifstream current_file(input_file, std::ios_base::in);
    std::string line;
    std::stringstream ss;
    unsigned int value;
    unsigned int start;

    while (std::getline(current_file, line))
    {
        std::vector<std::string> string_bytes = split(' ', line);
        ss.clear();
        ss << std::hex << string_bytes[0].substr(0, 4);
        ss >> start;

        for (int i = 1; i < string_bytes.size(); i++)
        {
            ss.clear();
            ss << std::hex << string_bytes[i];
            ss >> value;

            if (PR_START + start + i - 1 > PR_END)
            {
                throw EmulatorError("Writing outside of program bounds. ", start + i - 1, (BYTE)value);
            }

            memory[(int)start + i - 1] = (BYTE)value;
        }
    }
}

Emulator::~Emulator()
{
    delete term;
}

void Emulator::finish_emulation()
{
    if (finished)
    {
        std::cout << "Emulated processor executed halt instruction \n";
    }
    else
    {
        std::cout << "Emulated prcessor did not finish execution properly \n";
    }

    std::cout << "Emulated processor state: \n";
    std::cout << "psw=0b" << std::bitset<16>(psw).to_string() << "\n";
    int w = 9;

    std::cout 
            << std::setfill('0');
    
    for (int i = 0; i < 8; i++)
    {
        if (i!= 0 && i % 4 == 0) std::cout << "\n";
        std::cout 
            << "r"
            << i
            << "=0x"
            << std::hex
            << std::setw(4)
            << *r[i]
            << "  ";
    }
    std::cout << "\n";
}

void Emulator::execute_instruction(Instruction *to_execute)
{
    WORD DST = to_execute->registers >> 4;
    WORD SRC = to_execute->registers & 0x0F;

    switch (instruction_codes[to_execute->op_code])
    {
    case HALT:
    {
        finished = true;
        break;
    }
    case INT: // push psw; pc <= mem16[(regD mod 8)*2];
    {
        stack_push(pc);
        stack_push(psw);
        
        pc = memory[(*r[DST] % 8) * 2]  << 8 ;
        pc |= memory[(*r[DST] % 8) * 2 + 1];
        

        break;
    }
    case IRET:
    {
        psw = stack_pop();
        pc = stack_pop();

        break;
    }
    case CALL:
    {
        stack_push(pc);
        handle_call(to_execute);
        break;
    }
    case RET:
    {
        pc = stack_pop();
        break;
    }
    case JMP:
    {
        handle_call(to_execute);
        break;
    }
    case JEQ:
    {
        if (psw & Z_FLAG)
        {
            handle_call(to_execute);
        }
        break;
    }
    case JNE:
    {
        if (!(psw & Z_FLAG))
        {
            handle_call(to_execute);
        }
        break;
    }
    case JGT:
    {
        if (!(psw & N_FLAG))
        {
            handle_call(to_execute);
        }
        break;
    }
    case XCHG:
    {
        WORD temp = *r[DST];
        *r[DST] = *r[SRC];
        *r[SRC] = temp;

        break;
    }
    case ADD:
    {
        *r[DST] = *r[DST] + *r[SRC];
        fill_z_flag(*r[DST]);
        fill_n_flag(*r[DST]);

        break;
    }
    case SUB:
    {
        *r[DST] = *r[DST] - *r[SRC];
        fill_z_flag(*r[DST]);
        fill_n_flag(*r[DST]);

        break;
    }
    case MUL:
    {
        *r[DST] = *r[DST] * *r[SRC];
        fill_z_flag(*r[DST]);
        fill_n_flag(*r[DST]);

        break;
    }
    case DIV:
    {
        *r[DST] = *r[DST] / *r[SRC];
        fill_z_flag(*r[DST]);
        fill_n_flag(*r[DST]);

        break;
    }
    case CMP:
    { // Z O C N
        int temp = *r[DST] - *r[SRC];

        fill_z_flag(temp);
        fill_n_flag(temp);
        fill_c_flag(*r[DST] < *r[SRC]);

        if (temp > 0x7FFF || temp < -0x7FFF)
        {
            psw |= O_FLAG;
        }
        else
        {
            psw &= ~O_FLAG;
        }

        break;
    }
    case NOT:
    {
        *r[DST] = !*r[DST];
        fill_z_flag(*r[DST]);
        fill_n_flag(*r[DST]);

        break;
    }
    case AND:
    {
        *r[DST] = *r[DST] & *r[SRC];
        fill_z_flag(*r[DST]);
        fill_n_flag(*r[DST]);

        break;
    }
    case OR:
    {
        *r[DST] = *r[DST] | *r[SRC];
        fill_z_flag(*r[DST]);
        fill_n_flag(*r[DST]);

        break;
    }
    case XOR:
    {
        *r[DST] = *r[DST] ^ *r[SRC];
        fill_z_flag(*r[DST]);
        fill_n_flag(*r[DST]);

        break;
    }
    case TEST:
    {
        int temp = *r[DST] & *r[SRC];
        fill_z_flag(temp);
        fill_n_flag(temp);

        break;
    }
    case SHL:
    { // Z C N
        fill_c_flag((*r[DST] << (*r[SRC] - 1)) & (1 << 15));
        *r[DST] = *r[DST] << *r[SRC];
        fill_z_flag(*r[DST]);
        fill_n_flag(*r[DST]);

        break;
    }
    case SHR:
    {
        fill_c_flag((*r[DST] >> (*r[SRC] - 1)) & 1 );
        *r[DST] = *r[DST] >> *r[SRC];
        fill_z_flag(*r[DST]);
        fill_n_flag(*r[DST]);

        break;
    }
    case LDR:
    {
        handle_ldr(to_execute);
        break;
    }
    case STR:
    {
        handle_str(to_execute);
        break;
    }
    default:
        break;
    }
}

void Emulator::handle_ldr(Instruction *instr)
{
    std::string addr_type = addressing_codes[instr->addressing & 0x0F];
    WORD value = 0;
    BYTE DST = instr->registers >> 4;
    BYTE SRC = instr->registers & 0x0F;
    WORD PAYLOAD = (WORD)(instr->load1 << 8 | instr->load2);

    if (addr_type == "IMMEDIATE")
    {
        value = PAYLOAD;
    }
    else if (addr_type == "MEMORY")
    {
        WORD base_address = PAYLOAD;
        value = memory[base_address] | memory[base_address + 1] << 8;
    }
    else if (addr_type == "REGISTER_INDIRECT_OFFSET" || addr_type == "REGISTER_INDIRECT")
    {
        if (instr->addressing >> 4 < 3)
        {
            update_register(instr->addressing >> 4, r[SRC]);
        }

        WORD base_address = *r[SRC] + (addr_type == "REGISTER_INDIRECT_OFFSET" ? PAYLOAD : 0);
        BYTE low = memory[base_address];
        BYTE high = memory[base_address + 1];
        value = (WORD)(high << 8 | low);

        if (instr->addressing >> 4 >= 3)
        {
            update_register(instr->addressing >> 4, r[SRC]);
        }
    }
    else if (addr_type == "REGISTER_DIRECT_OFFSET")
    {
        value = *r[SRC] + PAYLOAD;
    }
    else if (addr_type == "REGISTER_DIRECT")
    {
        value = *r[SRC];
    }

    *r[DST] = value;
}

void Emulator::handle_str(Instruction *instr)
{
    std::string addr_type = addressing_codes[instr->addressing & 0x0F];

    if (addr_type == "IMMEDIATE" || addr_type == "REGISTER_DIRECT_OFFSET")
        throw EmulatorError("Wrong addressing type! Can not store data into a literal!", pc, instr->addressing);

    BYTE low = *r[instr->registers >> 4] & 0xFF;
    BYTE high = *r[instr->registers >> 4] >> 8;

    BYTE DST = instr->registers >> 4;
    BYTE SRC = instr->registers & 0x0F;

    WORD PAYLOAD = (WORD)(instr->load1 << 8 | instr->load2);

    if (addr_type == "MEMORY")
    {
        WORD base_address = PAYLOAD;
        memory[base_address] = low;
        memory[base_address + 1] = high;
    }
    else if (addr_type == "REGISTER_INDIRECT_OFFSET" || addr_type == "REGISTER_INDIRECT")
    {
        if (instr->addressing >> 4 < 3)
        {
            update_register(instr->addressing >> 4, r[DST]);
        }

        WORD address = *r[DST] + (addr_type == "REGISTER_INDIRECT_OFFSET" ? PAYLOAD : 0);
        memory[address] = low;
        memory[address + 1] = high;

        if (instr->addressing >> 4 >= 3)
        {
            update_register(instr->addressing >> 4, r[DST]);
        }
    }
    else if (addr_type == "REGISTER_DIRECT")
    {
        *r[SRC] = (WORD)((high << 8) | low);
    }
}

void Emulator::handle_call(Instruction *instr)
{
    std::string addr_type = addressing_codes[instr->addressing & 0x0F];
    WORD PAYLOAD = instr->load1 << 8 | instr->load2;
    BYTE DST = instr->registers & 0x0F;

    if (addr_type == "IMMEDIATE")
    {
        pc = PR_START + PAYLOAD; //absolute addressing
    }
    else if (addr_type == "MEMORY")
    {
        pc = memory[PAYLOAD];
    }
    else if (addr_type == "REGISTER_DIRECT")
    {
        pc = *r[DST];
    }
    else if (addr_type == "REGISTER_DIRECT_OFFSET")
    {
        pc = *r[DST] + PAYLOAD;
    }
    else // REGISTER_INDIRECT & REGISTER_INDIRECT_OFFSET // PC relative addressing
    {
        if (instr->addressing >> 4 < 3)
        {
            update_register(instr->addressing >> 4, r[DST]);
        }

        // adding payload if addressing is register indirect with offset; if there is no offset, no payload (regsiter_indirect)
        WORD address = *r[DST] + (addr_type == "REGISTER_INDIRECT_OFFSET" ? PAYLOAD : 0);
        pc = memory[address];
        pc |= memory[address + 1] << 8;

        if (instr->addressing >> 4 >= 3)
        {
            update_register(instr->addressing >> 4, r[DST]);
        }
    }
}

void Emulator::fill_z_flag(WORD value)
{
    if (value == 0)
    {
        psw |= Z_FLAG;
    }
    else
    {
        psw &= ~Z_FLAG;
    }
}

void Emulator::fill_n_flag(WORD value)
{
    if (value & 1 << 15)
    {
        psw |= N_FLAG;
    }
    else
    {
        psw &= ~N_FLAG;
    }
}

void Emulator::fill_c_flag(bool fill)
{
    if (fill)
    {
        psw |= C_FLAG;
    }
    else
    {
        psw &= ~C_FLAG;
    }
}

void Emulator::stack_push(WORD val)
{
    BYTE high = val >> 8;
    BYTE low = val & 0xFF;

    if (sp - 2 < SP_END)
        throw EmulatorError("Stack underflow when trying to push to stack. Stack: ", sp, val);
    memory[sp] = low;
    --sp; memory[sp] = high;
}

WORD Emulator::stack_pop()
{
    BYTE high, low;

    if (sp + 2 > SP_START)
        throw EmulatorError("Stack overflow when trying to pop from stack. Stack: ", sp, memory[sp]);

    high = memory[sp]; ++sp;
    low = memory[sp];

    return high << 8 | low;
}

void Emulator::update_register(BYTE up, REGISTER_PTR reg)
{
    WORD inc = up & 0x01 ? -2 : 2;
    *reg = *reg + inc;
}

void Emulator::write_mem_reg(BYTE byte, int offset)
{
    if (offset < 0xFF00)
        throw EmulatorError("Trying to write outside of memory mapped registers.", byte, offset);
    if (offset >= MEMORY_SIZE)
        throw EmulatorError("Trying to write outside of memory bounds.", ' ', offset);
    memory[offset] = byte;
}

BYTE Emulator::read_mem_reg(int offset)
{
    if (offset < 0xFF00)
        throw EmulatorError("Trying to read outside of memory mapped registers.", ' ', offset);
    if (offset >= MEMORY_SIZE)
        throw EmulatorError("Trying to read outside of memory bounds.", ' ', offset);
    return memory[offset];
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
