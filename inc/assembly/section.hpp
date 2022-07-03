#ifndef SECTION_HPP
#define SECTION_HPP

#include <string>
#include <vector>

class Section
{
public:
    Section(std::string);

    void add_section_data(std::string);
    void add_section_data(int data);
    void inc_section_location_counter(int);

    void set_section_location_counter(int);
    void set_section_offset(int);

    std::string get_section_name() const;
    int get_section_location_counter() const;
    int get_section_offset() const;
    
    std::string read_section_data(int, int);
    void write_section_data(int, std::string);

    void print() const;
    std::string to_string() const;

    static Section* create_aggregate_section(std::vector<Section*> sections, std::string section_name);

    ~Section();

private:
    std::string section_name;
    int location_counter;
    int offset;

    std::vector<Section_entry*> section_data;
    std::vector<std::string> data;
};

#endif