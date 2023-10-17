#include "util.hpp"
#include "instruction.hpp"

using namespace ParserUtil;

std::unordered_map<std::string, eGPR> GPRMap =
{
    { "r0", eGPR::R0 },   { "r1", eGPR::R1 },   { "r2", eGPR::R2 },   { "r3", eGPR::R3 },
    { "r4", eGPR::R4 },   { "r5", eGPR::R5 },   { "r6", eGPR::R6 },   { "r7", eGPR::R7 },
    { "r8", eGPR::R8 },   { "r9", eGPR::R9 },   { "r10", eGPR::R10 }, { "r11", eGPR::R11 },
    { "r12", eGPR::R12 }, { "r13", eGPR::R13 }, { "r14", eGPR::R14 }, { "r15", eGPR::R15 }
};

eGPR ParserUtil::GPRStringToEnum(std::string reg)
{
    return GPRMap[reg];
}

std::unordered_map<std::string, eCSR> CSRMap =
{
    { "status",  eCSR::STATUS },
    { "handler", eCSR::HANDLER },
    { "cause",   eCSR::CAUSE }
};

eCSR ParserUtil::CSRStringToEnum(std::string csr)
{
    return CSRMap[csr];
}

AssemblyUtil::symbol_ptr AssemblyUtil::FindSymbol(std::vector<symbol_ptr>& table, std::string& name)
{
    for (auto entry: table)
    {
        if (entry->label == name)
        {
            return entry;
        }
    }

    return nullptr;
}

bool AssemblyUtil::InsertSymbol(std::vector<symbol_ptr>& table, symbol_ptr symbol)
{
    if (FindSymbol(table, symbol->label) == nullptr)
    {
        table.push_back(symbol);
        return true;
    }

    return false;
}

bool AssemblyUtil::UpdateSymbolOffset(std::vector<symbol_ptr>& table, symbol_ptr symbol, int offset)
{
    auto entry = FindSymbol(table, symbol->label);
    if (entry == nullptr)
        return false;
    
    if (entry->offset != offset)
    {
        entry->offset = offset;
        return true;
    }

    return false;
}

bool AssemblyUtil::UpdateSymbolSection(std::vector<symbol_ptr>& table, symbol_ptr symbol, std::string section)
{
    auto entry = FindSymbol(table, symbol->label);
    if (entry == nullptr)
        return false;

    if (entry->section.empty())
    {
        entry->section = section;
        return true;
    }

    return false;
}

bool AssemblyUtil::UpdateSymbolLocality(std::vector<symbol_ptr>& table, symbol_ptr symbol, bool isLocal)
{
    auto entry = FindSymbol(table, symbol->label);
    if (entry != nullptr)
    {
        entry->local = isLocal;
        return true;
    }

    return false;
}

bool AssemblyUtil::WriteDataToSection(section_ptr section, const std::vector<BYTE>& data, int offset)
{
    if (section->sectionData.size() < offset )
        return false;
    
    if (offset + data.size() > section->sectionData.size())
        return false;

    for (auto byte: data)
        section->sectionData[++offset] = byte;

    return true;
}

std::vector<BYTE> AssemblyUtil::ReadDataFromSection(std::vector<AssemblyUtil::section_ptr>& sections, std::string& name, int offsetStart, int offsetEnd)
{
    AssemblyUtil::section_ptr section = nullptr;
    for (auto elem: sections)
    {
        if (elem->name == name)
        {
            section = elem;
            break;
        }
    }

    if (section == nullptr)
        throw AssemblyUtil::AssemblyException("Section does not exist");

    if (offsetStart > section->sectionData.size() || offsetEnd > section->sectionData.size())
        throw AssemblyUtil::AssemblyException(
            "Wrong range for reading from section (" + section->name + ") in file" + __FILE__ + " on line:" + std::to_string(__LINE__));

    auto ret = std::vector<BYTE>();

    for (auto i = offsetStart; i < offsetEnd; i++)
    {
        ret.push_back(section->sectionData[i]);
    }

    return ret;
}
