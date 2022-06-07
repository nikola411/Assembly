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

Assembly::Assembly() : end_occured(false)
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
        handle_jump((Jump*)line);
    }
    else if (typeid(*lines.back()) == instruction_type)
    {
        handle_instruction((Instruction*)line);
    }
    else if (typeid(*lines.back()) == directive_type)
    {
        handle_directive((Directive*)line);
    }
}

void Assembly::finish_parsing()
{
    // Backpatching here



    // write to output.o
}

void Assembly::print()
{
    symbol_table.print();
    std::cout << "\n";
    for (int i = 0; i < sections.size(); i++)
    {
        sections[i]->print();
    }

    std::cout << "\n";

    relocation_table.print();
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

}

void Assembly::handle_jump(Jump* jump)
{
    std::string value = jump_codes[jump->get_type()];
    std::string operand = jump->get_operand_value();
    Label_type operand_type = jump->get_operand_type();
    bool is_register = false;

    // Filling register bytes
    value += "1111";
    if (operand_type == Label_type::REGISTER)
    {
        value += register_codes[jump->get_operand_value()];
        is_register = true;
    }
    else
    {
        value += "1111";
    }

    // Filling addressing bytes
    Addressing_type addr_type = jump->get_addressing_type();
    value += "0000"; // dummy padding, should have a meaningful value
    if (is_register)
    {
        if (addr_type == Addressing_type::LITERAL_OFFSET)
        {
            value += addressing_codes["REGISTER_OFFSET"];
        }
        else if (addr_type == Addressing_type::SYMBOL_OFFSET)
        {
            value += addressing_codes["REGISTER_OFFSET"];
        }
        else if (addr_type == Addressing_type::MEMORY)
        {
            value += addressing_codes["REGISTER_INDIRECT"];
        }
        else if (addr_type == Addressing_type::PC_RELATIVE)
        {
            value += addressing_codes["REGISTER_OFFSET"];
        }
        else // REGISTER DIRECT
        {
            value += addressing_codes["REGISTER_DIRECT"];
        }
    }
    else
    {
        if (addr_type == ABSOLUTE)
        {
            value += addressing_codes["IMMEDIATE"];
        }
        else
        {
            value += "0100";
        }
    }

    // Filling payload bytes
    std::string offset = jump->get_offset_value();
    std::string to_convert = "";
    if (addr_type == Addressing_type::LITERAL_OFFSET || operand_type == Label_type::LITERAL)
    {
        if (addr_type == Addressing_type::LITERAL_OFFSET)
        {
            to_convert = offset;
        }
        else
        {
            to_convert = operand;
        }
        
        value += std::bitset<16>(std::stoi(to_convert)).to_string();
    }
    else if (addr_type == Addressing_type::SYMBOL_OFFSET || operand_type == Label_type::SYMBOL)
    {
        if (operand_type == Label_type::SYMBOL)
        {
            to_convert = operand;
        }
        else // addr_type == Addressing_type::SYMBOL_OFFSET
        {
            to_convert = offset;
        }

        value += get_symbol_value_or_relocate(to_convert, addr_type, Label_type::SYMBOL);
    }
    

    //Adding value to section
    current_section->add_section_data(value);
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
                    sym->defined = false;
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
            std::vector<std::string> operands = directive->get_operands();

            for (int i = 0; i < operands.size(); i++)
            {
                Symbol_table_entry* sym = symbol_table.find_symbol(operands[i]);
                if (sym != nullptr)
                {
                    //ERROR
                    std::cout << "ERROR! Symbol that is in this file cannot be extern! \n";
                }

                sym = new Symbol_table_entry();
                sym->binding = "GLOBAL";
                sym->defined = false;
                sym->label = operands[i];

                symbol_table.add_symbol_table_entry(sym);

                Relocation_entry* relocation = new Relocation_entry();
                relocation->offset = current_section->get_section_location_counter();
                relocation->section = current_section->get_section_name();
                relocation->ord_number = symbol_table.get_symbol_ord_number(operands[i]);
                relocation->type = Relocation_type::R_PC_RELATIVE;

                relocation_table.add_new_relocation(relocation);
            }

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
                    current_section->add_section_data(std::bitset<16>(stoi(args[i])).to_string());
                    //current_section->inc_section_location_counter(2);
                }
                else if (types[i] == Label_type::SYMBOL)
                {
                    // If we know the value of the symbol, we write it to the section data
                    std::string value = get_symbol_value_or_relocate(args[i], ABSOLUTE, SYMBOL);
                    current_section->add_section_data(value);
                    //current_section->inc_section_location_counter(2);
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
            std::string label = directive->get_operands()[0];
            Symbol_table_entry* entry = this->symbol_table.find_symbol(label);

            if (entry == nullptr)
            {
                entry->binding = "LOCAL";
                entry->label = label;
                entry->section = current_section->get_section_name();
                entry->offset = current_section->get_section_location_counter();
                entry->defined = true;
                entry->size = 2;
                symbol_table.add_symbol_table_entry(entry);
            }
            else
            {    
                entry->binding = "LOCAL";
                entry->offset = current_section->get_section_location_counter();
                entry->section = current_section->get_section_name();
                entry->size = 2;
                entry->defined = true;
            }
           
            break;
        }
        case Directive_type::END:
        {
            // Finish parsing asm code
            sections.emplace_back(current_section);
            end_occured = true;
            finish_parsing();
            // Start backpatching
            break;
        }
        default: break;
    }
}

