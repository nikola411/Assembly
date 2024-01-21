#pragma once

#include <string>
#include <vector>

#include "AssemblyUtil.hpp"

namespace LinkerUtil
{
    static const std::string OUT_FLAG("-o");
    static const std::string HEX_FLAG("-hex");
    static const std::string RELOCATABLE_FLAG("-relocatable");
    static const std::string LINKER_FLAG("./linker");
    static const std::string PLACE_FLAG("-place=");

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
        std::vector<std::string> place;
        std::string outFile;
        std::vector<std::string> inFiles;
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

    typedef std::vector<AssemblyUtil::symbol_ptr> SymbolTable;
    typedef std::vector<AssemblyUtil::section_ptr> SectionTable;
    typedef std::vector<AssemblyUtil::relocation_ptr> RelocationTable;
    typedef std::shared_ptr<SectionMergeData> SectionMergeDataPtr;
}