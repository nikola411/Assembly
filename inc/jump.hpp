#ifndef JUMP_HPP
#define JUMP_HPP

#include "define.hpp"
#include "line.hpp"

#include <string>
#include <map>

class Jump : public Line
{
public:
    Jump();
    Jump(Jump_type);
    
    void set_jump_type(Jump_type);
    void set_operand(std::string, Addressing_type, Label_type);
    void set_offset(std::string);
    void set_offset_type(Label_type);

    Jump_type get_type() const;
    std::string get_operand_value() const;
    Addressing_type get_addressing_type() const;
    Label_type get_operand_type() const;
    std::string get_offset_value() const;

    void print() const;
    ~Jump();

private:
    std::string operand;
    Label_type operand_type;
    Addressing_type addressing_type;
    Jump_type type;
    std::string offset_value;
    Label_type offset_type;

    static std::map<Jump_type, std::string> jump_table;
};

#endif