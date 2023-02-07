#include "linker.hpp"
#include "symbol_table.hpp"
#include "section.hpp"
#include "relocation_table.hpp"
#include "error_handler.hpp"

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <algorithm>
#include <bitset>
#include <iomanip>

void BREAK()
{
    int a;
    std::cin >> a;
}

const int SECTION_MAX_OFFSET = 65535;

enum FILE_READ_STATE
{
    TABLE = 1, SECTIONS, RELOCATION
};

struct File_handle
{
    File_handle()
    {

    }

    Symbol_table file_symbol_table;
    std::vector<Section*> file_sections;
    Relocation_table file_relocations;
};

Linker::Linker(std::vector<std::string> input_files, std::string output_file, bool is_hex) :
    input_files(input_files),
    output_file(output_file),
    is_hex(is_hex)
{
    read_files();
}

void Linker::link()
{
    std::cout << "DEBUG INFO: Loaded files \n";
    for (auto file: files)
    {
        file->file_symbol_table.print();
        std::cout << "\n";
        for (auto sec: file->file_sections)
        {
            sec->print();
        }
        
        file->file_relocations.print();
    }
    
    std::cout << "DEBUG INFO END. \n";

    //--------------------update sections offsets--------------------
    update_sections_offsets();

    for(auto section: sections)
    {
        section->print();
    }
    //--------------------update offsets in relocations--------------------

    //--------------------make unified symbol table--------------------

    //--------------------write relocation data to sections--------------------

    //--------------------write sections to output--------------------
}

void Linker::generate_hex()
{

}

Linker::~Linker()
{
    for (int i = 0; i < files.size(); i++)
    {
        delete files[i];
    }

    delete symbol_table;
    delete relocation_table;

    for (int i = 0; i < sections.size(); i++)
    {
        delete sections[i];
    }

}

void Linker::update_sections_offsets()
{
    int offset = 0;

    for (auto file_handle: files)
    {
        for (auto section: file_handle->file_sections)
        {
            if (!section) continue;

            section->set_section_offset(offset);
            offset += section->get_section_location_counter();
            sections.emplace_back(section);
        }
    }
}

void Linker::read_files()
{
    FILE_READ_STATE state = TABLE;
    int file_cnt = 0;
    int line_cnt = 0;
    
    
    for (auto file : input_files)
    {
        File_handle* current_file = new File_handle();
        std::ifstream input_file(file, std::ios_base::in);
        std::string line;
        Section* current_section = nullptr;

        while (std::getline(input_file, line))
        {
            line_cnt++;

            switch (state)
            {
                case TABLE:
                {
                    
                    if (line == "")
                    {
                        state = SECTIONS;
                        continue;
                    }
                    
                    std::vector<std::string> attributes = split(' ', line);
                    Symbol_table_entry* current_entry = new Symbol_table_entry();
                    current_entry->label = attributes[0];
                    current_entry->section = attributes[1];
                    current_entry->offset = atoi(attributes[2].c_str());
                    current_entry->binding = attributes[3];
                    current_entry->ord_num = atoi(attributes[4].c_str());

                    current_file->file_symbol_table.add_symbol_table_entry(current_entry);
                    break;
                }
                case SECTIONS:
                {
                    if (line == "")
                    {
                        state = RELOCATION;
                        continue;
                    }

                    if (current_section == nullptr)
                    {
                        std::vector<std::string> data = split(' ', line);
                        if (data.size() != 1)
                        {
                            throw ErrorHandler("Trying to create a section, but name is not given in the current line number: " + line_cnt);
                        }

                        if (data[0] == "UNDEFINED") break;
                        current_section = new Section(data[0]);
                        break;
                    }
                    
                    std::vector<std::string> data = split(' ', line);

                    if (data.size() == 1)
                    {
                        if (data[0] == "UNDEFINED") break;
                        current_file->file_sections.emplace_back(current_section);
                        current_section = new Section(data[0]);
                        break;
                    }

                    std::string row = "";

                    for (int i = 1; i < data.size(); i++)
                    {
                        row += data[i];
                    }

                    current_section->add_section_data(row);
                    
                    break;
                }
                case RELOCATION:
                {
                    if (current_section)
                    {
                        current_file->file_sections.emplace_back(current_section);
                        current_section = nullptr;
                    }

                    if (line == "") break;

                    Relocation_entry* current_relocation = new Relocation_entry();
                    std::vector<std::string> data = split(' ', line);

                    current_relocation->offset = atoi(data[0].c_str());
                    current_relocation->section = data[1];
                    current_relocation->type = (Relocation_type)atoi(data[2].c_str());
                    current_relocation->ord_number = atoi(data[3].c_str());
                    current_relocation->label = data[4];

                    current_file->file_relocations.add_new_relocation(current_relocation);
                    break;
                }
                default: break;
            }
        }

        files.emplace_back(current_file);
        state = TABLE;
    }
}

std::vector<std::string> Linker::split(char del, std::string agg) const
{
    int last_del = 0;
    std::string ins = "";
    std::vector<std::string> ret;
    
    for (int i = 0; i < agg.size(); i++)
    {
        if (agg[i] != del)
        {
            if (last_del)
            {
                last_del = 0;
                ins = "";
            }
            
            ins += agg[i];
        }
        else
        {
            if (!last_del)
            {
                ret.emplace_back(ins);
                last_del = 1;
            }
        }
    }

    if (ins != "" && !last_del)
    {
        ret.emplace_back(ins);
    }
    
    return ret;
}

