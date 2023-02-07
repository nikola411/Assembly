#ifndef LINKER_HPP
#define LINKER_HPP

#include <vector>
#include <string>

class Section;
class Symbol_table;
class Relocation_table;
struct Relocation_entry;
struct Symbol_table_entry;

struct File_handle;

class Linker
{
public:
    Linker(std::vector<std::string>, std::string, bool);

    void link();
    void generate_hex();

    ~Linker();

private:

    void update_sections_offsets();
    void update_relocations_offsets();
    void merge_sections();
    void merge_relocations();

    void read_files();
    std::vector<std::string> split(char del, std::string agg) const;

    std::vector<File_handle*> files;

    std::vector<Section*> sections;
    Symbol_table* symbol_table;
    Relocation_table* relocation_table;

    std::vector<std::string> input_files;
    std::string output_file;
    bool is_hex;
};

#endif