#include "AssemblyUtil.hpp"
#include "Instruction.hpp"

using namespace ParserUtil;

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
