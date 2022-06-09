#include "relocation_table.hpp"
#include "define.hpp"

#include <iostream>

Relocation_table::Relocation_table()
{

}

void Relocation_table::add_new_relocation(Relocation_entry* entry)
{
    relocations.emplace_back(entry);
}

Relocation_entry* Relocation_table::get_relocation(int offset)
{
    if (offset > relocations.size())
    {
        //ERROR
        return nullptr;
    }

    return relocations[offset];
}

Relocation_entry* Relocation_table::get_first_relocation()
{
    if (relocations.size() > 1)
    {
        return relocations[0];
    }

    return nullptr;
}

void Relocation_table::print() const
{
    std::cout << "\n";
    for (int i = 0; i < relocations.size(); i++)
    {
        std::cout << relocations[i]->offset << " " << relocations[i]->section << " " << relocations[i]->type << " " << relocations[i]->ord_number<<"\n";
    }
    std::cout << "\n";
}

Relocation_table::~Relocation_table()
{
    for (int i = 0; i < relocations.size(); i++)
    {
        delete relocations[i];
    }
}