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
    void read_symbol_tables(std::ifstream& file);
    void read_sections(std::ifstream& file);
    void read_relocations(std::ifstream& file);

    std::vector<std::string> input_files;
    std::string output_file;
    bool is_hex;

    std::vector<std::vector<Section*>> sections;
    std::vector<Symbol_table*> symbol_tables;
    std::vector<Relocation_table*> relocations;

};

#endif