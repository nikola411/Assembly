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
    Directive()
    {
    }

    void set_type(Directive_type);
    Directive_type get_type() const;
    void set_arguments(std::vector<std::string>);
    std::vector<std::string> get_arguments() const;
    void add_argument(std::string);
    void add_argument(std::string arg, Label_type type);

    void print() const;

    ~Directive()
    {
    }
    
private:
    Directive_type type;
    std::vector<std::string> arguments;
    std::vector<Label_type> argument_types;

    static std::map<Directive_type, std::string> directive_map;
};

#endif