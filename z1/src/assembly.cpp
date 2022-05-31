#include "assembly.hpp"

#include <iostream>
#include <bitset>

std::string Assembly::section_names[5] = {".text", ".data", ".bss", ".rel", ".rodata"};
std::map<Instruction_type, std::string> Assembly::instruction_codes
{
    { IRET, "00100000" },
    { HALT, "00000000" },
    { RET, "01000000" },
    { INT, "00010000" },
    { CALL, "00110000" },
    { XCHG, "01100000" },
    { ADD, "01110000" },
    { SUB, "01110001" },
    { MUL, "01110010" },
    { DIV, "01110011" }, 
    { CMP, "01110100" },
    { NOT, "10000000" },
    { AND, "10000001" },
    { OR,  "10000010" },
    { XOR, "10000011" },
    { TEST, "10000100" },
    { SHL,  "10010000" },
    { SHR,  "10010001" },
    { STR, "10110000" },
    { LDR, "10100000" }
};

std::map<Jump_type, std::string> Assembly::jump_codes
{
    {JMP, "01010000"},
    {JEQ, "01010001"},
    {JNE, "01010010"},
    {JGT, "01010011"}
};

std::map<std::string, std::string> Assembly::register_codes
{
    {"r0", "0000"},
    {"r1", "0001"},
    {"r2", "0010"},
    {"r3", "0011"},
    {"r4", "0100"},
    {"r5", "0101"},
    {"r6", "0110"},
    {"sp", "0110"},
    {"r7", "0111"},
    {"pc", "0111"},
    {"psw", "1000"}
};

std::map<std::string, std::string> Assembly::addressing_codes
{
    {"IMMEDIATE", "0000"},
    {"REGISTER_DIRECT", "0001"},
    {"REGISTER_ADD", "0101"},
    {"REGISTER_INDIRECT", "0010"},
    {"REGISTER_OFFSET", "0011"},
    {"MEMORY", "0100"}
};

Assembly::Assembly()
{
  
}

Line* Assembly::get_current_line()
{
    return lines.back();
}

void Assembly::add_new_line(Line* line)
{
    lines.push_back(line);

    if (typeid(*lines.back()) == jump_type)
    {
        //handle_jump((Jump*)line);
    }
    else if (typeid(*lines.back()) == instruction_type)
    {
        //handle_instruction((Instruction*)line);
    }
    else if (typeid(*lines.back()) == directive_type)
    {
        handle_directive((Directive*)line);
    }
}

void Assembly::print()
{
    symbol_table.print();
    for (int i = 0; i < sections.size(); i++)
    {
        sections[i]->print();
    }
}

Assembly::~Assembly()
{
    for (int i = 0; i < lines.size(); i++)
    {
        delete lines[i];
    }

    for (int i = 0; i < sections.size(); i++)
    {
        delete sections[i];
    }

    for (int i = 0; i < forward_refs.size(); i++)
    {
        delete forward_refs[i];
    }
}

void Assembly::handle_jump(Jump* jump)
{

}

