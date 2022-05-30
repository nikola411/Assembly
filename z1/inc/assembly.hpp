#ifndef ASSEMBLY_HPP
#define ASSEMBLY_HPP

#include "instruction.hpp"
#include "directive.hpp"
#include "jump.hpp"
#include "symbol_table.hpp"
#include "section.hpp"

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

    ~Assembly();

private:
    void handle_jump(Jump*);
    void handle_directive(Directive*);
    void handle_instruction(Instruction*);

    bool does_section_exists(std::string) const;

    std::vector<Line*> lines;

    const std::type_info& instruction_type = typeid(Instruction);
    const std::type_info& jump_type = typeid(Jump);
    const std::type_info& directive_type = typeid(Directive);

    // variables used for code analyzing

    Symbol_table symbol_table;
    Section* current_section;
    std::vector<Section*> sections;
    std::vector<Forward_reference*> forward_refs;

    static std::string section_names[5];
};

#endif