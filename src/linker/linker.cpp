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
    
    // populate help structures
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
    make_aggregate_sections(sections_by_file_values, sections_global);

    handle_relocations();
}

void Linker::generate_hex()
{
    std::ofstream current_file(output_file, std::ios_base::out);
    std::string line;
    int location_counter = 0;
    auto cnt = 0;
    
    current_file
            << std::internal
            << std::setfill('0');

    for (auto section: aggregate_section)
    {
        std::vector<std::string> data = section->get_section_data();
        
        for (auto d: data)
        {
            if (cnt == 0)
            {
                current_file << std::hex << std::setw(4) << location_counter  << ": ";
            }
            cnt++;
            int val = (int)std::bitset<16>(d).to_ulong();
            current_file << std::hex << std::setw(2) << val << std::dec << " ";
            if (cnt == 8)
            {
                cnt = 0;
                current_file << " \n";
            }

            location_counter++;
        }
    }

}

Linker::~Linker()
{
    // Symbol_table* aggregate_symbol_table;
    // Relocation_table* aggregate_relocation_table;
    for (int i = 0; i < sections.size(); i++)
    {
        for (int j = 0; j < sections[i].size(); j++)
        {
            delete sections[i][j];
        }
    }

    for (int i = 0; i < symbol_tables.size(); i++)
    {
        delete symbol_tables[i];
    }

    for (int i = 0; i < relocations.size(); i++)
    {
        delete relocations[i];
    }

    // for (int i = 0; i < aggregate_section.size(); i++)
    // {
    //     delete aggregate_section[i];
    // }

    // delete aggregate_symbol_table;
    // delete aggregate_relocation_table;
}

void Linker::read_files()
{
    int state = 0;
    int file_cnt = 0;
    
    for (auto file : input_files)
    {
        std::ifstream current_file(file, std::ios_base::in);
        std::string line;

        Symbol_table* current_symbol_table = nullptr;
        Section* current_section = nullptr;
        Relocation_table* current_relocation_table = nullptr;

        std::vector<Section*> file_sections;
        bool section_inserted = false;
        auto line_count = 0;

        while (std::getline(current_file, line))
        {
            line_count++;
            std::vector<std::string> splt = split(' ', line);
            
            if (splt.size() == 0)
            {
                state++;
                continue;
            }
           
            if (state == 0) // Reading symbol table
            {
                if (splt.size() < 6 || splt.size() > 6)
                {
                    throw ErrorHandler("Error! Wrong number of arguments(" \
                                        + std::to_string(splt.size()) + ") on line: " + std::to_string(line_count));
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
            sections.emplace_back(file_sections);
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
    new_entry->value = std::atoi(splt[4].c_str());
    new_entry->ord_num = std::atoi(splt[5].c_str());

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

/*
    Checks if symbol is defined (exists in some extern symbol table but not in that files relocation table)
*/

Symbol_table_entry* Linker::check_for_symbol_definition(std::vector<Symbol_table*>::iterator current_symbol_table,
                                                        std::string symbol)
{
    Symbol_table_entry* extern_entry = nullptr;
    Symbol_table_entry* double_entry = nullptr;
    auto extern_table = current_symbol_table + 1;
    auto double_extern_table = extern_table;
    auto symbol_found = false;

    for (double_extern_table = symbol_tables.begin(); double_extern_table != symbol_tables.end(); double_extern_table++)
    {
        if (double_extern_table == current_symbol_table) continue;

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
        else if (double_entry && symbol_found)
        {
            throw ErrorHandler("ERROR! Symbol " + double_entry->label + " definition found in two tables!");
        }
    }

    if (extern_entry && extern_table != symbol_tables.end())
    {
        (*extern_table)->remove_symbol_table_entry(extern_entry);
    }

    return extern_entry;
}

/*
    Iterate through all symbol tables, look through symbols; if symbol is not defined(it is in that files relocation table)
    then, look through all other symbol tables to find it's definition (it is in symbol table but not in relocation table).
    Add that symbol to aggregate symbol table and remove it from the table it was previously.
*/

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
                        throw ErrorHandler("Symbol " + (*symbol)->label + " definition not found!");
                    }

                    to_insert = new Symbol_table_entry(*extern_entry);                  
                    to_insert->defined = true;
                }
                else
                {
                    to_insert = new Symbol_table_entry(**symbol);
                    to_insert->defined = false;
                }

                aggregate_symbol_table->add_symbol_table_entry(to_insert);
            }
            else
            {
                if (in_table_entry->label != in_table_entry->section)
                {
                    Symbol_table_entry* extern_entry = nullptr;
                    extern_entry = check_for_symbol_definition(symbol_table, (*symbol)->label);

                    if (extern_entry == nullptr)
                    {
                        throw ErrorHandler("Symbol " + (*symbol)->label + " definition not found!");
                    }

                    Symbol_table_entry* fresh = new Symbol_table_entry(*extern_entry);
                    fresh->defined = true;
                    aggregate_symbol_table->remove_symbol_table_entry(*symbol);
                    aggregate_symbol_table->add_symbol_table_entry(fresh);
                }
                else
                {
                    (*symbol_table)->remove_symbol_table_entry(*symbol);
                }
            }
        }
    }
}

void Linker::make_aggregate_sections(std::vector<std::vector<std::pair<int, int>>>& sections_by_file_values,
                                    std::vector<std::pair<std::string, int>>& sections_global)
{
    
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

    aggregate_section = std::vector<Section*>();

    for (auto section_name: sections_global)
    {
        std::vector<Section*> _arg_section_vector;
        for (auto section_vector: sections)
        {
            for (auto section_ptr: section_vector)
            {
                if (section_ptr && section_ptr->get_section_name() == section_name.first)
                {
                    _arg_section_vector.emplace_back(section_ptr);
                }
            }
        }

        Section* unified_section = Section::create_aggregate_section(_arg_section_vector, section_name.first);
        aggregate_section.emplace_back(unified_section);
    }
}

void Linker::handle_relocations()
{
    for (auto relocation_table: relocations)
    {
        std::vector<Relocation_entry*> relocation_vector = relocation_table->get_relocation_table();

        for (auto relocation: relocation_vector)
        {
            int _roffset = relocation->offset;
            std::string _rsymbol = relocation->label;
            std::string _rsection = relocation->section;

            int _rvalue = aggregate_symbol_table->find_symbol(_rsymbol)->value;
            for (auto section: aggregate_section)
            {
                if (section->get_section_name() == _rsection)
                {
                    section->write_section_data(_roffset - section->get_section_offset(), std::bitset<16>(_rvalue).to_string());
                    break;
                }
            }
            // if (relocation->type == Relocation_type::R_ABSOLUTE)
            // {
            //     int _roffset = relocation->offset;
            //     std::string _rsymbol = relocation->label;
            //     std::string _rsection = relocation->section;

            //     int _rvalue = aggregate_symbol_table->find_symbol(_rsymbol)->value;
            //     for (auto section: aggregate_section)
            //     {
            //         if (section->get_section_name() == _rsection)
            //         {
            //             section->write_section_data(_roffset, std::bitset<16>(_rvalue).to_string());
            //             break;
            //         }
            //     }
            // }
            // else if (relocation->type == Relocation_type::R_PC_RELATIVE)
            // {

            // }
            // else
            // {
            //     throw ErrorHandler("ERROR! Wrong relocation type given.");
            // }
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

