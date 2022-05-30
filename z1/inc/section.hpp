#ifndef SECTION_HPP
#define SECTION_HPP

#include <string>
#include <vector>

class Section
{
public:
    Section(std::string);

    std::string get_section_name() const;
    int get_section_location_counter() const;

    void add_section_data(std::string);

    void set_section_location_counter(int);
    void inc_section_location_counter(int);

    void print() const;

    ~Section();

private:
    std::string section_name;
    int location_counter;

    std::vector<std::string> section_data;
};

#endif