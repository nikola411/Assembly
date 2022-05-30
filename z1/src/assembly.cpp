#include "assembly.hpp"

#include <iostream>

std::string Assembly::section_names[5] = {".text", ".data", ".bss", ".rel", ".rodata"};

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
        sections[i] -> print();
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
    switch (directive -> get_type())
    {
        case Directive_type::SECTION:
        {
            std::string name = (directive -> get_arguments()) [0];

            if (!does_section_exists(name))
            {
                // ERROR
                std::cout << "Error! Wrong section name! \n";
                break;
            }

            if (current_section == nullptr || name != current_section -> get_section_name()) 
            { // new section, reset local counter and update secitions vectr
                if (current_section != nullptr)
                {
                    this->sections.emplace_back(current_section);
                }
                
                current_section = new Section(name);

                Symbol_table_entry* entry = new Symbol_table_entry();
                entry -> label = name;
                entry -> section = name;
                entry -> binding = "LOCAL";

                this->symbol_table.add_symbol_table_entry(entry);
            }
            else // should this be an error state?
            {
                
            }

            break;
        }
        case Directive_type::GLOBAL:
        {
            std::vector<std::string> operands = directive -> get_arguments();

            for (int i = 0; i < operands.size(); i++)
            {
                Symbol_table_entry* sym = symbol_table.find_symbol(operands[i]);
                if (sym == nullptr)
                {
                    // We did not read this symbols definition thus we don't know its
                    // value and address
                    sym = new Symbol_table_entry();
                    sym -> label = operands[i];
                    sym -> section = current_section -> get_section_name();
                    sym -> size = 0;
                    sym -> defined = false;
                    sym -> value = 0;
                    sym -> fref . emplace_back(current_section -> get_section_location_counter());
                    sym -> binding = "GLOBAL";

                    symbol_table.add_symbol_table_entry(sym);
                }
                else
                {
                    // We can just change this symbols binding to global
                    // whether it is defined or not we do not care
                    // since this directive will not define it 
                    sym -> binding = "GLOBAL";
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
            std::string arg = directive -> get_arguments() [0];
            current_section -> inc_section_location_counter(atoi(arg.c_str()));
            break;
        }
        case Directive_type::WORD:
        {
            std::vector<std::string> args = directive -> get_arguments();
            std::vector<Label_type> types = directive -> get_argument_types();

            for (int i = 0; i < args.size(); i++)
            {
                if (i >= types.size())
                {
                    // ERROR but this should not happen
                    break;
                }

                if (types[i] == Label_type::LITERAL)
                {
                    current_section -> inc_section_location_counter(2);
                    current_section -> add_section_data(args[i]);
                }
                else if (types[i] == Label_type::SYMBOL)
                {
                    // If we know the value of the symbol, we write it to the section data
                    Symbol_table_entry* entry = symbol_table.find_symbol(args[i]);
                    if (entry != nullptr && entry -> defined)
                    { // We know the value of the symbol so we write it to the section
                        current_section -> add_section_data(std::to_string(entry -> value));
                    }
                    else if (entry != nullptr && !entry -> defined)
                    { // We know the symbol but it is not defined so we make a fref and inc loc counter
                        entry -> fref . emplace_back (current_section -> get_section_location_counter());   
                    }
                    else
                    { // We have not encountered this symbol at this time so we make a new entry in symbol table
                        entry = new Symbol_table_entry();
                        entry -> label = args[i];
                        entry -> fref . emplace_back (current_section -> get_section_location_counter());
                        entry -> section = current_section -> get_section_name();
                        entry -> size = 2;
                        entry -> value = 0;
                        entry -> binding = "LOCAL";

                        symbol_table.add_symbol_table_entry(entry);
                    }

                    current_section -> inc_section_location_counter(2);
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
            std::cout << "Label!!! ";
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

