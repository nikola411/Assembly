#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include <vector>
#include <string>

#include "define.hpp"

class Symbol_table
{
public:
    Symbol_table();

    void add_symbol_table_entry(Symbol_table_entry*);
    std::vector<Symbol_table_entry*> get_symbol_table_entry() const;
    Symbol_table_entry* find_symbol(std::string) const;
    int get_symbol_ord_number(std::string) const;

    void print() const;

    ~Symbol_table();

private:
    std::vector<Symbol_table_entry*> table;
    int location_counter;
    
};

#endif