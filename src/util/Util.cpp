#include "Util.hpp"

int Contains(std::string& base, char c)
{
    for (auto i = 0; i < base.size(); ++i)
        if (base[i] == c)
            return i;

    return -1;
}

bool StartsWith(std::string& base, const std::string& start)
{
    if (start.size() > base.size()) return false;
    if (base.size() == 0) return false;

    for (auto i = 0; i < start.size(); ++i)
        if (base[i] != start[i])
            return false;
    
    return true;
}

std::vector<std::string> Split(std::string& toSplit, char c)
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
