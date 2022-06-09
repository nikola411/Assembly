#ifndef _DIRECTIVE_H
#define _DIRECTIVE_H

#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "define.hpp"
#include "line.hpp"

class Directive : public Line
{
public:
    Directive();

    void set_type(Directive_type);
    Directive_type get_type() const;
    void set_operands(std::vector<std::string>);
    std::vector<std::string> get_operands() const;
    std::vector<Label_type> get_operand_types() const;
    void add_operand(std::string);
    void add_operand(std::string arg, Label_type type);

    void print() const;

    ~Directive();
    
private:
    Directive_type type;
    std::vector<std::string> operands;
    std::vector<Label_type> operand_types;

    static std::map<Directive_type, std::string> directive_map;
};

#endif