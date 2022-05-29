#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include "line.hpp"
#include "define.hpp"

#include <string>
#include <map>

class Instruction : public Line
{
public:
    Instruction(Instruction_type, std::string);
    Instruction(Instruction_type, std::string, std::string); //used when source is a register (in all instructions except ldr and str)

    void set_second_argument(Addressing_type, std::string, Label_type);
    void set_offset(std::string, Label_type);

    std::pair<Addressing_type, std::string> get_second_argument() const;
    std::string get_destination_register() const;
    std::string get_offset() const;

    void print() const;

    ~Instruction();

private:
    std::pair<Addressing_type, std::string> second_argument;
    Label_type second_argument_type;
    std::string destination_register;
    Instruction_type type;

    // When second arguemnt is a register and addresing type is memory with offset
    std::string offset;
    Label_type offset_type;

    // Used for 1-1 mapping of enums and strings
    static std::map<Instruction_type, std::string> instruction_map;
};

#endif