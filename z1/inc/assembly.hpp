#ifndef ASSEMBLY_HPP
#define ASSEMBLY_HPP

#include "instruction.hpp"
#include "directive.hpp"
#include "jump.hpp"

#include <vector>
#include <typeinfo>

class Line;

class Assembly
{
public:
    Assembly();
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

    const std::type_info& instruction_type = typeid(Instruction);
    const std::type_info& jump_type = typeid(Jump);
    const std::type_info& directive_type = typeid(Directive);

};

#endif