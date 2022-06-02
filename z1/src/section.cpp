#include "define.hpp"
#include "section.hpp"

#include <iostream>
#include <bitset>

Section::Section(std::string name) : section_name(name), location_counter(0)
{

}

std::string Section::get_section_name() const
{
    return this->section_name;
}

int Section::get_section_location_counter() const
{
    return this->location_counter;
}

void Section::add_section_data(std::string data)
{
    Section_entry* entry = new Section_entry();
    entry->offset = location_counter;
    if (data.size() > 0)
    {
        for (int i = 0; i < data.size(); i++)
        {
            entry->data.emplace_back((char)data[i]);
        }
    }

    location_counter += data.size() / 8;
    section_data.emplace_back(entry);
}

void Section::add_section_data(int data)
{
    //section_data.emplace_back(data);
}

void Section::set_section_location_counter(int value)
{
    this->location_counter = value;
}

void Section::inc_section_location_counter(int inc)
{
    this->location_counter += inc;
}

std::string Section::read_section_data(int offset, int size)
{
    int i;
    Section_entry* to_find = nullptr;
    for (i = 0; i < section_data.size(); i++)
    {
        if (section_data[i]->offset == offset)
        {
            to_find = section_data[i];
            break;
        }
    }

    if (to_find == nullptr)
    {
        std::cout << "Wrong offset given for reading! \n";
    }

    std::string data = "";

    for (i = 0; i < to_find->data.size(); i++)
    {
        data += to_find->data[i];
    }

    return data;
}

void Section::write_section_data(int start, std::string data)
{
    int end = start + data.size();
    if (end > section_data.size())
    {
        std::cout << "ERROR! Writing out of sections bounds!\n";
    }
}

void Section::print() const
{
    std::cout << section_name << "\n";
    int cnt = 0;
    for (int i = 0; i < section_data.size(); i++)
    {
        std::cout << section_data[i]->offset << " ";
        for (int j = 0; j < section_data[i]->data.size(); j++)
        {
            std::cout << section_data[i]->data[j];
            cnt ++;
            if (cnt == 8)
            {
                cnt = 0;
                std::cout <<" ";
            }
        }
        std::cout << "\n";
    }
}

Section::~Section()
{
    for (int i = 0; i < section_data.size(); i++)
    {
        delete section_data[i];
    }

}