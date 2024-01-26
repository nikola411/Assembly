#include "LinkerUtil.hpp"

#include "Util.hpp"

void LinkerUtil::UpdateSymbolsOffset(std::vector<AssemblyUtil::symbol_ptr>& table, std::string& sectionName, int offset)
{
    for (auto symbol: table)
    {
        if (symbol->section == sectionName)
        {
            symbol->offset += offset;
            break;
        } 
    }
}

void LinkerUtil::UpdateRelocationsOffset(std::vector<AssemblyUtil::relocation_ptr>& table, std::string& sectionName, int offset)
{
    for (auto relocation: table)
    {
        if (relocation->section == sectionName)
        {
            relocation->offset += offset;
            break;
        }
    }
}

void LinkerUtil::ReadSectionLiteralPoolInReverse(AssemblyUtil::section_ptr section)
{
    int count = section->valueMapSize;
    for (int j = 0; j < count; ++j)
    {
        uint32_t literal = 0x0;
        for (int i = 0; i < 4; ++i)
        {
            BYTE byte = section->sectionData.back();
            section->sectionData.pop_back();

            literal |= (byte << (BYTE_SIZE * i));
        }

        section->valueMap.push_back(literal);
    }
}

void LinkerUtil::LinkerArguments::ParseArguments(std::vector<std::string>& argVector)
{
    ArgumentParsingState next = DO_NOTHING;

    for (auto arg: argVector)
    {
        if (arg == FlagLinker)
            continue;

        if (arg == FlagHex)
        {
            if (relocatable)
                throw LinkerException("Cannot have -relocatable and -hex together");
            hex = true;
            continue;
        }

        if (arg == FlagRelocatable)
        {
            if (hex)
                throw LinkerException("Cannot have -relocatable and -hex together");
            relocatable = true;
            continue;
        }
        
        if (arg == FlagOut)
        {
            next = READ_OUTPUT;
            continue;
        }

        if (StartsWith(arg, FlagPlace))
        {
            auto directive = arg.substr(FlagPlace.size(), arg.size());
            auto parts = Split(directive, '@');

            if (parts.size() != 2)
                throw LinkerException("Wrong argument format for place command.");

            std::size_t pos{};
            int base(16);

            place.push_back({parts[0], std::stoul(parts[1], &pos, base)});
            continue;
        }

        if (next == READ_OUTPUT)
        {
            outFile = arg;
            next = DO_NOTHING;
            continue;
        }

        inFiles.push_back(arg);
    }
}