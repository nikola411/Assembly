#include "jump.hpp"

#include <iostream>

std::map<Jump_type, std::string> Jump::jump_table = {
    {Jump_type::JMP, "jmp"},
    {Jump_type::JEQ, "jeq"},
    {Jump_type::JNE, "jne"},
    {Jump_type::JGT, "jgt"}              
};

Jump::Jump()
{

}

Jump::Jump(Jump_type type) :
    type(type)
{

}

void Jump::set_jump_type(Jump_type type)
{
    this->type = type;
}

void Jump::set_operand(std::string value, Addressing_type addr_type, Label_type label_type)
{
    this->operand = value;
    this->addressing_type = addr_type;
    this->operand_type = label_type;
}

void Jump::set_offset(std::string offset_value)
{
    this->offset_value = offset_value;
}

void Jump::set_offset_type(Label_type offset_type)
{
    this->offset_type = offset_type;
}

Jump_type Jump::get_type() const
{
    return this->type;
}

std::string Jump::get_operand_value() const
{
    return this->operand;
}

Addressing_type Jump::get_addressing_type() const
{
    return this->addressing_type;
}

Label_type Jump::get_operand_type() const
{
    return this->operand_type;
}

std::string Jump::get_offset_value() const
{
    return this->offset_value;
}

Label_type Jump::get_offset_type() const
{
    return this->offset_type;
}

void Jump::print() const
{
    std::cout << jump_table[type] << " " << operand << 
    ( addressing_type != Addressing_type::SYMBOL_OFFSET && addressing_type != Addressing_type::LITERAL_OFFSET ? "\n" : " " + offset_value + " \n");
}

Jump::~Jump()
{

}