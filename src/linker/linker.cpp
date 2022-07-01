#include "linker.hpp"
#include "symbol_table.hpp"
#include "section.hpp"
#include "relocation_table.hpp"

#include <iostream>
#include <fstream>
#include <unordered_map>

const int SECTION_MAX_OFFSET = 65535;

Linker::Linker(std::vector<std::string> input_files, std::string output_file, bool is_hex) :
    input_files(input_files),
    output_file(output_file),
    is_hex(is_hex)
{
    
}

void Linker::link()
{
    read_files();
    // Prepare structures for section linking
    /*  Mapping files with their sections; mapping section names with their offset, location_counter pairs
        std::vector<
            std::map<
                std::string section_name,
                std::pair<
                    int section_offset,
                    int section_lc
        >>>
    */
    std::vector<std::unordered_map<std::string, std::pair<int, int>>> sections_by_file;
    /* Mapping sections with their base offsets that are later used for symbol table and relocation table update
        std::map<
            std::string section_name,
            int section_base_offset
        >
    */
    std::unordered_map<std::string, int> sections_map;

    // std::vector<std::pair<std::string, int>> sections_mp;

    std::cout << sections.size() <<"\n";
    
    for (int i = 0; i < sections.size(); i++)
    {
        std::unordered_map<std::string, std::pair<int, int>> file_sections_map;

        for (int j = 0; j < sections[i].size(); j++)
        {
            Section* current_section = sections[i][j];
            std::cout << current_section->get_section_name() << " \n";

            auto section_pair = std::make_pair(current_section->get_section_offset(), current_section->get_section_location_counter());
            auto section_name = current_section->get_section_name();

            file_sections_map.emplace(section_name, section_pair);

            if (sections_map.find(section_name) == sections_map.end())
            {
                sections_map[section_name] = 0;
            }

        }

        sections_by_file.emplace_back(file_sections_map);
    }

    int current_offset = 0;
    // Update sections offsets by linking them in order
    for (auto section: sections_map)
    {
        for (auto& file: sections_by_file)
        {
            std::string section_name = section.first;
            if (file.find(section_name) != file.end())
            {
                file[section_name].first = current_offset;
                current_offset += file[section_name].second;
            }
        }
    }

    // Update base section offsets
    for (auto& section: sections_map)
    {
        section.second = SECTION_MAX_OFFSET;
        std::string section_name = section.first;

        for (int i = 0; i < sections_by_file.size(); i++)
        {
            if (sections_by_file[i].find(section_name) != sections_by_file[i].end() &&
                sections_by_file[i][section_name].first < section.second)
            {
                section.second = sections_by_file[i][section_name].first;
            }
        }
    }

    // Update symbol offsets in symbol tables
    for (int i = 0; i < sections_by_file.size(); i++)
    {
        for (auto section: sections_by_file[i])
        {
            std::string section_name = section.first;
            int section_base_offset = sections_map[section_name];

            symbol_tables[i]->update_sections_offsets(section_base_offset, section_name);
        }        
    }

    for (auto section:sections_map)
    {
        std:: cout << section.first << " \n";
    }

    for (auto table: symbol_tables)
    {
        table->print();
    }
 
    // // Update relocations offsets
    for (int i = 0; i < sections_by_file.size(); i++)
    {
        for (auto section: sections_by_file[i])
        {
            std::string section_name = section.first;
            int section_base_offset = section.second.first;

            relocations[i]->update_sections_offsets(section_base_offset, section_name);
        }
    }

    for (auto relocation : relocations)
    {
        relocation->print();
    }

    link_sections();    
}

Linker::~Linker()
{

}

void Linker::read_files()
{
    int state = 0;
    int file_cnt = 0;
    
    for (auto file : input_files)
    {
        std::cout << file << "\n";
        std::ifstream current_file("tests/" + file, std::ios_base::in);
        std::string line;

        Symbol_table* current_symbol_table = nullptr;
        Section* current_section = nullptr;
        Relocation_table* current_relocation_table = nullptr;

        std::vector<Section*> file_sections;

        while (std::getline(current_file, line))
        {
            std::vector<std::string> splt = split(' ', line);
            
            if (splt.size() == 0)
            {
                state++;
                continue;
            }
           
            if (state == 0) // Reading symbol table
            {
                if (splt.size() < 5)
                {
                    std::cout << "ERROR! Line word count is : " << splt.size() << "\n";
                }

                if (!current_symbol_table)
                {
                    current_symbol_table = new Symbol_table();
                }

                read_symbol_table_entry(current_symbol_table, splt);
            }
            else if (state == 1) // Reading sections
            {
                if (splt.size() == 1)
                {
                    int offset = 0;

                    if (current_section)
                    {
                        offset = current_section->get_section_offset() +
                                    current_section->get_section_location_counter();
                        file_sections.emplace_back(current_section);
                    }

                    current_section = new Section(splt[0]);
                    current_section->set_section_offset(offset);
                    continue;
                }

                read_section_data(current_section, splt);
            }
            else // Reading relocations
            {  
                if (!current_relocation_table)
                {
                    file_sections.emplace_back(current_section);
                    sections.emplace_back(file_sections);
                    current_relocation_table = new Relocation_table();
                }
                 
                read_relocation_table_entry(current_relocation_table, splt);
            }
        }

        symbol_tables.emplace_back(current_symbol_table);
        relocations.emplace_back(current_relocation_table);
        current_file.close();
        state = 0;
    }
}

void Linker::read_symbol_table_entry(Symbol_table* current_symbol_table, std::vector<std::string> splt)
{
    Symbol_table_entry* new_entry = new Symbol_table_entry();
    new_entry->label = splt[0];
    new_entry->section = splt[1];
    new_entry->offset = std::atoi(splt[2].c_str());
    new_entry->binding = splt[3];
    new_entry->ord_num = std::atoi(splt[4].c_str());

    current_symbol_table->add_symbol_table_entry(new_entry);
}

void Linker::read_section_data(Section* current_section, std::vector<std::string> splt)
{
    std::string data = "";
    for (int i = 1; i < splt.size(); i++)
    {
        data += splt[i];
    }

    current_section->add_section_data(data);
}

void Linker::read_relocation_table_entry(Relocation_table* current_relocation_table, std::vector<std::string> splt)
{
    Relocation_entry* current_relocation_entry = new Relocation_entry();
                 
    current_relocation_entry->offset = std::atoi(splt[0].c_str());
    current_relocation_entry->section = splt[1];
    current_relocation_entry->type = (Relocation_type)std::atoi(splt[2].c_str());
    current_relocation_entry->ord_number = std::atoi(splt[3].c_str());

    current_relocation_table->add_new_relocation(current_relocation_entry);
}

void Linker::link_sections()
{

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

