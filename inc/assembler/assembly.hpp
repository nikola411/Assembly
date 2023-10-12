#ifndef ASSEMBLY_HPP
#define ASSEMBLY_HPP

#include "util.hpp"

#include <vector>
#include <memory>
#include <stdint.h>

class Assembly
{
public:
    Assembly();

    void SetInstruction(eInstructionIdentifier instruction, eInstructionType type);
    void SetOperand(std::string value, eOperandType type);
    void SetMultipleOperands(std::vector<std::pair<std::string, eOperandType>> operands);
    void SetAddressingType(eAddressingType addrType);

    void FinishInstruction();
    void ParseLine();

private:
    line_ptr m_currentLine;
    std::vector<line_ptr> m_program;
    std::vector<symbol_ptr> m_symbolTable;
    std::vector<relocation_ptr> m_relcationTable;
    
    int m_locationCounter;
};


#endif