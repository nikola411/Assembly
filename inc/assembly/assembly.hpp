#ifndef ASSEMBLY_HPP
#define ASSEMBLY_HPP

#include "instruction.hpp"
#include "directive.hpp"
#include "jump.hpp"
#include "symbol_table.hpp"
#include "section.hpp"
#include "relocation_table.hpp"

#include <vector>
#include <typeinfo>
#include <set>

class Relocation_table;
class Line;

class Assembly
{
public:
    Assembly(std::string);
    Line* get_current_line();
    void add_new_line(Line*);
    void finish_parsing();
    void print();

    ~Assembly();

private:
    void handle_jump(Jump*);
    void handle_directive(Directive*);
    void handle_instruction(Instruction*);

    bool does_section_exists(std::string) const;
    std::string get_instruction_value(Instruction*);
    std::string get_addressing_byte_value(Addressing_type addr_type, Label_type label_type) const;
    std::string get_symbol_value_or_relocate(std::string symbol);
    std::string get_payload_byte_value(std::string operand, Label_type operand_type, Addressing_type addr_type, std::string offset = "", Label_type offset_type = Label_type::REGISTER);
    void write_to_output();

    void backpatch();

    std::vector<Line*> lines;
    std::string output_file;

    const std::type_info& instruction_type = typeid(Instruction);
    const std::type_info& jump_type = typeid(Jump);
    const std::type_info& directive_type = typeid(Directive);

    // variables used for code analyzing

    Symbol_table symbol_table;
    Section* current_section;
    std::vector<Section*> sections;
    Relocation_table relocation_table;

    bool end_occured;

    static std::set<std::string> section_names;
    static std::map<Instruction_type, std::string> instruction_codes;
    static std::map<Jump_type, std::string> jump_codes;
    static std::map<std::string, std::string> register_codes;
    static std::map<std::string, std::string> addressing_codes;

};

#endif