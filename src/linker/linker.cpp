#include "linker.hpp"
#include "symbol_table.hpp"
#include "section.hpp"
#include "relocation_table.hpp"
#include "error_handler.hpp"
#include "define.hpp"

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
    std::vector<Relocation_entry> file_relocations;

    File_handle() : file_relocations{}, file_sections{}
    {
        
    }
};

Linker::Linker(std::vector<std::string> input_files, std::string output_file, bool is_hex) :
    input_files(input_files),
    output_file(output_file),
    is_hex(is_hex)
{
    read_files(); 
}

void Linker:: debug_output() const
{
    for (auto file: files)
    {
        file->file_symbol_table.print();
        std::cout << "\n";

        for (auto sec: file->file_sections)
        {
            sec->print();
        }

        std::cout << "\n";

        for (auto rel: file->file_relocations)
        {
            std::cout << rel.label << " " << rel.offset << " " << rel.section << " " << rel.type << " " << rel.ord_number << " \n";
        }
        std::cout << "\n";std::cout << "\n";
    }
}

void Linker::link()
{
    merge_sections();
    update_sections_offsets();
    update_relocations_offsets();
    merge_symbol_tables();
    resolve_relocations();
    write_to_output();
}

Linker::~Linker()
{
    for (auto& file: files)
    {
        for (auto& section: file->file_sections)
        {
            delete section;
        }

        delete file;
    }

    for (auto sec: sections)
    {
        delete sec;
    }
}

void Linker::update_sections_offsets()
{
    // update sections offsets in merged sections vector
    int offset = 0;

    for (auto section: sections)
    {
        section->set_section_offset(offset);
        offset += section->get_section_location_counter();
    }
}

void Linker::update_relocations_offsets()
{
    // since relocation offset is relative to the section start, we need to 
    // read new offsets from updated sections vector where sections are not merged
    // so we get right values for offsets in section
    for (auto file_handle: files)
    {       
        for (auto& relocation: file_handle->file_relocations)
        {
            std::string relocation_section = relocation.section;

            for (auto section: file_handle->file_sections)
            {
                if (section != nullptr && section->get_section_name() == relocation_section)
                {
                    relocation.offset += section->get_section_offset();
                }
            }
        }
    }
}

void Linker::merge_sections()
{
    std::map<std::string, std::vector<Section*>> sections_map = {};
    std::vector<std::string> sections_order = {};

    // populate help structures; sections_order is making sure sections are places later as they appear
    // sections_map is making sure that sections in different files with the same name are concatenated in order they appear
    for (auto file_handle: files)
    {
        for (auto section: file_handle->file_sections)
        {
            std::string section_name = section->get_section_name();
            
            if (sections_map[section_name].size() == 0)
            {
                sections_order.emplace_back(section_name);
            }

            sections_map[section_name].emplace_back(section);
        }
    }

    // update sections offset in file_handle array, this will later be used to update relocations
    for (auto section_name: sections_order)
    {
        int offset = sections_map[section_name].front()->get_section_offset();
        for (auto section: sections_map[section_name])
        {
            section->set_section_offset(offset);
            offset += section->get_section_location_counter();
        }
    }
    
    // create unified sections
    for (auto section_name: sections_order)
    {
        sections.emplace_back(Section::create_aggregate_section(sections_map[section_name], section_name));
    }
}

void Linker::merge_symbol_tables()
{
    // make one symbol table by taking every defined symbol in all symbol tables
    for (auto file_handle: files)
    {
        std::vector<Symbol_table_entry*> symbol_table_vector = file_handle->file_symbol_table.get_symbol_table_entry();
        
        for (auto symbol: symbol_table_vector)
        {
            if (symbol->defined && symbol_table.find_symbol(symbol->label) == nullptr) // this is a resolved local symbol
            {
                symbol_table.add_symbol_table_entry(new Symbol_table_entry(*symbol));
            }
        }
    }

    // update symbol values by reading them from sections
    for (auto section: sections)
    {
        for (auto symbol: symbol_table.get_symbol_table_entry())
        {
            if (symbol->section == section->get_section_name())
            {
                symbol->value = (int)std::bitset<16>(section->read_section_data(symbol->offset, 2)).to_ulong();
                symbol->offset += section->get_section_offset();
            }
        }
    }
}

void Linker::resolve_relocations()
{
    for (auto file_handle: files)
    {
        for (auto relocation: file_handle->file_relocations)
        {
            auto symbol = symbol_table.find_symbol(relocation.label);
            for (auto section: sections)
            {
                if (section->get_section_name() == relocation.section)
                { 
                    section->write_section_data(relocation.offset, std::bitset<16>(symbol->value).to_string());
                    break;
                }
            }
        }
    }
}

void Linker::write_to_output()
{
    if (is_hex)
    {
        std::ofstream current_file(output_file, std::ios_base::out);
        std::string line;
        int location_counter = 0;
        auto cnt = 0;
        
        current_file
            << std::internal
            << std::setfill('0');

        for (auto section: sections)
        {
            std::vector<std::string> data = section->get_section_data();
            
            for (auto byte: data)
            {
                if (cnt == 0)
                {
                    current_file << std::hex << std::setw(4) << location_counter  << ": ";
                }
                cnt++;
                int byte_value = (int)std::bitset<16>(byte).to_ulong();
                current_file << std::hex << std::setw(2) << byte_value << std::dec << " ";
                if (cnt == 8)
                {
                    cnt = 0;
                    current_file << " \n";
                }

                location_counter++;
            }
        }
    }
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
                    Symbol_table_entry current_entry;
                    current_entry.label = attributes[0];
                    current_entry.section = attributes[1];
                    current_entry.offset = atoi(attributes[2].c_str());
                    current_entry.binding = attributes[3];
                    current_entry.defined = atoi(attributes[4].c_str());
                    current_entry.value = atoi(attributes[5].c_str());
                    current_entry.ord_num = atoi(attributes[6].c_str());

                    current_file->file_symbol_table.add_symbol_table_entry(new Symbol_table_entry(current_entry));
                    break;
                }
                case SECTIONS:
                {
                    if (line == "")
                    {
                        if (current_section)
                        {
                            current_file->file_sections.emplace_back(current_section);
                            current_section = nullptr;
                        }
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
                        current_section = nullptr;
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
                    if (line == "") break;

                    Relocation_entry current_relocation;
                    std::vector<std::string> data = split(' ', line);

                    current_relocation.offset = atoi(data[0].c_str());
                    current_relocation.section = data[1];
                    current_relocation.type = (Relocation_type)atoi(data[2].c_str());
                    current_relocation.ord_number = atoi(data[3].c_str());
                    current_relocation.label = data[4];

                    current_file->file_relocations.emplace_back(current_relocation);
                    break;
                }
                default: break;
            }
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

