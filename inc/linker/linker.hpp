#ifndef LINKER_HPP
#define LINKER_HPP

#include "symbol_table.hpp"
#include "relocation_table.hpp"

#include <vector>
#include <string>

class Section;
struct Relocation_entry;
struct Symbol_table_entry;

struct File_handle;

class Linker
{
public:
    Linker(std::vector<std::string>, std::string, bool);

    void link();

    ~Linker();

private:
    void debug_output() const;
    void update_sections_offsets();
    void update_relocations_offsets();
    void merge_sections();
    void merge_symbol_tables();
    void resolve_relocations();
    void handle_relocations();
    void write_to_output();

    void read_files();
    std::vector<std::string> split(char del, std::string agg) const;

    std::vector<File_handle*> files;

    std::vector<Section*> sections;
    Symbol_table symbol_table;
    std::vector<Relocation_entry> relocation_table;

    std::vector<std::string> input_files;
    std::string output_file;
    bool is_hex;
};

#endif