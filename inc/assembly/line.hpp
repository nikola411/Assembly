#ifndef LINE_HPP
#define LINE_HPP

#include "define.hpp"

class Line
{
public:
    Line() {};

    virtual void print() const {};
    //virtual void add_argument(std::string) {};
    virtual ~Line() {};
};

#endif