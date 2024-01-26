#include "Linker.hpp"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <set>
#include <algorithm>

#include "Util.hpp"

    /*
        1: init init 0 1 1
        2: meta meta 0 1 1
        3: looping looping 0 1 1
        4: loop looping 0 1 1
    */

#define SPACE ' '
#define HEX_BASE 16
#define LABEL_INDEX 1
#define SECTION_INDEX 2
#define OFFSET_INDEX 3
#define LOCAL_INDEX 4
#define TYPE_INDEX 4
#define DEFINED_INDEX 5
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

static std::map<std::string, eInputFileState> states = 
{
    { "SYMBOL TABLE:", eInputFileState::SYMBOL_TABLE },
    { "SECTIONS:", eInputFileState::SECTIONS },
    { "RELOCATIONS:", eInputFileState::RELOCATIONS },
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
    for (const auto& table: mSymbolTableVector)
    {
        for (const auto& symbol: table)
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

    for (const auto& symbol: mSymbolTable)
        if (!symbol->defined)
            throw LinkerException("Undefined symbol: " + symbol->label + " in section: " + symbol->section);

    UpdateSymbolPointers();
}

void Linker::MergeRelocationTables()
{
    for (const auto& relocationTable: mRelocationTableVector)
    {
        for (const auto& relocation: relocationTable)
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

                firstByte |= (value >> BYTE_SIZE) & 0x0F;
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

    for (const auto& entry: mergeVector)
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

    if (arguments.place.size() != 0)
    {
        auto compare = [](std::pair<std::string, unsigned long> first, std::pair<std::string, unsigned long> second)
        {
            return first.second > second.second;
        };

        std::sort(arguments.place.begin(), arguments.place.end(), compare);

        for (const auto& place : arguments.place)
        {
            for (auto& section : mSectionTable)
            {
                if (section->name == place.first)
                {
                    section->offset = place.second;
                    break;
                }
            }
        }
    }
}

// void Linker::MergeSections()
// {
//     std::map<std::string, section_ptr> sections = {};
//     int offset = 0;

//     for (int file = 0; file < mSectionTableVector.size(); ++file)
//     {
//         auto currentSectionVector = mSectionTableVector[file];
//         for (const auto& section : currentSectionVector)
//         {
//             if (sections.find(section->name) == sections.end())
//             {
//                 auto newSection = std::make_shared<AssemblyUtil::SectionEntry>();
//                 newSection->name = section->name;
//                 newSection->offset = offset;

//                 for (const auto byte : section->sectionData)
//                 {
//                     newSection->sectionData.push_back(byte);
//                 }

//                 for (const auto entry : section->valueMap)
//                 {
//                     newSection->valueMap.push_back(entry);
//                 }

//                 newSection->locationCounter = section->locationCounter;
                
//                 offset += newSection->sectionData.size();
//                 continue;
//             }

//             auto& baseSection = sections[section->name];

//             for (auto entry : baseSection->valueMap)
//             {
//                 // check if present in next section
//                 bool present = false;
//                 for (auto secondEntry : section->valueMap)
//                     if (secondEntry == )
//             }
//         }
//     }
// }

void Linker::PopulateMergeDataVector(std::vector<SectionMergeDataPtr>& vector)
{
    std::map<std::string, SectionMergeDataPtr> sectionsMap;
    auto index = 0;
    auto file  = 0;

    for (const auto& fileSections: mSectionTableVector)
    {
        for (const auto& section: fileSections)
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

    for (const auto& sectionEntry: sectionsMap)
    {
        auto sectionMergeData = sectionEntry.second;
        vector.push_back(sectionMergeData);
    }

    auto orderFunction = [](SectionMergeDataPtr first, SectionMergeDataPtr second)
    {
        return first->index < second->index;
    };

    std::sort(vector.begin(), vector.end(), orderFunction);
}

void Linker::UpdateSectionPoolEntries(section_ptr base, section_ptr next)
{
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
    for (const auto&  symbol: mSymbolTable) // update symbol pointers in all tables to the defined version of the symbol
        for (auto& table: mSymbolTableVector)
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
                continue;
            
            std::string strippedLine = Strip(line);
            if (states.find(strippedLine) != states.end())
            {
                state = states[strippedLine];
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

        // read literal pool from last section that appeared since we don't read it for only that seciton
        auto& lastSection = currentSectionsTable.back();
        ReadSectionLiteralPoolInReverse(lastSection);
        std::reverse(lastSection->valueMap.begin(), lastSection->valueMap.end());

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

    if (bytes.size() == 4)
    {
        auto section = std::make_shared<AssemblyUtil::SectionEntry>();
        section->name = bytes.front(); 
        section->locationCounter = std::stoi(bytes[1], &size, HEX_BASE);
        section->offset = std::stoi(bytes[2], &size, HEX_BASE);
        section->valueMapSize = std::stoi(bytes[3].substr(0, bytes[3].size() - 1), &size, HEX_BASE);

        if (!currentSectionTable.empty()) //read last sections valueMap (literal pool)
        {
            ReadSectionLiteralPoolInReverse(currentSectionTable.back());
            //reverese since we are reading from the back
            std::reverse(section->valueMap.begin(), section->valueMap.end());
        }

        currentSectionTable.push_back(section);
        return;
    }
    //skipping first byte since that byte represents the address of the instruction 
    for (int i = 1; i < bytes.size(); ++i)
    {
        currentSectionTable.back()->sectionData.push_back(std::stoi(bytes[i], &size, HEX_BASE));
    }
}

void Linker::ReadRelocationsEntry(std::string& line, RelocationTable& currentRelocationTable)
{
    // name section offset type
    std::size_t size;
    auto relocationInfo  = Split(line, SPACE);
    auto relocationEntry = std::make_shared<AssemblyUtil::RelocationTableEntry>();

    relocationEntry->index   = std::stoi(Strip(relocationInfo.front(), ':'));
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
        if (section->sectionData.size() == 0)
            continue;
        
        unsigned long address = section->offset;
        unsigned long written = 0;
        file << address << ": ";
        for (const auto& byte: section->sectionData)
        {
            file << std::setw(2) << std::setfill('0') <<(short)byte << " ";
            ++written;
            if (++instructionCounter == 4)
            {
                file << "\n";
                instructionCounter = 0;
                if (written != section->sectionData.size())
                {
                    address += 4;
                    file << address << ": ";
                }
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
    for (const auto&  entry: mSymbolTable)
    {
        file << entry->label << " " << entry->section << " "
                << entry->offset << " " << entry->local << " "
                << entry->defined << "\n";
    }

    file << "\n" << "sections" << "\n";
    for (const auto&  section: mSectionTable)
    {
        file << section->name << " " << section->offset << "\n";
        auto i = 0;
        for (const auto&  byte: section->sectionData)
        {
            file << std::setw(2) << std::setfill('0') <<(short)byte << " ";
            if (++i == 4)
            {
                i = 0;
                file << "\n";
            }
        }
    }

        int i = 0;
    for (auto relocation : mRelocationTable)
    {
        file << ++i << ": " << relocation->label << SPACE << relocation->section << SPACE
            << relocation->offset << SPACE << relocation->type << " \n";
    }

    file.close();
}
