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
    void MergeSymbolTables();
    void MergeRelocationTables();
    void MergeSectionsInOrder();

    std::vector<section_ptr> FindSectionInFiles(std::string& name, int currentFile);
    section_ptr FindSectionInGlobalSectionsTable(std::string& name);
    bool HandleSymbolSecondEncounter(symbol_ptr original, symbol_ptr found);
    void UpdateSymbolOffset(std::string& sectionName, int file, int sectionOffset);

    void ReadInputFiles();

    void ReadSymbolTableEntry(std::string& line, SymbolTable& currentSymbolTable);
    void ReadSectionsLine(std::string& line, SectionTable& currentSectionTable);
    void ReadRelocationsEntry(std::string& line, RelocationTable& currentRelocationTable);

    void PrintHexProgramData();
    void PrintRelocatableProgramData();

    SymbolTable mSymbolTable;
    SectionTable mSectionTable;
    RelocationTable mRelocationTable;

    std::vector<SymbolTable> mSymbolTableVector;
    std::vector<SectionTable> mSectionTableVector;
    std::vector<RelocationTable> mRelocationTableVector;

    LinkerArguments arguments;
};

#endif