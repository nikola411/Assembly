#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include <vector>
#include <string>

#include "define.hpp"

class Symbol_table
{
public:
    Symbol_table();

    // General purpose methods
    void add_symbol_table_entry(Symbol_table_entry*);
    std::vector<Symbol_table_entry*> get_symbol_table_entry() const;
    Symbol_table_entry* find_symbol(std::string) const;
    int get_symbol_ord_number(std::string) const;

    void print() const;
    std::string to_string() const;

    // Methods linker needs
    void update_sections_offsets(int base_section_offset, int relative_section_offset, std::string base_section_name);
    void remove_symbol_table_entry(Symbol_table_entry*);

    ~Symbol_table();

    std::vector<Symbol_table_entry*> table;
    int location_counter;
    
};

#endif