void Assembly::handle_instruction(Instruction* instruction)
{
    std::string value = get_instruction_value(instruction);

    current_section->add_section_data(value);
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
// this method will not return only addressing byte but also if needed the payload bytes
std::string Assembly::get_addressing_byte_value(Instruction* instruction) const
{
    Addressing_type addr_type = instruction->get_addressing_type();
    std::string addr_byte = "0000";
    switch (addr_type)
    {
        case SYMBOL_OFFSET :
        {
            addr_byte += "0011";
            break;
        }
        case LITERAL_OFFSET :
        {
            addr_byte += "0011";
            break;
        }
        case MEMORY :
        {
            addr_byte += "0100";
            break;
        }
        case ABSOLUTE :
        {
            addr_byte += "0000";
            break;
        }
        case PC_RELATIVE :
        {
            addr_byte += "0100";
            break;
        }
        default: break;
    }

    return addr_byte;
}

std::string Assembly::get_instruction_value(Instruction* instruction)
{
    Instruction_type type = instruction->get_type();
    std::string value = "";

    int operands_cnt = instruction->get_number_of_operands();

    if (operands_cnt == 0)
    {
        value += instruction_codes[type];
        return value;
    }

    if (operands_cnt == 1)
    {
        std::string operand_value = "";
        std::string operand;

        value += instruction_codes[type];

        if (type == Instruction_type::CALL)
        {
            // Here we call get second operand because when parsing, since Instruction class is
            // made to recognize only registers as first operands
            // we pack call operand as second operand to be able to know its type
            operand = instruction->get_second_operand(); 
            Label_type type = instruction->get_second_operand_type();

            if (type == REGISTER)
            {
                value += register_codes[operand] + "1111";
            }
            else
            {
                value += "111111111";
            }

            value += get_addressing_byte_value(instruction);
            value += get_payload_byte_value(instruction);

            return value;
        }
        else
        {
            operand = instruction->get_first_operand();
            value += register_codes[operand] + "0000";
            value += get_addressing_byte_value(instruction);
        }

        return value;
    }
    
    // 2 operand instruction
    if (instruction->get_type() == Instruction_type::LDR ||
        instruction->get_type() == Instruction_type::STR)
    {
        value += instruction_codes[type];
        std::string op1 = instruction->get_first_operand();
        std::string op2 = instruction->get_second_operand();
        value += register_codes[op1];
    
        Label_type sec_op_type = instruction->get_second_operand_type();

        if (sec_op_type == Label_type::REGISTER)
        {
            value += register_codes[op2];
        }
        else
        {
            value += "1111";
        }

        value += get_addressing_byte_value(instruction);
        value += get_payload_byte_value(instruction);
        // second operand type is symbol so we need to see about that
    }
    else
    {
        std::string r1 = instruction->get_first_operand();
        std::string r2 = instruction->get_second_operand();

        value += instruction_codes[type] +
                register_codes[r1] + register_codes[r2] + // registers byte
                get_addressing_byte_value(instruction); // addressing byte
    }
    
    return value;
}

std::string Assembly::get_symbol_value_or_relocate(std::string symbol, Addressing_type addr_type, Label_type label_type)
{
    Symbol_table_entry* entry = symbol_table.find_symbol(symbol);
    std::string operand_value = std::bitset<16>(0).to_string();
    if (entry == nullptr)
    {
        entry = new Symbol_table_entry();
        entry->label = symbol;
        entry->fref.emplace_back(current_section->get_section_location_counter());
        entry->defined = false;

        symbol_table.add_symbol_table_entry(entry);
    }

    if (current_section->get_section_name() == entry->section && entry->defined)
    {
        if (entry->defined)
        {
            int offset = entry->offset;
            int size = entry->size;
            operand_value = current_section->read_section_data(offset, size);
        }
        else
        {
            entry->fref.emplace_back(current_section->get_section_location_counter());
        }
    }
    else
    {
        Relocation_entry* relocation = new Relocation_entry();
        relocation->offset = current_section->get_section_location_counter();
        relocation->section = current_section->get_section_name();
        relocation->ord_number = symbol_table.get_symbol_ord_number(symbol);
        relocation->type = Relocation_type::R_PC_RELATIVE;

        relocation_table.add_new_relocation(relocation);
    }            
   
    return operand_value;
}

std::string Assembly::get_payload_byte_value(Instruction* instruction)
{
    Addressing_type addr_type = instruction->get_addressing_type();
    Label_type labl_type = instruction->get_second_operand_type();
    std::string op2 = instruction->get_second_operand();
    std::string value = "";

    if (addr_type == Addressing_type::ABSOLUTE || addr_type == Addressing_type::MEMORY || addr_type == Addressing_type::PC_RELATIVE)
    {
        if (labl_type == LITERAL)
        {
            value += std::bitset<16>(std::stoi(op2)).to_string();
        }
        else if (labl_type == SYMBOL)
        {
            value += get_symbol_value_or_relocate(op2, addr_type, labl_type);
        }

        return value;
    }

    if (addr_type == Addressing_type::LITERAL_OFFSET)
    {
        value += std::bitset<16>(std::stoi(op2)).to_string();
        return value;
    }

    // Addressing_type::SYMBOL_OFFSET

    value = get_symbol_value_or_relocate(op2, addr_type, instruction->get_second_operand_type());

    return value;
}

