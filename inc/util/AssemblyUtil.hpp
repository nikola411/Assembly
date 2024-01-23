#pragma once

#include <stdint.h>
#include <string>
#include <unordered_map>
#include <map>
#include <memory>
#include <vector>

#include "Instruction.hpp"
#include "ParserUtil.hpp"

#define EMPTY_BYTE 0

namespace AssemblyUtil
{
    struct SymbolTableEntry
    {
        std::string label;
        std::string section;
        int offset;
        bool local;
        bool defined;
        int index;

        SymbolTableEntry(std::string label = "", std::string section = "", int offset = 0, bool local = true, bool defined = false, int index = 0)
            :label(label), section(section), offset(offset), local(local), defined(defined), index(index)
        {
        }
    };

    struct RelocationTableEntry
    {
        std::string label;
        std::string section;
        ParserUtil::eRelocationType type;
        int offset;
        int index;
    };

    struct SectionEntry
    {
        std::string name;
        std::vector<BYTE> sectionData;
        int locationCounter;
        int offset;
    };

    struct ProgramLine
    {
        ParserUtil::eInstructionIdentifier instruction;
        std::vector<ParserUtil::ParserOperand> operands;
        int numberOfArguments;
        ParserUtil::eInstructionType type;
        ParserUtil::eAddressingType addressingType;
    };

    class AssemblyException: public std::exception
    {
    public:
        AssemblyException(std::string msg): m_msg(msg) {}
        std::string what(){return m_msg;}
    private:
        std::string m_msg;
    };

    typedef std::shared_ptr<SymbolTableEntry> symbol_ptr;
    typedef std::shared_ptr<RelocationTableEntry> relocation_ptr;
    typedef std::shared_ptr<ProgramLine> line_ptr;
    typedef std::shared_ptr<SectionEntry> section_ptr;

    symbol_ptr FindSymbol(std::vector<symbol_ptr>& table, std::string& name);
    symbol_ptr CreateSymbolEntry(std::string label = "", std::string section = "", int offset = 0, bool local = true, bool defined = false, int index = 0);
    bool InsertSymbol(std::vector<symbol_ptr>& table, symbol_ptr symbol);
    bool UpdateSymbolOffset(std::vector<symbol_ptr>& table, symbol_ptr symbol, int offset);
    bool UpdateSymbolSection(std::vector<symbol_ptr>& table, symbol_ptr symbol, std::string section);
    bool UpdateSymbolLocality(std::vector<symbol_ptr>& table, symbol_ptr symbol, bool isLocal);
    bool WriteDataToSection(section_ptr sections, const std::vector<BYTE>& data, int offset);
    bool WriteDataToSection(AssemblyUtil::section_ptr section, uint32_t data, int offset, int bytesToWrite);
    bool WriteDataToSection(std::vector<AssemblyUtil::section_ptr>& sectionTable, std::string& sectionName, uint32_t data, int offset, int bytesStart, int bytesToWrite);
    bool AllocateSectionData(section_ptr section, int size);
    relocation_ptr FindRelocation(std::vector<relocation_ptr>& relocations, std::string& name);
    std::vector<BYTE> ReadDataFromSection(std::vector<section_ptr>& sections, std::string& name, int offsetStart, int offsetEnd);
}