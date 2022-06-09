#include "define.hpp"
#include "section.hpp"

#include <iostream>
#include <bitset>

Section::Section(std::string name) : section_name(name), location_counter(0), offset(0)
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

void  Section::set_section_offset(int offset)
{
    this->offset = offset;
}

int  Section::get_section_offset() const
{
    return this->offset;
}

void Section::add_section_data(std::string data)
{
    if (data.size() == 0)
    {
        //ERROR
        std::cout << "ERROR! Cannot write 0 bytes to the section!\n";
    }

    Section_entry* entry = new Section_entry();
    entry->offset = location_counter;
    entry->size = data.size() / 8;
    section_data.emplace_back(entry);

    for (int i = 0; i < data.size(); i+=8)
    {
        std::string to_write = data.substr(i, 8);
        this->data.emplace_back(to_write);
    }
    
    location_counter += data.size() / 8;
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
    if (offset + size > this->data.size())
    {
        std::cout << "ERROR! Trying to read out of section bounds! \n";
    }

    std::string to_read = "";
    for (int i = 0; i < size; i++)
    {
        to_read += this->data[i + offset];
    }

    return to_read;
}

void Section::write_section_data(int start, std::string data)
{
    if (start + data.size()/8 >= this->data.size() || start < 0)
    {
        std::cout << "ERROR! Trying to write out of section bounds! \n";
    }

    int j = start;

    for (int i = 0; i < data.size(); i+=8)
    {
        this->data[j++] = data.substr(i, 8);
    }
}

void Section::print() const
{
    std::cout << section_name << "\n";
    int cnt = 0;
    for (int i = 0; i < section_data.size(); i++)
    {
        int start = section_data[i]->offset;
        int end = start + section_data[i]->size;

        std::cout << start << " ";

        for (int j = start; j < end; j++)
        {
            std::cout << data[j] << " ";
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