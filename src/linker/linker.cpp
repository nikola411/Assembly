#include "linker.hpp"
#include "symbol_table.hpp"
#include "section.hpp"
#include "relocation_table.hpp"

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <algorithm>

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

    std::vector<std::vector<std::string>> sections_by_file_names;
    std::vector<std::vector<std::pair<int, int>>> sections_by_file_values; // offset, location_counter
    std::unordered_map<std::string, bool> visited_sections_map; // section_name, visited
    std::vector<std::pair<std::string, int>> sections_global; // section_name, global_offset
    
    for (int i = 0; i < sections.size(); i++)
    {
        std::vector<std::string> _nfile_sections_vector;
        std::vector<std::pair<int, int>> _vfile_sections_vector;

        for (int j = 0; j < sections[i].size(); j++)
        {
            Section* current_section = sections[i][j];

            auto section_pair = std::make_pair(current_section->get_section_offset(), current_section->get_section_location_counter());
            auto section_name = current_section->get_section_name();

            _nfile_sections_vector.emplace_back(section_name);
            _vfile_sections_vector.emplace_back(section_pair);

            if (visited_sections_map.find(section_name) == visited_sections_map.end())
            {
                visited_sections_map[section_name] = 1;
                sections_global.emplace_back(std::make_pair(section_name, 0));
            }
        }

        sections_by_file_names.emplace_back(_nfile_sections_vector);
        sections_by_file_values.emplace_back(_vfile_sections_vector);
    }

    update_sections_offsets(sections_by_file_names, sections_by_file_values, sections_global);
    update_global_sections_offsets(sections_by_file_names, sections_by_file_values, sections_global);
    update_symbol_tables(sections_by_file_names, sections_by_file_values, sections_global);
    update_relocation_tables(sections_by_file_names, sections_by_file_values, sections_global);

    make_aggregate_symbol_table();
    // free up space (this needs to be done for every structure (relocations, sections) so we can do this altogether in one method)
    for (int file = 0; file < sections.size(); file++)
    {
        for (int section = 0; section < sections[file].size(); section++)
        {
            if (sections[file][section])
            {
                sections[file][section]->set_section_offset(sections_by_file_values[file][section].first);
            }
        }
    }

    std::vector<Section*> sec_vec;
    for (auto secv: sections)
    {
        for (auto sec: secv)
        {
            if (sec && sec->get_section_name() == ".text")
            {
                sec_vec.emplace_back(sec);
            }
        }
    }

    Section* agg_sec = Section::create_aggregate_section(sec_vec, ".text");
    // agg_sec->print();
    std::cout << agg_sec->get_section_offset() << " " << agg_sec->get_section_name() << "\n";
    std::cout << agg_sec->to_string();

    for (auto symt: symbol_tables)
    {
        delete symt;
    }

    // aggregate_symbol_table->print();

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
        bool section_inserted = false;

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
                    section_inserted = false;

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
                    current_section = nullptr;
                    
                    current_relocation_table = new Relocation_table();
                }
                 
                read_relocation_table_entry(current_relocation_table, splt);
            }
        }

        symbol_tables.emplace_back(current_symbol_table);
        relocations.emplace_back(current_relocation_table);
        if (current_section)
        {
            file_sections.emplace_back(current_section);
            sections.emplace_back(std::vector<Section*>());
            sections.back().emplace_back(current_section);
        }
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
    current_relocation_entry->label = splt[4];

    current_relocation_table->add_new_relocation(current_relocation_entry);
}

void Linker::update_sections_offsets(std::vector<std::vector<std::string>>& sections_by_file_names,
                                    std::vector<std::vector<std::pair<int, int>>>& sections_by_file_values,
                                    std::vector<std::pair<std::string, int>>& sections_global)
{
    int current_offset = 0;
    for (auto section: sections_global)
    {
        std::string section_name = section.first;
        for (int file = 0; file < sections_by_file_names.size(); file++)
        {
            auto iter = std::find(sections_by_file_names[file].begin(), sections_by_file_names[file].end(), section_name);
            if (iter != sections_by_file_names[file].end())
            {
                int i = iter - sections_by_file_names[file].begin();
                sections_by_file_values[file][i].first = current_offset;
                current_offset += sections_by_file_values[file][i].second;
            }
        }
    }
}

void Linker::update_global_sections_offsets(std::vector<std::vector<std::string>>& sections_by_file_names,
                                    std::vector<std::vector<std::pair<int, int>>>& sections_by_file_values,
                                    std::vector<std::pair<std::string, int>>& sections_global)
{
    for (auto& section: sections_global)
    {
        section.second = SECTION_MAX_OFFSET;
        std::string section_name = section.first;

        for (int i = 0; i < sections_by_file_values.size(); i++)
        {
            auto iter = std::find(sections_by_file_names[i].begin(), sections_by_file_names[i].end(), section_name);
            if (iter != sections_by_file_names[i].end())
            {
                int index = iter - sections_by_file_names[i].begin();
                if (sections_by_file_values[i][index].first < section.second)
                {
                    section.second = sections_by_file_values[i][index].first;
                }
            }
        }
    }
}

