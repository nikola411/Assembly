#include "linker.hpp"
#include "symbol_table.hpp"
#include "section.hpp"
#include "relocation_table.hpp"

#include <iostream>
#include <fstream>



Linker::Linker(std::vector<std::string> input_files, std::string output_file, bool is_hex) :
    input_files(input_files),
    output_file(output_file),
    is_hex(is_hex)
{
    
}

void Linker::link()
{
    read_files();
         
}

Linker::~Linker()
{

}

void Linker::read_files()
{
    int state = 0;

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
            std::cout << line << "                    ";
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

                Symbol_table_entry* new_entry = new Symbol_table_entry();
                new_entry->label = splt[0];
                new_entry->section = splt[1];
                new_entry->offset = std::atoi(splt[2].c_str());
                new_entry->binding = splt[3];
                new_entry->ord_num = std::atoi(splt[4].c_str());

                current_symbol_table->add_symbol_table_entry(new_entry);
            }
            else if (state == 1) // Reading sections
            {
                if (splt.size() == 1)
                {
                    if (current_section)
                    {
                        file_sections.emplace_back(current_section);
                    }

                    current_section = new Section(splt[0]);
                    continue;
                }

                // std::cout << "KURAC! \n";

                std::string data = "";
                for (int i = 1; i < splt.size(); i++)
                {
                    data += splt[i];
                }

                current_section->add_section_data(data);
            }
            else // Reading relocations
            {
                if (!current_relocation_table)
                {
                    file_sections.emplace_back(current_section);
                    sections.emplace_back(file_sections);
                    current_relocation_table = new Relocation_table();
                }

                Relocation_entry* current_relocation_entry = new Relocation_entry();
                current_relocation_entry->offset = std::atoi(splt[0].c_str());
                current_relocation_entry->section = splt[1];
                current_relocation_entry->type = (Relocation_type)std::atoi(splt[2].c_str());
                current_relocation_entry->ord_number = std::atoi(splt[3].c_str());

                current_relocation_table->add_new_relocation(current_relocation_entry);
            }
        }

        // current_symbol_table->print();
        // for (auto s: sections)
        // {
        //     for (auto k: s)
        //     {
        //         k->print();
        //     }
        // }
        symbol_tables.emplace_back(current_symbol_table);
        // current_relocation_table->print();
        relocations.emplace_back(current_relocation_table);
        current_file.close();
    }
}

void Linker::read_symbol_tables(std::ifstream& file)
{
    

    //current_symt->print();
}

void Linker::read_sections(std::ifstream& file)
{

}

void Linker::read_relocations(std::ifstream& file)
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

