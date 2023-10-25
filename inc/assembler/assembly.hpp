#ifndef ASSEMBLY_HPP
#define ASSEMBLY_HPP

#include "util.hpp"
#include "codes.hpp"

#include <vector>
#include <memory>
#include <stdint.h>

class Assembly;

typedef void (Assembly::* HandleMethodPtr)(AssemblyUtil::line_ptr);

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

    void PrintProgram(std::string outFile);
private:
    // util
    int GetVariantOperandNumber(AssemblyUtil::line_ptr line) const;
    bool CanOperandHaveOffset(AssemblyUtil::line_ptr line) const;
    uint16_t GetDataValue(std::vector<ParserUtil::ParserOperand>& operands, ProcessorUtil::eValueToUse value);
    uint16_t GetSymbolValue(std::string& name, ParserUtil::eAddressingType adressingType);
    uint16_t GetRegisterValue(std::string& name, ParserUtil::eOperandType type);
    uint16_t GetLiteralValue(std::string& value);
    ParserUtil::eOperandType GetOperandType(std::vector<ParserUtil::ParserOperand>& operands, AssemblyUtil::line_ptr line);

    bool InsertSection(AssemblyUtil::section_ptr section);
    bool CreateRelocationEntry(std::string name, std::string section, int offset);

    // Directives handles
    void SectionFirstPass(AssemblyUtil::line_ptr line);
    void EndFirstPass(AssemblyUtil::line_ptr line);
    void ExternFirstPass(AssemblyUtil::line_ptr line);
    void LabelFirstPass(AssemblyUtil::line_ptr line);

    void SectionSecondPass(AssemblyUtil::line_ptr line);
    void EndSecondPass(AssemblyUtil::line_ptr line);
    void SkipSecondPass(AssemblyUtil::line_ptr line);
    void WordSecondPass(AssemblyUtil::line_ptr line);
    void LabelSecondPass(AssemblyUtil::line_ptr line);
    // Other handles
    void OtherTypeFirstPass(AssemblyUtil::line_ptr line);
    void MemoryTypeFirstPass(AssemblyUtil::line_ptr line);
    void DoNothing(AssemblyUtil::line_ptr line);

    void InitializeHandleMap();

    AssemblyUtil::line_ptr m_currentLine;
    std::vector<AssemblyUtil::line_ptr> m_program;
    std::vector<AssemblyUtil::symbol_ptr> m_symbolTable;
    std::vector<AssemblyUtil::section_ptr> m_sections;
    std::vector<AssemblyUtil::relocation_ptr> m_relcationTable;
    std::map<bool, std::map<ParserUtil::eInstructionType, std::map<ParserUtil::eInstructionIdentifier, HandleMethodPtr>>> m_handles;

    AssemblyUtil::section_ptr m_currentSection;
    bool m_end; // .END encountered
    int m_locationCounter;
};

#endif