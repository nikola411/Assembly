#include "linker.hpp"


#include <fstream>
#include <iostream>
#include <string>
#include <set>

#define SPACE ' '

std::map<std::string, eInputFileState> StateMap =
{
    { "symbol table", eInputFileState::SYMBOL_TABLE },
    { "sections",     eInputFileState::SECTIONS },
    { "relocations",  eInputFileState::RELOCATIONS}
};

std::set<std::string> StateSet =
{
    "relocations", "symbol table", "sections"
};

Linker::Linker(LinkerArguments& args)
    : arguments(args)
{
}

void Linker::StartLinking()
{
}

void Linker::WriteOutput()
{
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
            if (line == "\n")
                continue;

            if (StateSet.find(line) != StateSet.end())
            {
                state = StateMap[line];
                continue;
            }

            switch (state)
            {
            case eInputFileState::SYMBOL_TABLE:
                ReadSymbolTableEntry(line);

                break;
            case eInputFileState::SECTIONS:
                ReadSectionsLine(line);
                break;
            case eInputFileState::RELOCATIONS:
                ReadRelocationsEntry(line);
                break;
            }
        }

        mSymbolTableVector.push_back(currentSymbolTable);
        mSectionTableVector.push_back(currentSectionsTable);
        mRelocationTableVector.push_back(currentRelocationTable);
    }
}

symbol_ptr Linker::ReadSymbolTableEntry(std::string& line)
{
}

section_ptr Linker::ReadSectionsLine(std::string& line)
{
    auto lineVector = Split(line, SPACE);
    if (lineVector.size() == 2)
    {
        return;
    }



}

relocation_ptr Linker::ReadRelocationsEntry(std::string& line)
{
}
