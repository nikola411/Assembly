#include "assembly.hpp"

#include "line.hpp"

#include <iostream>

Line* Assembly::get_current_line()
{
    return lines.back();
}

void Assembly::add_new_line(Line* line)
{
    lines.push_back(line);
    //current_line = *line;
}

void Assembly::print()
{
    std::cout << "Printing assembly lines! \n";
    for (int i = 0; i < lines.size(); i++)
    {
        lines[i]->print();
    }
}

void Assembly::clean()
{
    for (int i = 0; i < lines.size(); i++)
    {
        delete lines[i];
    }
}

