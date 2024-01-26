#pragma once

#include <string>
#include <vector>

#include "AssemblyUtil.hpp"

namespace LinkerUtil
{
    static const std::string FlagOut("-o");
    static const std::string FlagHex("-hex");
    static const std::string FlagRelocatable("-relocatable");
    static const std::string FlagLinker("./linker");
    static const std::string FlagPlace("-place=");

    class LinkerException: std::exception
    {
    public:
        LinkerException(std::string msg):msg(msg){}
        std::string what(){return msg;}
    private:
        std::string msg;
    };

    struct LinkerArguments
    {
        bool hex;
        bool relocatable;
        std::vector<std::pair<std::string, unsigned long>> place;
        std::string outFile;
        std::vector<std::string> inFiles;

        /*
            linker [opcije] <naziv_ulazne_datoteke>...
            [opcije]:
                * -hex
                * -o <naziv_izlazne_datoteke>
                * -place=<ime_sekcije>@<adresa>
                * -relocatable
        */
        void ParseArguments(std::vector<std::string>& argVector);
    };

    enum ArgumentParsingState
    {
        DO_NOTHING = 0x00,
        READ_OUTPUT = 0x01
    };

    enum eInputFileState
    {
        SYMBOL_TABLE,
        SECTIONS,
        RELOCATIONS
    };

    struct SectionMergeData
    {
        int index;
        std::string name;
        std::vector<std::pair<int, AssemblyUtil::section_ptr>> sections;

        void AddSection(int file, AssemblyUtil::section_ptr section)
        {
            sections.push_back({ file, section });
        }
    };

    void UpdateSymbolsOffset(std::vector<AssemblyUtil::symbol_ptr>& table, std::string& sectionName, int offset);
    void UpdateRelocationsOffset(std::vector<AssemblyUtil::relocation_ptr>& table, std::string& sectionName, int offset);
    void ReadSectionLiteralPoolInReverse(AssemblyUtil::section_ptr section);

    typedef std::vector<AssemblyUtil::symbol_ptr> SymbolTable;
    typedef std::vector<AssemblyUtil::section_ptr> SectionTable;
    typedef std::vector<AssemblyUtil::relocation_ptr> RelocationTable;
    typedef std::shared_ptr<SectionMergeData> SectionMergeDataPtr;
}