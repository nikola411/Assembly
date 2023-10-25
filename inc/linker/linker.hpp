#ifndef _LINKER_HPP
#define _LINKER_HPP

#include "util.hpp"

using namespace LinkerUtil;
using AssemblyUtil::symbol_ptr;
using AssemblyUtil::section_ptr;
using AssemblyUtil::relocation_ptr;

namespace LinkerUtil
{
    typedef std::vector<symbol_ptr> SymbolTable;
    typedef std::vector<section_ptr> SectionTable;
    typedef std::vector<relocation_ptr> RelocationTable;

    enum eInputFileState
    {
        SYMBOL_TABLE,
        SECTIONS,
        RELOCATIONS
    };
}

class Linker
{
public:
    Linker(LinkerArguments& args);

    void StartLinking();
    void WriteOutput();
private:
    void ReadInputFiles();

    symbol_ptr ReadSymbolTableEntry(std::string& line);
    section_ptr ReadSectionsLine(std::string& line);
    relocation_ptr ReadRelocationsEntry(std::string& line);

    SymbolTable mSymbolTable;
    SectionTable mSectionTable;
    RelocationTable mRelocationTable;

    std::vector<SymbolTable> mSymbolTableVector;
    std::vector<SectionTable> mSectionTableVector;
    std::vector<RelocationTable> mRelocationTableVector;

    LinkerArguments arguments;
};

#endif