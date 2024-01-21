#include "LinkerUtil.hpp"

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