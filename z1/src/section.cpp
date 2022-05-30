#include "section.hpp"

#include <iostream>

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
    this->section_data.emplace_back(data);
}

void Section::set_section_location_counter(int value)
{
    this->location_counter = value;
}

void Section::inc_section_location_counter(int inc)
{
    this->location_counter += inc;
}

void Section::print() const
{
    std::cout << section_name << "\n";
    for (int i = 0; i < section_data.size(); i++)
    {
        std::cout << section_data[i] << "\n";
    }
}

Section::~Section()
{

}