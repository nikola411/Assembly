#ifndef JUMP_HPP
#define JUMP_HPP

#include "define.hpp"
#include "line.hpp"

#include <string>
#include <map>

class Jump : public Line
{
public:
    Jump(Jump_type);
    Jump();
    
    void set_jump_type(Jump_type);
    void set_operand(std::string, Addressing_type, Label_type);
    void set_offset(std::string);

    void print() const;
    ~Jump();

private:
    std::string operand;
    Addressing_type addressing_type;
    Label_type label_type;

    Jump_type type;

    std::string offset_value;

    static std::map<Jump_type, std::string> jump_table;
};

#endif