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


std::string Relocation_table::to_string() const
{
    std::string relocations_as_string = "";
    for (int i = 0; i < relocations.size(); i++)
    {
        relocations_as_string += std::to_string(relocations[i]->offset);
        relocations_as_string += " ";
        relocations_as_string += relocations[i]->section;
        relocations_as_string += " ";
        relocations_as_string += std::to_string(relocations[i]->type);
        relocations_as_string += " ";
        relocations_as_string += std::to_string(relocations[i]->ord_number);
        relocations_as_string += "\n";
    }

    return relocations_as_string;
}

void Relocation_table::update_sections_offsets(int base_section_offset, std::string base_section_name)
{
    for (auto relocation: relocations)
    {
        if (relocation->section == base_section_name)
        {
            relocation->offset += base_section_offset;
        }
    }
}

Relocation_table::~Relocation_table()
{
    for (int i = 0; i < relocations.size(); i++)
    {
        delete relocations[i];
    }
}