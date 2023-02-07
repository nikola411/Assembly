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

const int SECTION_MAX_OFFSET = 65535;

enum FILE_READ_STATE
{
    TABLE = 1, SECTIONS, RELOCATION
};

struct File_handle
{
    Symbol_table file_symbol_table;
    std::vector<Section*> file_sections;
    Relocation_table file_relocations;

    ~File_handle()
    {
        for (auto sec: file_sections)
        {
            delete sec;
        }
    }
};

Linker::Linker(std::vector<std::string> input_files, std::string output_file, bool is_hex) :
    input_files(input_files),
    output_file(output_file),
    is_hex(is_hex)
{
    relocation_table = new Relocation_table();
    symbol_table = new Symbol_table();
    read_files(); 
}

void Linker::link()
{
    //--------------------update sections offsets--------------------
    update_sections_offsets();
    //--------------------update offsets in relocations--------------------
    update_relocations_offsets();
    //--------------------make unified symbol table--------------------
    merge_sections();
    //--------------------write relocation data to sections--------------------

    //--------------------write sections to output--------------------
}

void Linker::generate_hex()
{

}

Linker::~Linker()
{
    delete symbol_table;
    delete relocation_table;

    for (auto section: sections)
    {
        delete section;
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
        }
    }
}

void Linker::update_relocations_offsets()
{
    for (auto file_handle: files)
    {
        auto relocation_vector = file_handle->file_relocations.get_relocation_table();
        
        for (auto relocation: relocation_vector)
        {
            if (!relocation) continue;

            std::string relocation_section = relocation->section;

            for (auto section: file_handle->file_sections)
            {
                if (section != nullptr && section->get_section_name() == relocation_section)
                {
                    relocation->offset += section->get_section_offset();
                }
            }
        }
    }
}

void Linker::merge_sections()
{
    std::map<std::string, std::vector<Section*>> sections_map = {};
    std::vector<std::string> sections_order = {};

    for (auto file_handle: files)
    {
        for (auto section: file_handle->file_sections)
        {
            std::string section_name = section->get_section_name();
            std::cout << section_name << " \n";
            if (sections_map[section_name].size() == 0)
            {
                sections_order.emplace_back(section_name);
            }

            sections_map[section_name].emplace_back(section);
        }
    }
    
    for (auto section_name: sections_order)
    {
        sections.emplace_back(Section::create_aggregate_section(sections_map[section_name], section_name));
    }
}

void Linker::merge_relocations()
{

}

void Linker::read_files()
{
    FILE_READ_STATE state = TABLE;
    int file_cnt = 0;
    int line_cnt = 0;
    
    File_handle* current_file;
    for (auto file : input_files)
    {
        std::ifstream input_file(file, std::ios_base::in);
        std::string line;

        current_file = new File_handle();
        Section* current_section = nullptr;
        Relocation_entry* current_relocation = nullptr;

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
                        current_relocation = nullptr;
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

                    current_relocation = new Relocation_entry();
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

        if (current_section)
        {
            current_file->file_sections.emplace_back(current_section);
            current_section = nullptr;
        }

        if (current_relocation)
        {
            current_file->file_relocations.add_new_relocation(current_relocation);
            current_relocation = nullptr;
        }

        files.emplace_back(current_file);
        current_file = nullptr;
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

