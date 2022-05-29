#include "instruction.hpp"

#include <iostream>

std::map<Instruction_type, std::string> Instruction::instruction_map = {
                    {Instruction_type::LDR,  "ldr"},
                    {Instruction_type::STR,  "str"},
                    {Instruction_type::SHL,  "shl"},
                    {Instruction_type::SHR,  "shr"},
                    {Instruction_type::TEST, "test"},
                    {Instruction_type::XOR,  "xor"},
                    {Instruction_type::OR,   "or"},
                    {Instruction_type::AND,  "and"},
                    {Instruction_type::CMP,  "cmp"},
                    {Instruction_type::DIV,  "div"},
                    {Instruction_type::MUL,  "mul"},
                    {Instruction_type::SUB,  "sub"},
                    {Instruction_type::ADD,  "add"},
                    {Instruction_type::XCHG, "xchg"},
                    };

Instruction::Instruction(Instruction_type type, std::string destination_register) : 
            type(type),
            destination_register(destination_register)
{
    
}

Instruction::Instruction(Instruction_type type, std::string r1, std::string r2) : 
            type(type),
            destination_register(r1)
{
    this->second_argument = std::pair<Addressing_type, std::string>(Addressing_type::ABSOLUTE, r2);
    this->second_argument_type = Label_type::REGISTER;
}

void Instruction::set_second_argument(Addressing_type arg_addr, std::string arg_val, Label_type arg_type)
{
    this->second_argument = std::pair<Addressing_type, std::string>(arg_addr, arg_val);
    this->second_argument_type = arg_type;
}

void Instruction::set_offset(std::string offset_val, Label_type offset_type)
{
    this->offset = offset_val;
    this->offset_type = offset_type;
}

std::pair<Addressing_type, std::string> Instruction::get_second_argument() const
{
    return this->second_argument;
}

std::string Instruction::get_destination_register() const
{
    return this->destination_register;
}

std::string Instruction::get_offset() const
{
    return this->offset;
}

void Instruction::print() const
{
    std::cout << Instruction::instruction_map[this->type] << " "
    << this->destination_register  << " " << second_argument.second << "\n";
}

Instruction::~Instruction()
{

}