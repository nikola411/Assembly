#include "define.hpp"
#include "section.hpp"

#include <iostream>
#include <bitset>

Section::Section(std::string name) : section_name(name), location_counter{0}, offset{0}
{

}

Section::Section(const Section& orig)
{
    this->section_name = orig.section_name;
    this->location_counter = orig.location_counter;
    this->offset = orig.offset;

    for (auto byte: orig.data)
    {
        this->data.emplace_back(byte);
    }

    for (auto info: orig.section_data)
    {
        this->section_data.emplace_back(new Section_entry(*info));
    }
}

std::string Section::get_section_name() const
{
    return this->section_name;
}

int Section::get_section_location_counter() const
{
    return this->location_counter;
}

/*
    We need to update section offset and offset of every memory location in this section.
*/
void Section::set_section_offset(int offset)
{
    this->offset = offset;
}

int Section::get_section_offset() const
{
    return this->offset;
}

std::vector<std::string> Section::get_section_data() const
{
    return data;
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
    std::cout << "START: " << start << " size: "<< data.size() / 8 << " data_size " << this->data.size() << '\n';
    std::cout << "writing data: " << data << " \n";
    if (start + data.size()/8 > this->data.size() || start < 0)
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

        std::cout << start + offset<< " ";

        for (int j = start; j < end; j++)
        {
            std::cout << data[j] << " ";
        }

        std::cout << "\n";
    }
}

std::string Section::to_string() const
{
    std::string section_as_string = "";
    section_as_string += section_name;
    section_as_string += "\n";
    for (int i = 0; i < section_data.size(); i++)
    {
        int start = section_data[i]->offset;
        int end = start + section_data[i]->size;

        section_as_string += std::to_string(start + offset);
        section_as_string += " ";

        for (int j = start; j < end; j++)
        {
            section_as_string += data[j];
            section_as_string +=  " ";
        }

        section_as_string += "\n";
    }

    return section_as_string;
}

// Create one section out of multiple section parts (sections from different files with the same name)
// in order they are found in the vector sections
Section* Section::create_aggregate_section(std::vector<Section*> sections, std::string section_name)
{
    Section* aggregate = new Section(section_name);
    aggregate->offset = sections.front()->offset;
    auto last_chunk_offset = 0;

    for (auto section: sections)
    {
        for (auto byte: section->data)
        {
            aggregate->data.emplace_back(byte);
        }

        for (auto data: section->section_data)
        {
            Section_entry* section_entry_copy = new Section_entry();
            section_entry_copy->offset = data->offset;
            section_entry_copy->size = data->size;

            aggregate->section_data.emplace_back(section_entry_copy);
            aggregate->section_data.back()->offset = last_chunk_offset;
            last_chunk_offset += aggregate->section_data.back()->size;
        }
    }

    aggregate->location_counter = aggregate->data.size();

    return aggregate;
}

Section::~Section()
{
    for (int i = 0; i < section_data.size(); i++)
    {
        if (section_data[i])
        {
            delete section_data[i];
            section_data[i] = nullptr;
        }
    }
}