#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include "line.hpp"
#include "define.hpp"

#include <string>
#include <map>

class Instruction : public Line
{
public:
    Instruction();
    Instruction(Instruction_type);
    Instruction(Instruction_type, std::string);
    Instruction(Instruction_type, std::string, std::string); //used when source is a register (in all instructions except ldr and str)

    void set_first_operand(std::string);
    void set_second_operand(std::string, Label_type);
    void set_offset(std::string, Label_type);
    void set_addressing_type(Addressing_type);
    void set_number_of_operands(int);
    void set_instruction_type(Instruction_type);

    std::string get_second_operand() const;
    Label_type get_second_operand_type() const;
    std::string get_first_operand() const;
    std::string get_offset() const;
    int get_number_of_operands() const;
    Instruction_type get_type() const;
    Addressing_type get_addressing_type() const;

    void print() const;

    ~Instruction();

private:
    std::string second_operand;
    Label_type second_operand_type;
    std::string destination_register;
    Instruction_type type;
    Addressing_type addressing_type;

    // When second arguemnt is a register and addresing type is memory with offset
    std::string offset;
    Label_type offset_type;

    int number_of_operands;

    // Used for 1-1 mapping of enums and strings
    static std::map<Instruction_type, std::string> instruction_map;
};

#endif