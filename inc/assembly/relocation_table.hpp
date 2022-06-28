#ifndef RELOCATION_TABLE_HPP
#define RELOCATION_TABLE_HPP

#include <string>
#include <vector>

struct Relocation_entry;

class Relocation_table
{
public:
    Relocation_table();

    void add_new_relocation(Relocation_entry*);
    Relocation_entry* get_relocation(int);
    Relocation_entry* get_first_relocation();

    void print() const;
    std::string to_string() const;

    ~Relocation_table();

private:
    std::vector<Relocation_entry*> relocations;
};

#endif