void Assembly::handle_directive(Directive* directive)
{
    switch (directive->get_type())
    {
        case Directive_type::SECTION:
        {
            std::string name = (directive->get_operands()) [0];

            if (!does_section_exists(name))
            {
                // ERROR
                std::cout << "Error! Wrong section name! \n";
                break;
            }

            if (current_section == nullptr || name != current_section->get_section_name()) 
            { // new section, reset local counter and update secitions vectr
                if (current_section != nullptr)
                {
                    this->sections.emplace_back(current_section);
                }
                
                current_section = new Section(name);
                current_section->set_section_location_counter(0);

                Symbol_table_entry* entry = new Symbol_table_entry();
                entry->label = name;
                entry->section = name;
                entry->binding = "LOCAL";

                this->symbol_table.add_symbol_table_entry(entry);
            }
            else // should this be an error state?
            {
                
            }

            break;
        }
        case Directive_type::GLOBAL:
        {
            std::vector<std::string> operands = directive->get_operands();

            for (int i = 0; i < operands.size(); i++)
            {
                Symbol_table_entry* sym = symbol_table.find_symbol(operands[i]);
                if (sym == nullptr)
                {
                    // We did not read this symbols definition thus we don't know its
                    // value and address
                    sym = new Symbol_table_entry();
                    sym->label = operands[i];
                    sym->section = current_section->get_section_name();
                    sym->size = 0;
                    sym->defined = false;
                    sym->offset = 0;
                    sym->fref . emplace_back(current_section->get_section_location_counter());
                    sym->binding = "GLOBAL";

                    symbol_table.add_symbol_table_entry(sym);
                }
                else
                {
                    // We can just change this symbols binding to global
                    // whether it is defined or not we do not care
                    // since this directive will not define it 
                    sym->binding = "GLOBAL";
                }
            }
            break;
        }
        case Directive_type::EXTERN:
        {
            // We can generate a relocation entry whenever we get .extern since we know they will not be in 
            // this file
            break;
        }
        case Directive_type::SKIP:
        {
            std::string arg = directive->get_operands() [0];
            current_section->inc_section_location_counter(atoi(arg.c_str()));
            break;
        }
        case Directive_type::WORD:
        {
            std::vector<std::string> args = directive->get_operands();
            std::vector<Label_type> types = directive->get_operand_types();

            for (int i = 0; i < args.size(); i++)
            {
                if (i >= types.size())
                {
                    // ERROR but this should not happen
                    break;
                }

                if (types[i] == Label_type::LITERAL)
                {
                    current_section->inc_section_location_counter(2);
                    current_section->add_section_data(args[i]);
                }
                else if (types[i] == Label_type::SYMBOL)
                {
                    // If we know the value of the symbol, we write it to the section data
                    Symbol_table_entry* entry = symbol_table.find_symbol(args[i]);
                    if (entry != nullptr && entry->defined)
                    { // We know the value of the symbol so we write it to the section
                        current_section->add_section_data(std::to_string(entry->offset));
                    }
                    else if (entry != nullptr && !entry->defined)
                    { // We know the symbol but it is not defined so we make a fref and inc loc counter
                        entry->fref . emplace_back (current_section->get_section_location_counter());   
                    }
                    else
                    { // We have not encountered this symbol at this time so we make a new entry in symbol table
                        entry = new Symbol_table_entry();
                        entry->label = args[i];
                        entry->fref . emplace_back (current_section->get_section_location_counter());
                        entry->section = current_section->get_section_name();
                        entry->size = 2;
                        entry->offset = current_section->get_section_location_counter();
                        entry->binding = "LOCAL";

                        symbol_table.add_symbol_table_entry(entry);
                    }

                    current_section->inc_section_location_counter(2);
                }
                else
                {
                    std::cout << "ERROR! Register cannot be passed as .word operand!\n";
                    break;
                }
            }

            break;
        } 
        case Directive_type::LABEL:
        {
            Symbol_table_entry* entry = new Symbol_table_entry();
            entry->binding = "LOCAL";
            entry->label = directive->get_operands()[0];
            entry->section = current_section->get_section_name();
            entry->offset = current_section->get_section_location_counter();
            
            symbol_table.add_symbol_table_entry(entry);
            break;
        }
        case Directive_type::END:
        {
            // Finish parsing asm code
            sections.emplace_back(current_section);

            break;
        }
        default: break;
    }
}

void Assembly::handle_instruction(Instruction* instruction)
{
    std::string instruction_value = get_instruction_value(instruction);
    current_section->add_section_data(instruction_value);
}

bool Assembly::does_section_exists(std::string section) const
{
    for (int i = 0; i < 5; i++)
    {
        if (section == section_names[i])
        {
            return true;
        }
    }

    return false;
}

std::string Assembly::get_instruction_value(Instruction* instruction) const
{
    std::string value = "";
    value += Assembly::instruction_codes[instruction->get_type()];

    int arg_no = instruction->get_number_of_operands();
    if (arg_no == 0) return value;

    if (arg_no == 1)
    {
        if (instruction->get_type() == CALL)
        {
            value += "11110000";

            //value += Assembly::get_addressing_code(instruction->get_second_operand())
        }
        else
        {
            value += Assembly::register_codes[instruction->get_first_operand()];
            value += "0000";
        }

        return value;
    }

    //value += 

    return "";
}