void Linker::update_symbol_tables(std::vector<std::vector<std::string>>& sections_by_file_names,
                                    std::vector<std::vector<std::pair<int, int>>>& sections_by_file_values,
                                    std::vector<std::pair<std::string, int>>& sections_global)
{
    for (int i = 0; i < sections_by_file_values.size(); i++)
    {
        // std::cout << sections_by_file_names[i].size() << " " << sections_by_file_values[i].size() << "\n";
        for (int j = 0; j < sections_by_file_values[i].size(); j++)
        {
            std::string section_name = sections_by_file_names[i][j];
            int index = 0;
            for (index; index < sections_global.size(); index++)
            {
                if (sections_global[index].first == section_name) break;
            }

            int section_base_offset = sections_global[index].second;
            int relative_section_offset = sections_by_file_values[i][j].first; // relative section offset is offset that is 
            // a sum of base_section_offset and location counter values of sections from other files with the same name
        
            symbol_tables[i]->update_sections_offsets(section_base_offset, relative_section_offset, section_name);
        }        
    }
}

void Linker::update_relocation_tables(std::vector<std::vector<std::string>>& sections_by_file_names,
                                        std::vector<std::vector<std::pair<int, int>>>& sections_by_file_values,
                                        std::vector<std::pair<std::string, int>>& sections_global)
{
    for (int i = 0; i < sections_by_file_values.size(); i++)
    {
        for (int j = 0; j < sections_by_file_values[i].size(); j++)
        {
            std::string section_name = sections_by_file_names[i][j];
            int section_base_offset = sections_by_file_values[i][j].first;
            
            if (relocations[i])
            {
                relocations[i]->update_sections_offsets(section_base_offset, section_name);
            }
        }
    }
}

Symbol_table_entry* Linker::check_for_symbol_definition(std::vector<Symbol_table*>::iterator current_symbol_table,
                                                        std::string symbol)
{
    Symbol_table_entry* extern_entry = nullptr;
    Symbol_table_entry* double_entry = nullptr;
    auto extern_table = current_symbol_table + 1;
    auto double_extern_table = extern_table;
    auto symbol_found = false;

    for (double_extern_table; double_extern_table != symbol_tables.end(); double_extern_table++)
    {
        double_entry = (*double_extern_table)->find_symbol(symbol);
       
        if (double_entry != nullptr && !symbol_found)
        {
            // check in that tables relocation table if symbol exists
            int index = double_extern_table - symbol_tables.begin();
            if (index < 0 || index >= relocations.size()) break; // ERROR CASE BUT SHOULD NOT HAPPEN

            Relocation_table* extern_relocation_table = relocations[index];
            Relocation_entry* extern_relocation = nullptr;

            if (extern_relocation_table)
            {
               extern_relocation = extern_relocation_table->find_symbol_relocation(symbol);
            }
            
            if (!extern_relocation)
            {
                extern_entry = double_entry;
                extern_table = double_extern_table;
                double_entry = nullptr;
                symbol_found = true;
            }
        }
        else if (double_entry != nullptr && symbol_found)
        {
            std::cout << "ERROR! Symbol definition found in two tables! \n";
            exit(-1);
        }
    }

    if (extern_entry && extern_table != symbol_tables.end())
    {
        (*extern_table)->remove_symbol_table_entry(extern_entry);
    }

    return extern_entry;
}

void Linker::make_aggregate_symbol_table()
{
    aggregate_symbol_table = new Symbol_table();
    // Make aggregate symbol table
    for (auto symbol_table = symbol_tables.begin(); symbol_table != symbol_tables.end(); symbol_table++)
    {
        std::vector<Symbol_table_entry*> symbol_vector = (*symbol_table)->get_symbol_table_entry();
        int file_index = symbol_table - symbol_tables.begin();

        for (auto symbol = symbol_vector.begin(); symbol != symbol_vector.end(); symbol++)
        {
            Symbol_table_entry* in_table_entry = nullptr;
            if (!(in_table_entry = aggregate_symbol_table->find_symbol((*symbol)->label)))
            {
                // fetch relocations for this file, go through relocations and look if this symbol is there
                std::vector<Relocation_entry*> file_relocations;
                if (relocations[file_index])
                {
                    file_relocations = relocations[file_index]->get_relocation_table();
                }
        
                bool is_relocated = false;

                for (auto relocation: file_relocations)
                {
                    if (is_relocated = 
                        (relocation->label == (*symbol)->label && relocation->type == Relocation_type::R_PC_RELATIVE))
                    {
                        break;
                    }
                }
                
                Symbol_table_entry* to_insert = nullptr;
               
                if (is_relocated)
                {
                    // Go through all other symbol tables and search for symbol definition
                    Symbol_table_entry* extern_entry = nullptr;
                    extern_entry = check_for_symbol_definition(symbol_table, (*symbol)->label);

                    if (extern_entry == nullptr)
                    {
                        std::cout << "ERROR! Symbol definition not found! \n";
                        exit(-1);
                    }

                    to_insert = new Symbol_table_entry(*extern_entry);                  
                }
                else
                {
                    to_insert = new Symbol_table_entry(**symbol);
                }

                aggregate_symbol_table->add_symbol_table_entry(to_insert);
            }
        }
    }
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

