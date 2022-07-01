#ifndef LINKER_HPP
#define LINKER_HPP

#include <vector>
#include <string>

class Section;
class Symbol_table;
class Relocation_table;
struct Relocation_entry;

class Linker
{
public:
    Linker(std::vector<std::string>, std::string, bool);

    void link();
    std::vector<std::string> split(char s, std::string delimeter) const;

    ~Linker();
    
private:
    void read_files();
    void read_symbol_table_entry(Symbol_table*, std::vector<std::string>);
    void read_section_data(Section*, std::vector<std::string>);
    void read_relocation_table_entry(Relocation_table*, std::vector<std::string>);

    void link_sections();

    std::vector<std::string> input_files;
    std::string output_file;
    bool is_hex;

    std::vector<std::vector<Section*>> sections;
    std::vector<Symbol_table*> symbol_tables;
    std::vector<Relocation_table*> relocations;

    Section* aggregate_section;
    Symbol_table* aggregate_symbol_table;
    Relocation_table* aggregate_relocation_table;

};

#endif