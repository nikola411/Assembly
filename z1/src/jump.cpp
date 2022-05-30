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
    this->label_type = label_type;
}

void Jump::set_offset(std::string offset_value)
{
    this->offset_value = offset_value;
}

void Jump::print() const
{
    std::cout << jump_table[type] << " " << operand << 
    ( addressing_type != Addressing_type::SYMBOL_OFFSET && addressing_type != Addressing_type::LITERAL_OFFSET ? "\n" : " " + offset_value + " \n");
}

Jump::~Jump()
{

}