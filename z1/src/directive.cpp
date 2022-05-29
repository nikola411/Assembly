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

void Directive::set_arguments(std::vector<std::string> arg)
{
    this->arguments = arg;
}

std::vector<std::string> Directive::get_arguments() const
{
    return this->arguments;
}

void Directive::add_argument(std::string arg)
{
    this->arguments.emplace_back(arg);
}

void Directive::add_argument(std::string arg, Label_type type)
{
    this->arguments.emplace_back(arg);
    this->argument_types.emplace_back(type);
}

void Directive::print() const
{
    std::cout << Directive::directive_map[type] << " ";
    for (int i = 0; i < arguments.size(); i++)
    {
        std::cout << arguments[i] << " ";
    }
    std::cout << "\n";
}