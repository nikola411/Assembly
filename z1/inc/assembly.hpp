#ifndef ASSEMBLY_HPP
#define ASSEMBLY_HPP

#include <vector>

class Line;

class Assembly
{
public:
    Assembly()
    {

    }

    Line* get_current_line();
    void add_new_line(Line*);
    void print();

    void clean();

    ~Assembly()
    {
        clean();
    }
    
private:
    std::vector<Line*> lines;

};

#endif