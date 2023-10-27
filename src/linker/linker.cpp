#include "linker.hpp"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <set>

#define SPACE ' '
#define HEX_BASE 16
#define LABEL_INDEX 0
#define SECTION_INDEX 1
#define OFFSET_INDEX 2
#define LOCAL_INDEX 3
#define TYPE_INDEX 3
#define DEFINED_INDEX 4
#define EMPTY_LINE ""

using AssemblyUtil::FindSymbol;
using AssemblyUtil::AllocateSectionData;
using AssemblyUtil::WriteDataToSection;
using AssemblyUtil::FindRelocation;

std::map<std::string, eInputFileState> StateMap =
{
    { "symbol_table", eInputFileState::SYMBOL_TABLE },
    { "sections",     eInputFileState::SECTIONS },
    { "relocations",  eInputFileState::RELOCATIONS}
};

std::set<std::string> StateSet =
{
    "relocations", "symbol_table", "sections"
};

Linker::Linker(LinkerArguments& args)
    : arguments(args)
{
}

/*
Faza ucitavanja:
    Skupovi E U i D:
        -E: relokativni objektni fajlovi
        -U: nerazreseni simboli(relokacije)
        -D: skup definisanih simbola
    idemo kroz E, azuriramo U i D 
    ako je na kraju U neprazan, greska

Faza relokacija:
    1. relokacija sekcija i definicija simbola
        1.1 sekcije se pomeraju na offset po pojavljivanju (ili ako postoji direktiva place na odredjeno mesto u memoriji)
        1.2 simboli se azuriraju na osnovu promene offset-a sekcije, tj offset simbola + offset sekcije
    2. relokacija referenci simbola unutar sekcija

*/

void Linker::StartLinking()
{
    // Reading phase
    ReadInputFiles();
    MergeSymbolTables();
    MergeRelocationTables();

    //Relocations phase
    MergeSectionsInOrder();
    int i = 0;
}

void Linker::WriteOutput()
{
    arguments.hex ? PrintHexProgramData() : PrintRelocatableProgramData();
}

void Linker::MergeSymbolTables()
{
    auto currentFile = 0;

    for (auto table: mSymbolTableVector)
    {
        for (auto symbol: table)
        {
            auto entry = FindSymbol(mSymbolTable, symbol->label);
            if (entry != nullptr)
            {
                auto isHandled = HandleSymbolSecondEncounter(entry, symbol);
                if (!isHandled)
                    throw LinkerException("Cannot have two definitions of the same symbol. (" + symbol->label + ")");

                continue;
            }

            symbol->index = mSymbolTable.size();
            mSymbolTable.push_back(symbol);
        }

        ++currentFile;
    }
}

void Linker::MergeRelocationTables()
{
    for (auto relocationTable: mRelocationTableVector)
    {
        for (auto relocation: relocationTable)
        {
            if (FindSymbol(mSymbolTable, relocation->label) == nullptr)
                throw LinkerException("Undefined symbol: " + relocation->label);

            mRelocationTable.push_back(relocation);
        }
    }
}

void Linker::MergeSectionsInOrder()
{
    auto currentFile = 0;
    for (auto sections: mSectionTableVector)
    {
        auto currentSection = 0;
        for (auto section: sections)
        {
            auto globalSection = FindSectionInGlobalSectionsTable(section->name);
            if (globalSection == nullptr)
            {
                mSectionTable.push_back(section);
                globalSection = section;
            }

            auto size = section->sectionData.size();
            AllocateSectionData(globalSection, size);
            WriteDataToSection(globalSection, section->sectionData, globalSection->locationCounter);

            for (auto symbol: mSymbolTableVector[currentFile])
                if (symbol->section == section->name)
                    symbol->offset += section->offset;
        }

        ++currentFile;
    }
}

std::vector<section_ptr> Linker::FindSectionInFiles(std::string& name, int currentFile)
{
    std::vector<section_ptr> ret = {};

    for (auto i = currentFile + 1; i < mSectionTableVector.size(); ++i)
    {
        if (i == currentFile)
            continue;
        
        for (auto section: mSectionTableVector[i])
        {
            if (section->name == name)
                ret.push_back(section);
        }
    }

    return ret;
}

section_ptr Linker::FindSectionInGlobalSectionsTable(std::string& name)
{
    for (auto section: mSectionTable)
        if (section->name == name)
            return section;

    return nullptr;
}

bool Linker::HandleSymbolSecondEncounter(symbol_ptr original, symbol_ptr found)
{
    if (original->label == original->section && original->label == found->label)
        return true;

    if (!original->defined && !found->defined) // both undefined
        return true;
    
    if (original->defined && found->defined) // both defined
        return false;

    if (!original->defined && found->defined) // defined found but have undefined
    {
        original->section = found->section;
        original->offset = found->offset;
        original->defined = found->defined;

        return true;
    }

    found->section = original->section;
    found->offset = original->offset;
    found->defined = original->defined;
    // undefined found but original defined
    return true;
}

