#ifndef _LINKER_HPP
#define _LINKER_HPP

#include "util.hpp"

using namespace LinkerUtil;
using AssemblyUtil::symbol_ptr;
using AssemblyUtil::section_ptr;
using AssemblyUtil::relocation_ptr;

class Linker
{
public:
    Linker(LinkerArguments& args);

    void StartLinking();
    void WriteOutput();
private:
    void MergeSymbolTables();
    bool HandleSymbolSecondEncounter(symbol_ptr original, symbol_ptr found);
    void UpdateSymbolPointers();

    void MergeRelocationTables();

    void MergeSectionsInOrder();
    void PopulateMergeDataVector(std::vector<SectionMergeDataPtr>& vector);

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