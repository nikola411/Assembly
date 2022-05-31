#include "directive.hpp"

std::map<Directive_type, std::string> Directive::directive_map = {
                    {Directive_type::GLOBAL , ".global"},
                    {Directive_type::EXTERN , ".extern"},
                    {Directive_type::WORD , ".word"},
                    {Directive_type::SECTION , ".section"},
                    {Directive_type::SKIP , ".skip"},
                    {Directive_type::END , ".end"}
                };

void Directive::set_type(Directive_type type)
{
    this->type = type;
}

Directive_type Directive::get_type() const
{
    return this->type;
}

void Directive::set_operands(std::vector<std::string> arg)
{
    this->operands = arg;
}

std::vector<Label_type> Directive::get_operand_types() const
{
    return this->operand_types;
}

std::vector<std::string> Directive::get_operands() const
{
    return this->operands;
}

void Directive::add_operand(std::string arg)
{
    this->operands.emplace_back(arg);
}

void Directive::add_operand(std::string arg, Label_type type)
{
    this->operands.emplace_back(arg);
    this->operand_types.emplace_back(type);
}

void Directive::print() const
{
    std::cout << Directive::directive_map[type] << " ";
    for (int i = 0; i < operands.size(); i++)
    {
        std::cout << operands[i] << " ";
    }
    std::cout << "\n";
}