void Linker::UpdateSymbolOffset(std::string& sectionName, int file, int sectionOffset)
{
    for (auto symbol: mSymbolTableVector[file])
    {
        if (symbol->section == sectionName && symbol->label != symbol->section)
            symbol->offset += sectionOffset;
    }
    
}

void Linker::ReadInputFiles()
{
    for (auto inFile: arguments.inFiles)
    {
        std::string line("");
        std::ifstream file(inFile, std::ios_base::openmode::_S_in);
        auto state = eInputFileState::SYMBOL_TABLE;

        SymbolTable currentSymbolTable;
        SectionTable currentSectionsTable;
        RelocationTable currentRelocationTable;

        while (file)
        {
            std::getline(file, line);
            if (line == EMPTY_LINE)
                continue;

            if (StateSet.find(line) != StateSet.end())
            {
                state = StateMap[line];
                continue;
            }

            switch (state)
            {
            case eInputFileState::SYMBOL_TABLE:
                ReadSymbolTableEntry(line, currentSymbolTable);
                break;
            case eInputFileState::SECTIONS:
                ReadSectionsLine(line, currentSectionsTable);
                break;
            case eInputFileState::RELOCATIONS:
                ReadRelocationsEntry(line, currentRelocationTable);
                break;
            }
        }

        mSymbolTableVector.push_back(currentSymbolTable);
        mSectionTableVector.push_back(currentSectionsTable);
        mRelocationTableVector.push_back(currentRelocationTable);
    }
}

void Linker::ReadSymbolTableEntry(std::string& line, SymbolTable& currentSymbolTable)
{
    // name section offset isLocal
    std::size_t size;
    auto symbolAttributes = Split(line, SPACE);
    auto symbolEntry = std::make_shared<AssemblyUtil::SymbolTableEntry>();

    symbolEntry->label   = symbolAttributes[LABEL_INDEX];
    symbolEntry->section = symbolAttributes[SECTION_INDEX];
    symbolEntry->offset  = (int) std::stoi(symbolAttributes[OFFSET_INDEX], &size, HEX_BASE);
    symbolEntry->local   = (bool)std::stoi(symbolAttributes[LOCAL_INDEX], &size, HEX_BASE);
    symbolEntry->defined = (bool)std::stoi(symbolAttributes[DEFINED_INDEX], &size, HEX_BASE);

    currentSymbolTable.push_back(symbolEntry);
}

void Linker::ReadSectionsLine(std::string& line, SectionTable& currentSectionTable)
{
    std::size_t size;
    auto bytes = Split(line, SPACE);
    
    if (bytes.size() == 3)
    {
        auto section = std::make_shared<AssemblyUtil::SectionEntry>();
        section->name = bytes.front();
        section->locationCounter = std::stoi(bytes.back(), &size, HEX_BASE);
        section->offset = std::stoi(bytes.back(), &size, HEX_BASE);

        currentSectionTable.push_back(section);
        return;
    }

    for (auto byte: bytes)
    {
        currentSectionTable.back()->sectionData.push_back(std::stoi(byte, &size, HEX_BASE));
    }
}

void Linker::ReadRelocationsEntry(std::string& line, RelocationTable& currentRelocationTable)
{
    // name section type offset
    std::size_t size;
    auto relocationInfo  = Split(line, SPACE);
    auto relocationEntry = std::make_shared<AssemblyUtil::RelocationTableEntry>();

    relocationEntry->label   = relocationInfo[LABEL_INDEX];
    relocationEntry->section = relocationInfo[SECTION_INDEX];
    relocationEntry->offset  = std::stoi(relocationInfo[OFFSET_INDEX], &size, HEX_BASE);
    relocationEntry->type    = (ParserUtil::eRelocationType)std::stoi(relocationInfo[TYPE_INDEX], &size, HEX_BASE);

    currentRelocationTable.push_back(relocationEntry);
}

void Linker::PrintHexProgramData()
{
}

void Linker::PrintRelocatableProgramData()
{
    std::fstream file;
    auto outFile = arguments.outFile;

    file.open(outFile, std::ios_base::openmode::_S_out);

    file << "symbol_table" << "\n";
    for (auto entry: mSymbolTable)
    {
        file << entry->label << " " << entry->section << " "
                << std::hex << entry->offset << " " << entry->local << " "
                << entry->defined << "\n";
    }
    file << "\n" << "sections" << "\n";
    for (auto section: mSectionTable)
    {
        file << section->name << " " << section->locationCounter << "\n";
        auto i = 0;
        for (auto byte: section->sectionData)
        {
            file << std::hex << std::setw(2) << std::setfill('0') <<(short)byte << " ";
            if (++i == 4)
            {
                i = 0;
                file << "\n";
            }
        }
    }
    file << "\nrelocations\n";
    for (auto relocation: mRelocationTable)
    {
        file << relocation->label << " " << relocation->section << " "
                    << relocation->offset << " " << relocation->type << "\n";
    }

    file.close();
}
