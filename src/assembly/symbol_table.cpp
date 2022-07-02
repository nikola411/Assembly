#include "symbol_table.hpp"

#include <iostream>
#include <iomanip>

Symbol_table::Symbol_table()
{

}

void Symbol_table::add_symbol_table_entry(Symbol_table_entry* entry)
{
    table.emplace_back(entry);
}

std::vector<Symbol_table_entry*> Symbol_table::get_symbol_table_entry() const
{
    return this->table;
}

Symbol_table_entry* Symbol_table::find_symbol(std::string label) const
{
    for (int i = 0; i < table.size(); i++)
    {
        if (table[i] -> label == label)
        {
            return table[i];
        }
    }

    return nullptr;
}

int Symbol_table::get_symbol_ord_number(std::string symbol) const
{
    for (int i = 0; i < table.size(); i++)
    {
        if (table[i]->label == symbol)
            return i;
    }

    return -1;
}

void Symbol_table::update_sections_offsets(int base_section_offset, std::string section_name)
{
    for (auto& symbol: table)
    {
        if (symbol->section == section_name)
        {
            symbol->offset += base_section_offset;
        }
    }
}

void Symbol_table::remove_symbol_table_entry(Symbol_table_entry* to_delete)
{
    auto iter = table.begin();
    for (iter; iter != table.end(); iter++)
    {
        if (*iter == to_delete)
        {
            break;
        }
    }

    if (iter != table.end())
    {
        table.erase(iter);
    }
}

void Symbol_table::print() const
{
    int w = 15;
    std::cout 
            << std::left
            << std::setw(w)
            << "Label" 
            << std::left
            << std::setw(w)
            << "Section"
            << std::left
            << std::setw(w)
            << "Offset"
            << std::left
            << std::setw(w)
            << "Binding"
            << std::left
            << std::setw(w)
            << "Ord num";

    std::cout <<"\n";

    for (int i = 0; i < table.size(); i++)
    {
        std::cout 
            << std::left
            << std::setw(w)
            << table[i] -> label 
            << std::left
            << std::setw(w)
            << table[i] -> section
            << std::left
            << std::setw(w)
            << table[i] -> offset
            << std::left
            << std::setw(w)
            << table[i] -> binding
            << std::left
            << std::setw(w)
            << i << "\n";
    }
}

std::string Symbol_table::to_string() const
{
    std::string table_as_string = "";
    for (int i = 0; i < table.size(); i++)
    {
        table_as_string += table[i]->label;
        table_as_string += "  ";
        table_as_string += table[i]->section;
        table_as_string += "  ";
        table_as_string += std::to_string(table[i]->offset);
        table_as_string += "  ";
        table_as_string += table[i]->binding;
        table_as_string += "  ";
        table_as_string += std::to_string(i);
        table_as_string += "\n";
    }

    return table_as_string;
}

Symbol_table::~Symbol_table()
{

}
