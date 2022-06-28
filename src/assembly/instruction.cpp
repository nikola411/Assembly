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

Instruction::Instruction()
{

}

Instruction::Instruction(Instruction_type type) :
            type(type),
            number_of_operands(0)
{

}

Instruction::Instruction(Instruction_type type, std::string destination_register) : 
            type(type),
            destination_register(destination_register),
            number_of_operands(1)
{
    
}

Instruction::Instruction(Instruction_type type, std::string r1, std::string r2) : 
            type(type),
            destination_register(r1),
            second_operand(r2),
            addressing_type(Addressing_type::ABSOLUTE),
            second_operand_type(Label_type::REGISTER),
            number_of_operands(2)
{
    
}

void Instruction::set_first_operand(std::string r1)
{
    this->destination_register = r1;
}

void Instruction::set_second_operand(std::string arg_val, Label_type arg_type)
{
    this->second_operand = arg_val;
    this->second_operand_type = arg_type;
}

void Instruction::set_offset(std::string offset_val, Label_type offset_type)
{
    this->offset = offset_val;
    this->offset_type = offset_type;
}

void Instruction::set_addressing_type(Addressing_type addr_type)
{
    this->addressing_type = addr_type;
}

void Instruction::set_number_of_operands(int arg_no)
{
    this->number_of_operands = arg_no;
}

void Instruction::set_instruction_type(Instruction_type type)
{
    this->type = type;
}


std::string Instruction::get_second_operand() const
{
    return this->second_operand;
}

Label_type Instruction::get_second_operand_type() const
{
    return this->second_operand_type;
}

std::string Instruction::get_first_operand() const
{
    return this->destination_register;
}

std::string Instruction::get_offset() const
{
    return this->offset;
}

Label_type Instruction::get_offset_type() const
{
    return this->offset_type;
}

int Instruction::get_number_of_operands() const
{
    return this->number_of_operands;
}

Instruction_type Instruction::get_type() const
{
    return this->type;
}

Addressing_type Instruction::get_addressing_type() const
{
    return this->addressing_type;
}

void Instruction::print() const
{
    std::cout << Instruction::instruction_map[this->type] << " "
    << this->destination_register  << " " << second_operand << "\n";
}

Instruction::~Instruction()
{

}