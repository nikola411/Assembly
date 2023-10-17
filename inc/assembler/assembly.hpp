#ifndef ASSEMBLY_HPP
#define ASSEMBLY_HPP

#include "util.hpp"
#include "codes.hpp"

#include <vector>
#include <memory>
#include <stdint.h>

class Assembly
{
public:
    Assembly();

    void SetInstruction(ParserUtil::eInstructionIdentifier instruction, ParserUtil::eInstructionType type);
    void SetOperand(std::string value, ParserUtil::eOperandType type);
    void SetOperand(ParserUtil::ParserOperand& operand);
    void SetMultipleOperands(std::vector<ParserUtil::ParserOperand> operands);
    void FinishInstruction();

    void ContinueParsing();

    void PrintProgram();
private:
    int GetVariantOperandNumber(AssemblyUtil::line_ptr line) const;
    bool CanOperandHaveOffset(AssemblyUtil::line_ptr line) const;
    uint16_t GetDataValue(std::vector<ParserUtil::ParserOperand>& operands, ProcessorUtil::eValueToUse value);
    uint16_t GetSymbolValue(std::string& name);
    uint16_t GetRegisterValue(std::string& name, eOperandType type);
    uint16_t GetLiteralValue(std::string& value);

    void HandleDirective(AssemblyUtil::line_ptr line);

    AssemblyUtil::line_ptr m_currentLine;
    std::vector<AssemblyUtil::line_ptr> m_program;
    std::vector<AssemblyUtil::symbol_ptr> m_symbolTable;
    std::vector<AssemblyUtil::section_ptr> m_sections;
    std::vector<AssemblyUtil::relocation_ptr> m_relcationTable;

    AssemblyUtil::section_ptr m_currentSection;
    bool m_end;    
    int m_locationCounter;
};


#endif