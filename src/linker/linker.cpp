#include "linker.hpp"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <set>
#include <algorithm>

#define SPACE ' '
#define HEX_BASE 16
#define LABEL_INDEX 0
#define SECTION_INDEX 1
#define OFFSET_INDEX 2
#define LOCAL_INDEX 3
#define TYPE_INDEX 3
#define DEFINED_INDEX 4
#define EMPTY_LINE ""

#define FILE first
#define DATA second
#define LocationCounter sectionData.size()
#define NEXT_STATE(state) (eInputFileState)((state + 1) % 3)

using AssemblyUtil::FindSymbol;
using AssemblyUtil::AllocateSectionData;
using AssemblyUtil::WriteDataToSection;
using AssemblyUtil::FindRelocation;
using AssemblyUtil::ReadDataFromSection;
using AssemblyUtil::WriteDataToSection;

std::vector<eInputFileState> states =
{
    eInputFileState::SYMBOL_TABLE, eInputFileState::SECTIONS, eInputFileState::RELOCATIONS
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
    HandleRelocations();
    int i = 0;
}

void Linker::WriteOutput()
{
    arguments.hex ? PrintHexProgramData() : PrintRelocatableProgramData();
}

void Linker::MergeSymbolTables()
{
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
    }

    for (auto symbol: mSymbolTable)
        if (!symbol->defined)
            throw LinkerException("Undefined symbol: " + symbol->label + " in section: " + symbol->section);

    UpdateSymbolPointers();
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

void Linker::HandleRelocations()
{
    for (auto relocation: mRelocationTable)
    {
        auto sectionName = relocation->section;
        auto offset = relocation->offset;
        auto symbolName = relocation->label;

        auto entry = FindSymbol(mSymbolTable, symbolName);
        if (entry == nullptr)
            throw LinkerException("Undefined symbol: " + symbolName);

        auto sectionOffset = 0;
        for (auto section: mSectionTable)
        {
            if (section->name == entry->section)
            {
                sectionOffset = section->offset;
            }
        }

        for (auto section: mSectionTable)
        {
            if (section->name == sectionName)
            {
                auto value = entry->offset + sectionOffset;
                auto firstByte = section->sectionData[offset];
                auto secondByte = section->sectionData[offset + 1];

                firstByte |= (value >> (2 * BYTE_SIZE)) & 0x0F;
                secondByte = (value & 0xFF);
                
                section->sectionData[offset] = firstByte;
                section->sectionData[offset + 1] = secondByte;

                break;
            }
        }
    }
}

void Linker::MergeSectionsInOrder()
{
    std::vector<SectionMergeDataPtr> mergeVector = {};
    PopulateMergeDataVector(mergeVector);
    
    auto offset = 0;

    for (auto entry: mergeVector)
    {
        auto sectionName = entry->sections.front().DATA->name;
        auto baseSection = std::make_shared<AssemblyUtil::SectionEntry>();
        baseSection->name = sectionName;
        baseSection->offset = offset;
    
        for (auto section: entry->sections)
        {
            WriteDataToSection(baseSection, section.DATA->sectionData, baseSection->LocationCounter);
            UpdateSymbolsOffset(mSymbolTableVector[section.FILE], sectionName, offset - baseSection->offset);
            UpdateRelocationsOffset(mRelocationTableVector[section.FILE], sectionName, offset - baseSection->offset);     

            offset += section.DATA->LocationCounter;
        }

        mSectionTable.push_back(baseSection);
    }
}

void Linker::PopulateMergeDataVector(std::vector<SectionMergeDataPtr>& vector)
{
    std::map<std::string, SectionMergeDataPtr> sectionsMap;
    auto index = 0;
    auto file  = 0;

    for (auto fileSections: mSectionTableVector)
    {
        for (auto section: fileSections)
        {
            if (sectionsMap.find(section->name) == sectionsMap.end())
            {
                auto mergeData = std::make_shared<SectionMergeData>();;
                mergeData->index = ++index;
                mergeData->name = section->name;
                mergeData->AddSection(file, section);

                sectionsMap[section->name] = mergeData;
                continue;
            }

            sectionsMap[section->name]->AddSection(file, section);
        }
        ++file;
    }

    for (auto sectionEntry: sectionsMap)
    {
        auto sectionMergeData = sectionEntry.second;
        vector.push_back(sectionMergeData);
    }

    std::sort(vector.begin(), vector.end(), [](SectionMergeDataPtr first, SectionMergeDataPtr second){ return first->index < second->index; });
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

void Linker::UpdateSymbolPointers()
{
    for (auto symbol: mSymbolTable) // update symbol pointers in all tables to the defined version of the symbol
        for (auto table: mSymbolTableVector)
            for (auto i = 0; i < table.size(); ++i)
                if (table[i]->label == symbol->label)
                    table[i] = symbol;
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
            {
                state = NEXT_STATE(state);
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
    symbolEntry->index = currentSymbolTable.size();

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
    // name section offset type
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
    std::fstream file;
    const auto& outFile = arguments.outFile;

    file.open(outFile, std::ios_base::openmode::_S_out);
    int instructionCounter = 0;

    file << std::hex;
    for (const auto& section: mSectionTable)
    {
        for (const auto& byte: section->sectionData)
        {
            file << std::setw(2) << std::setfill('0') <<(short)byte << " ";
            if (++instructionCounter == 4)
            {
                file << "\n";
                instructionCounter = 0;
            }
        }
    }
}

void Linker::PrintRelocatableProgramData()
{
    std::fstream file;
    auto outFile = arguments.outFile;

    file.open(outFile, std::ios_base::openmode::_S_out);

    file << "symbol_table" << "\n";
    file << std::hex;
    for (auto entry: mSymbolTable)
    {
        file << entry->label << " " << entry->section << " "
                << entry->offset << " " << entry->local << " "
                << entry->defined << "\n";
    }

    file << "\n" << "sections" << "\n";
    for (auto section: mSectionTable)
    {
        file << section->name << " " << section->offset << "\n";
        auto i = 0;
        for (auto byte: section->sectionData)
        {
            file << std::setw(2) << std::setfill('0') <<(short)byte << " ";
            if (++i == 4)
            {
                i = 0;
                file << "\n";
            }
        }
    }

    file.close();
}
