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
    
    auto sectionSize = section->sectionData.size();
    if (offset + data.size() > section->sectionData.size())
    {
        auto size = (offset + data.size()) - sectionSize;
        for (auto i = 0; i < size; ++i)
            section->sectionData.push_back(EMPTY_BYTE);
    }

    for (auto byte: data)
        section->sectionData[offset++] = byte;

    section->locationCounter = section->sectionData.size();

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

bool AssemblyUtil::WriteDataToSection(AssemblyUtil::section_ptr section, uint32_t data, int offset, int bytesToWrite)
{
    if (offset < 0)
        return false;

    if (section->sectionData.size() < offset + bytesToWrite)
    {
        for (auto i = 0; i < bytesToWrite; ++i)
        {
            section->sectionData.push_back(EMPTY_BYTE);
        }
    }

    for (auto i = 0; i < bytesToWrite; i++)
    {
        uint8_t byte = data >> ((bytesToWrite - 1 - i) * BYTE_SIZE);
        section->sectionData[offset + i] = byte;
    }
        
    return true;
}

bool AssemblyUtil::AllocateSectionData(AssemblyUtil::section_ptr section, int size)
{
    for (auto i = 0; i < size; i++)
        section->sectionData.push_back(0x0);
        
    return true;
}

int LinkerUtil::Contains(std::string& base, char c)
{
    for (auto i = 0; i < base.size(); ++i)
        if (base[i] == c)
            return i;

    return -1;
}

bool LinkerUtil::StartsWith(std::string& base, std::string& start)
{
    if (start.size() > base.size()) return false;
    if (base.size() == 0) return false;

    for (auto i = 0; i < start.size(); ++i)
        if (base[i] != start[i])
            return false;
    
    return true;
}

std::vector<std::string> LinkerUtil::Split(std::string& toSplit, char c)
{
    std::vector<std::string> ret = {};
    
    auto i = Contains(toSplit, c);
    while (i != -1)
    {
        auto part = toSplit.substr(0, i);
        if (part == " ")
            continue;

        ret.push_back(part);
        toSplit = toSplit.substr(i + 1, toSplit.size());
        i = Contains(toSplit, c);
    }

    if (toSplit != " " && toSplit != "")
        ret.push_back(toSplit);

    return ret;
}

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

AssemblyUtil::relocation_ptr AssemblyUtil::FindRelocation(std::vector<AssemblyUtil::relocation_ptr>& relocations, std::string& name)
{
    for (auto relocation: relocations)
        if (relocation->label == name)
            return relocation;
    return nullptr;
}

AssemblyUtil::symbol_ptr AssemblyUtil::CreateSymbolEntry(std::string label, std::string section, int offset, bool local, bool defined, int index)
{
    return std::make_shared<SymbolTableEntry>(label, section, offset, local, defined, index);
}

bool AssemblyUtil::WriteDataToSection(std::vector<AssemblyUtil::section_ptr>& sectionTable, std::string& sectionName, uint32_t data, int offset, int bytesStart, int bytesToWrite)
{
    AssemblyUtil::section_ptr section = nullptr;
    for (auto sec: sectionTable)
    {
        if (sec->name == sectionName)
        {
            section = sec;
            break;
        }
    }

    if (section == nullptr)
        throw AssemblyUtil::AssemblyException("Section: " + sectionName + " does not exist.");

    if (section->sectionData.size() < offset + bytesToWrite)
        throw AssemblyUtil::AssemblyException("Trying to write out of section: " + sectionName);

    for (auto i = bytesStart; i < bytesToWrite; ++i)
    {
        auto byte = (data >> (i * BYTE_SIZE)) & 0x0F;
        section->sectionData[offset++] = byte;
    }

    return true;
}
